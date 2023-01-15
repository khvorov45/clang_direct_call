//===-- driver.cpp - Clang GCC-Compatible Driver --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This is the entry point to the clang driver; it is a thin wrapper
// for functionality in the Driver clang library.
//
//===----------------------------------------------------------------------===//

#include "llvm_lib_Target_X86_TargetInfo_X86TargetInfo.h"
#include "llvm_lib_Target_X86_X86TargetMachine.h"
#include "llvm_lib_Target_X86_X86.h"

#include "llvm_include_llvm_MC_TargetRegistry.h"
#include "llvm_include_llvm_InitializePasses.h"

#include "clang_include_clang_Driver_Driver.h"
#include "clang_include_clang_Basic_DiagnosticOptions.h"
#include "clang_include_clang_Basic_HeaderInclude.h"
#include "clang_include_clang_Config_config.h"
#include "clang_include_clang_Driver_Compilation.h"
#include "clang_include_clang_Driver_DriverDiagnostic.h"
#include "clang_include_clang_Driver_Options.h"
#include "clang_include_clang_Driver_ToolChain.h"
#include "clang_include_clang_Frontend_ChainedDiagnosticConsumer.h"
#include "clang_include_clang_Frontend_CompilerInvocation.h"
#include "clang_include_clang_Frontend_SerializedDiagnosticPrinter.h"
#include "clang_include_clang_Frontend_TextDiagnosticPrinter.h"
#include "clang_include_clang_Frontend_Utils.h"
#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_ADT_SmallString.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_Option_ArgList.h"
#include "llvm_include_llvm_Option_OptTable.h"
#include "llvm_include_llvm_Option_Option.h"
#include "llvm_include_llvm_Support_BuryPointer.h"
#include "llvm_include_llvm_Support_CommandLine.h"
#include "llvm_include_llvm_Support_CrashRecoveryContext.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_FileSystem.h"
#include "llvm_include_llvm_Support_Host.h"
#include "llvm_include_llvm_Support_Path.h"
#include "llvm_include_llvm_Support_PrettyStackTrace.h"
#include "llvm_include_llvm_Support_Process.h"
#include "llvm_include_llvm_Support_Program.h"
#include "llvm_include_llvm_Support_Regex.h"
#include "llvm_include_llvm_Support_Signals.h"
#include "llvm_include_llvm_Support_StringSaver.h"
#include "llvm_include_llvm_Support_TargetSelect.h"
#include "llvm_include_llvm_Support_Timer.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <memory>
#include <optional>
#include <set>
#include <system_error>
using namespace clang;
using namespace clang::driver;
using namespace llvm::opt;

std::string
GetExecutablePath(const char* Argv0) {
    // This just needs to be some symbol in the binary; C++ doesn't
    // allow taking the address of ::main however.
    void* P = (void*)(intptr_t)GetExecutablePath;
    return llvm::sys::fs::getMainExecutable(Argv0, P);
}

static const char*
GetStableCStr(std::set<std::string>& SavedStrings, StringRef S) {
    return SavedStrings.insert(std::string(S)).first->c_str();
}

/// ApplyQAOverride - Apply a list of edits to the input argument lists.
///
/// The input string is a space separate list of edits to perform,
/// they are applied in order to the input argument lists. Edits
/// should be one of the following forms:
///
///  '#': Silence information about the changes to the command line arguments.
///
///  '^': Add FOO as a new argument at the beginning of the command line.
///
///  '+': Add FOO as a new argument at the end of the command line.
///
///  's/XXX/YYY/': Substitute the regular expression XXX with YYY in the command
///  line.
///
///  'xOPTION': Removes all instances of the literal argument OPTION.
///
///  'XOPTION': Removes all instances of the literal argument OPTION,
///  and the following argument.
///
///  'Ox': Removes all flags matching 'O' or 'O[sz0-9]' and adds 'Ox'
///  at the end of the command line.
///
/// \param OS - The stream to write edit information to.
/// \param Args - The vector of command line arguments.
/// \param Edit - The override command to perform.
/// \param SavedStrings - Set to use for storing string representations.
static void
ApplyOneQAOverride(raw_ostream& OS, SmallVectorImpl<const char*>& Args, StringRef Edit, std::set<std::string>& SavedStrings) {
    // This does not need to be efficient.

    if (Edit[0] == '^') {
        const char* Str =
            GetStableCStr(SavedStrings, Edit.substr(1));
        OS << "### Adding argument " << Str << " at beginning\n";
        Args.insert(Args.begin() + 1, Str);
    } else if (Edit[0] == '+') {
        const char* Str =
            GetStableCStr(SavedStrings, Edit.substr(1));
        OS << "### Adding argument " << Str << " at end\n";
        Args.push_back(Str);
    } else if (Edit[0] == 's' && Edit[1] == '/' && Edit.endswith("/") && Edit.slice(2, Edit.size() - 1).contains('/')) {
        StringRef MatchPattern = Edit.substr(2).split('/').first;
        StringRef ReplPattern = Edit.substr(2).split('/').second;
        ReplPattern = ReplPattern.slice(0, ReplPattern.size() - 1);

        for (unsigned i = 1, e = Args.size(); i != e; ++i) {
            // Ignore end-of-line response file markers
            if (Args[i] == nullptr)
                continue;
            std::string Repl = llvm::Regex(MatchPattern).sub(ReplPattern, Args[i]);

            if (Repl != Args[i]) {
                OS << "### Replacing '" << Args[i] << "' with '" << Repl << "'\n";
                Args[i] = GetStableCStr(SavedStrings, Repl);
            }
        }
    } else if (Edit[0] == 'x' || Edit[0] == 'X') {
        auto Option = Edit.substr(1);
        for (unsigned i = 1; i < Args.size();) {
            if (Option == Args[i]) {
                OS << "### Deleting argument " << Args[i] << '\n';
                Args.erase(Args.begin() + i);
                if (Edit[0] == 'X') {
                    if (i < Args.size()) {
                        OS << "### Deleting argument " << Args[i] << '\n';
                        Args.erase(Args.begin() + i);
                    } else
                        OS << "### Invalid X edit, end of command line!\n";
                }
            } else
                ++i;
        }
    } else if (Edit[0] == 'O') {
        for (unsigned i = 1; i < Args.size();) {
            const char* A = Args[i];
            // Ignore end-of-line response file markers
            if (A == nullptr)
                continue;
            if (A[0] == '-' && A[1] == 'O' && (A[2] == '\0' || (A[3] == '\0' && (A[2] == 's' || A[2] == 'z' || ('0' <= A[2] && A[2] <= '9'))))) {
                OS << "### Deleting argument " << Args[i] << '\n';
                Args.erase(Args.begin() + i);
            } else
                ++i;
        }
        OS << "### Adding argument " << Edit << " at end\n";
        Args.push_back(GetStableCStr(SavedStrings, '-' + Edit.str()));
    } else {
        OS << "### Unrecognized edit: " << Edit << "\n";
    }
}

/// ApplyQAOverride - Apply a comma separate list of edits to the
/// input argument lists. See ApplyOneQAOverride.
static void
ApplyQAOverride(SmallVectorImpl<const char*>& Args, const char* OverrideStr, std::set<std::string>& SavedStrings) {
    raw_ostream* OS = &llvm::errs();

    if (OverrideStr[0] == '#') {
        ++OverrideStr;
        OS = &llvm::nulls();
    }

    *OS << "### CCC_OVERRIDE_OPTIONS: " << OverrideStr << "\n";

    // This does not need to be efficient.

    const char* S = OverrideStr;
    while (*S) {
        const char* End = ::strchr(S, ' ');
        if (!End)
            End = S + strlen(S);
        if (End != S)
            ApplyOneQAOverride(*OS, Args, std::string(S, End), SavedStrings);
        S = End;
        if (*S != '\0')
            ++S;
    }
}

extern int cc1_main(ArrayRef<const char*> Argv, const char* Argv0, void* MainAddr);
extern int cc1as_main(ArrayRef<const char*> Argv, const char* Argv0, void* MainAddr);
extern int cc1gen_reproducer_main(ArrayRef<const char*> Argv, const char* Argv0, void* MainAddr);

static void
insertTargetAndModeArgs(const ParsedClangName& NameParts, SmallVectorImpl<const char*>& ArgVector, std::set<std::string>& SavedStrings) {
    // Put target and mode arguments at the start of argument list so that
    // arguments specified in command line could override them. Avoid putting
    // them at index 0, as an option like '-cc1' must remain the first.
    int InsertionPoint = 0;
    if (ArgVector.size() > 0)
        ++InsertionPoint;

    if (NameParts.DriverMode) {
        // Add the mode flag to the arguments.
        ArgVector.insert(ArgVector.begin() + InsertionPoint, GetStableCStr(SavedStrings, NameParts.DriverMode));
    }

    if (NameParts.TargetIsValid) {
        const char* arr[] = {"-target", GetStableCStr(SavedStrings, NameParts.TargetPrefix)};
        ArgVector.insert(ArgVector.begin() + InsertionPoint, std::begin(arr), std::end(arr));
    }
}

static void
getCLEnvVarOptions(std::string& EnvValue, llvm::StringSaver& Saver, SmallVectorImpl<const char*>& Opts) {
    llvm::cl::TokenizeWindowsCommandLine(EnvValue, Saver, Opts);
    // The first instance of '#' should be replaced with '=' in each option.
    for (const char* Opt : Opts)
        if (char* NumberSignPtr = const_cast<char*>(::strchr(Opt, '#')))
            *NumberSignPtr = '=';
}

template<class T>
static T
checkEnvVar(const char* EnvOptSet, const char* EnvOptFile, std::string& OptFile) {
    T OptVal = ::getenv(EnvOptSet);
    if (OptVal) {
        if (const char* Var = ::getenv(EnvOptFile))
            OptFile = Var;
    }
    return OptVal;
}

static void
FixupDiagPrefixExeName(TextDiagnosticPrinter* DiagClient, const std::string& Path) {
    // If the clang binary happens to be named cl.exe for compatibility reasons,
    // use clang-cl.exe as the prefix to avoid confusion between clang and MSVC.
    StringRef ExeBasename(llvm::sys::path::stem(Path));
    if (ExeBasename.equals_insensitive("cl"))
        ExeBasename = "clang-cl";
    DiagClient->setPrefix(std::string(ExeBasename));
}

static void
SetInstallDir(SmallVectorImpl<const char*>& argv, Driver& TheDriver, bool CanonicalPrefixes) {
    // Attempt to find the original path used to invoke the driver, to determine
    // the installed path. We do this manually, because we want to support that
    // path being a symlink.
    SmallString<128> InstalledPath(argv[0]);

    // Do a PATH lookup, if there are no directory components.
    if (llvm::sys::path::filename(InstalledPath) == InstalledPath)
        if (llvm::ErrorOr<std::string> Tmp = llvm::sys::findProgramByName(
                llvm::sys::path::filename(InstalledPath.str())
            ))
            InstalledPath = *Tmp;

    // FIXME: We don't actually canonicalize this, we just make it absolute.
    if (CanonicalPrefixes)
        llvm::sys::fs::make_absolute(InstalledPath);

    StringRef InstalledPathParent(llvm::sys::path::parent_path(InstalledPath));
    if (llvm::sys::fs::exists(InstalledPathParent))
        TheDriver.setInstalledDir(InstalledPathParent);
}

static int
ExecuteCC1Tool(SmallVectorImpl<const char*>& ArgV) {
    // If we call the cc1 tool from the clangDriver library (through
    // Driver::CC1Main), we need to clean up the options usage count. The options
    // are currently global, and they might have been used previously by the
    // driver.
    llvm::cl::ResetAllOptionOccurrences();

    llvm::BumpPtrAllocator     A;
    llvm::cl::ExpansionContext ECtx(A, llvm::cl::TokenizeGNUCommandLine);
    if (llvm::Error Err = ECtx.expandResponseFiles(ArgV)) {
        llvm::errs() << toString(std::move(Err)) << '\n';
        return 1;
    }
    StringRef Tool = ArgV[1];
    void*     GetExecutablePathVP = (void*)(intptr_t)GetExecutablePath;
    if (Tool == "-cc1")
        return cc1_main(makeArrayRef(ArgV).slice(1), ArgV[0], GetExecutablePathVP);
    if (Tool == "-cc1as")
        return cc1as_main(makeArrayRef(ArgV).slice(2), ArgV[0], GetExecutablePathVP);
    if (Tool == "-cc1gen-reproducer")
        return cc1gen_reproducer_main(makeArrayRef(ArgV).slice(2), ArgV[0], GetExecutablePathVP);
    // Reject unknown tools.
    llvm::errs() << "error: unknown integrated tool '" << Tool << "'. "
                 << "Valid tools include '-cc1' and '-cc1as'.\n";
    return 1;
}

#define mdc_assert assert

bool
mdc_memeq(const void* ptr1, const void* ptr2, intptr_t bytes) {
    mdc_assert(bytes >= 0);
    bool result = true;
    for (intptr_t index = 0; index < bytes; index++) {
        if (((uint8_t*)ptr1)[index] != ((uint8_t*)ptr2)[index]) {
            result = false;
            break;
        }
    }
    return result;
}

// clang-format off
#define mdc_STR(x) (mdc_Str) { x, mdc_strlen(x) }
// clang-format on

typedef struct mdc_Str {
    const char* ptr;
    intptr_t    len;
} mdc_Str;

intptr_t
mdc_strlen(const char* str) {
    intptr_t result = 0;
    if (str) {
        while (str[result] != '\0') {
            result += 1;
        }
    }
    return result;
}

bool
mdc_strStartsWith(mdc_Str str, mdc_Str pattern) {
    bool result = false;
    if (pattern.len <= str.len) {
        result = mdc_memeq(str.ptr, pattern.ptr, pattern.len);
    }
    return result;
}

static bool
LLVMX8664MatchProc(llvm::Triple::ArchType archType) {
    bool result = archType == llvm::Triple::x86_64;
    return result;
}

static llvm::TargetMachine*
LLVMX86TargetMachineProc(const llvm::Target& T, const llvm::Triple& TT, llvm::StringRef CPU, llvm::StringRef FS, const llvm::TargetOptions& Options, std::optional<llvm::Reloc::Model> RM, std::optional<llvm::CodeModel::Model> CM, llvm::CodeGenOpt::Level OL, bool JIT) {
    return new llvm::X86TargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT);
}

extern "C" int
clang_main(int argc, char** argv) {
    {
        llvm::Target& x8664Target = llvm::getTheX86_64Target();
        llvm::TargetRegistry::RegisterTarget(x8664Target, "x86-64", "64-bit X86: EM64T and AMD64", "X86", LLVMX8664MatchProc, true);
        llvm::TargetRegistry::RegisterTargetMachine(x8664Target, LLVMX86TargetMachineProc);

        llvm::PassRegistry& PR = *llvm::PassRegistry::getPassRegistry();
        llvm::initializeX86LowerAMXIntrinsicsLegacyPassPass(PR);
        llvm::initializeX86LowerAMXTypeLegacyPassPass(PR);
        llvm::initializeX86PreAMXConfigPassPass(PR);
        llvm::initializeX86PreTileConfigPass(PR);
        llvm::initializeGlobalISel(PR);
        llvm::initializeWinEHStatePassPass(PR);
        llvm::initializeFixupBWInstPassPass(PR);
        llvm::initializeEvexToVexInstPassPass(PR);
        llvm::initializeFixupLEAPassPass(PR);
        llvm::initializeFPSPass(PR);
        llvm::initializeX86FixupSetCCPassPass(PR);
        llvm::initializeX86CallFrameOptimizationPass(PR);
        llvm::initializeX86CmovConverterPassPass(PR);
        llvm::initializeX86TileConfigPass(PR);
        llvm::initializeX86FastPreTileConfigPass(PR);
        llvm::initializeX86FastTileConfigPass(PR);
        llvm::initializeX86KCFIPass(PR);
        llvm::initializeX86LowerTileCopyPass(PR);
        llvm::initializeX86ExpandPseudoPass(PR);
        llvm::initializeX86ExecutionDomainFixPass(PR);
        llvm::initializeX86DomainReassignmentPass(PR);
        llvm::initializeX86AvoidSFBPassPass(PR);
        llvm::initializeX86AvoidTrailingCallPassPass(PR);
        llvm::initializeX86SpeculativeLoadHardeningPassPass(PR);
        llvm::initializeX86SpeculativeExecutionSideEffectSuppressionPass(PR);
        llvm::initializeX86FlagsCopyLoweringPassPass(PR);
        llvm::initializeX86LoadValueInjectionLoadHardeningPassPass(PR);
        llvm::initializeX86LoadValueInjectionRetHardeningPassPass(PR);
        llvm::initializeX86OptimizeLEAPassPass(PR);
        llvm::initializeX86PartialReductionPass(PR);
        llvm::initializePseudoProbeInserterPass(PR);
        llvm::initializeX86ReturnThunksPass(PR);
        llvm::initializeX86DAGToDAGISelPass(PR);
    }

    SmallVector<const char*, 256> Args(argv, argv + argc);

    // Handle -cc1 integrated tools, even if -cc1 was expanded from a response
    // file.
    if (argc >= 2) {
        mdc_Str firstArg = mdc_STR(argv[1]);
        if (mdc_strStartsWith(firstArg, mdc_STR("-cc1"))) {
            return ExecuteCC1Tool(Args);
        }
    }

    std::string Path = GetExecutablePath(Args[0]);

    IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = CreateAndPopulateDiagOpts(Args);

    TextDiagnosticPrinter* DiagClient = new TextDiagnosticPrinter(llvm::errs(), &*DiagOpts);
    FixupDiagPrefixExeName(DiagClient, Path);

    IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());

    DiagnosticsEngine Diags(DiagID, &*DiagOpts, DiagClient);

    if (!DiagOpts->DiagnosticSerializationFile.empty()) {
        auto SerializedConsumer =
            clang::serialized_diags::create(DiagOpts->DiagnosticSerializationFile, &*DiagOpts, /*MergeChildRecords=*/true);
        Diags.setClient(new ChainedDiagnosticConsumer(
            Diags.takeClient(),
            std::move(SerializedConsumer)
        ));
    }

    ProcessWarningOptions(Diags, *DiagOpts, /*ReportDiags=*/false);

    Driver TheDriver(Path, llvm::sys::getDefaultTargetTriple(), Diags);
    SetInstallDir(Args, TheDriver, true);
    auto TargetAndMode = ToolChain::getTargetAndModeFromProgramName(Args[0]);
    TheDriver.setTargetAndMode(TargetAndMode);

    std::set<std::string> SavedStrings;
    insertTargetAndModeArgs(TargetAndMode, Args, SavedStrings);

    TheDriver.CC1Main = &ExecuteCC1Tool;
    // Ensure the CC1Command actually catches cc1 crashes
    llvm::CrashRecoveryContext::Enable();

    std::unique_ptr<Compilation> Comp(TheDriver.BuildCompilation(Args));

    Driver::ReproLevel ReproLevel = Driver::ReproLevel::OnCrash;

    int                   Res = 1;
    bool                  IsCrash = false;
    Driver::CommandStatus CommandStatus = Driver::CommandStatus::Ok;
    // Pretend the first command failed if ReproStatus is Always.
    const Command* FailingCommand = nullptr;
    if (!Comp->getJobs().empty())
        FailingCommand = &*Comp->getJobs().begin();
    if (Comp && !Comp->containsError()) {
        SmallVector<std::pair<int, const Command*>, 4> FailingCommands;
        Res = TheDriver.ExecuteCompilation(*Comp, FailingCommands);

        for (const auto& P : FailingCommands) {
            int CommandRes = P.first;
            FailingCommand = P.second;
            if (!Res)
                Res = CommandRes;

            // If result status is < 0, then the driver command signalled an error.
            // If result status is 70, then the driver command reported a fatal error.
            // On Windows, abort will return an exit code of 3.  In these cases,
            // generate additional diagnostic information if possible.
            IsCrash = CommandRes < 0 || CommandRes == 70;
#ifdef _WIN32
            IsCrash |= CommandRes == 3;
#endif
#if LLVM_ON_UNIX
            // When running in integrated-cc1 mode, the CrashRecoveryContext returns
            // the same codes as if the program crashed. See section "Exit Status for
            // Commands":
            // https://pubs.opengroup.org/onlinepubs/9699919799/xrat/V4_xcu_chap02.html
            IsCrash |= CommandRes > 128;
#endif
            CommandStatus =
                IsCrash ? Driver::CommandStatus::Crash : Driver::CommandStatus::Error;
            if (IsCrash)
                break;
        }
    }

    if (FailingCommand != nullptr && TheDriver.maybeGenerateCompilationDiagnostics(CommandStatus, ReproLevel, *Comp, *FailingCommand))
        Res = 1;

    Diags.getClient()->finish();

    if (IsCrash) {
        // When crashing in -fintegrated-cc1 mode, bury the timer pointers, because
        // the internal linked list might point to already released stack frames.
        llvm::BuryPointer(llvm::TimerGroup::aquireDefaultGroup());
    } else {
        // If any timers were active but haven't been destroyed yet, print their
        // results now.  This happens in -disable-free mode.
        llvm::TimerGroup::printAll(llvm::errs());
        llvm::TimerGroup::clearAll();
    }

#ifdef _WIN32
    // Exit status should not be negative on Win32, unless abnormal termination.
    // Once abnormal termination was caught, negative status should not be
    // propagated.
    if (Res < 0)
        Res = 1;
#endif

    // If we have multiple failing commands, we return the result of the first
    // failing command.
    return Res;
}
