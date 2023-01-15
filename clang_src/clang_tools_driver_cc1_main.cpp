#include "llvm_lib_Target_X86_TargetInfo_X86TargetInfo.h"
#include "llvm_lib_Target_X86_X86TargetMachine.h"
#include "llvm_lib_Target_X86_X86.h"

#include "llvm_include_llvm_InitializePasses.h"

#include "clang_include_clang_Basic_TargetOptions.h"
#include "clang_include_clang_CodeGen_ObjectFilePCHContainerOperations.h"
#include "clang_include_clang_Config_config.h"
#include "clang_include_clang_Driver_DriverDiagnostic.h"
#include "clang_include_clang_Driver_Options.h"
#include "clang_include_clang_Frontend_CompilerInstance.h"
#include "clang_include_clang_Frontend_CompilerInvocation.h"
#include "clang_include_clang_Frontend_FrontendDiagnostic.h"
#include "clang_include_clang_Frontend_TextDiagnosticBuffer.h"
#include "clang_include_clang_Frontend_TextDiagnosticPrinter.h"
#include "clang_include_clang_Frontend_Utils.h"
#include "clang_include_clang_FrontendTool_Utils.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_Config_llvm-config.h"
#include "llvm_include_llvm_LinkAllPasses.h"
#include "llvm_include_llvm_MC_TargetRegistry.h"
#include "llvm_include_llvm_Option_Arg.h"
#include "llvm_include_llvm_Option_ArgList.h"
#include "llvm_include_llvm_Option_OptTable.h"
#include "llvm_include_llvm_Support_BuryPointer.h"
#include "llvm_include_llvm_Support_Compiler.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_ManagedStatic.h"
#include "llvm_include_llvm_Support_Path.h"
#include "llvm_include_llvm_Support_Process.h"
#include "llvm_include_llvm_Support_Signals.h"
#include "llvm_include_llvm_Support_TargetSelect.h"
#include "llvm_include_llvm_Support_TimeProfiler.h"
#include "llvm_include_llvm_Support_Timer.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include "llvm_include_llvm_Target_TargetMachine.h"
#include <cstdio>

#ifdef CLANG_HAVE_RLIMITS
#include <sys/resource.h>
#endif

using namespace clang;
using namespace llvm::opt;

//===----------------------------------------------------------------------===//
// Main driver
//===----------------------------------------------------------------------===//

static void
LLVMErrorHandler(void* UserData, const char* Message, bool GenCrashDiag) {
    DiagnosticsEngine& Diags = *static_cast<DiagnosticsEngine*>(UserData);

    Diags.Report(diag::err_fe_error_backend) << Message;

    // Run the interrupt handlers to make sure any special cleanups get done, in
    // particular that we remove files registered with RemoveFileOnSignal.
    llvm::sys::RunInterruptHandlers();

    // We cannot recover from llvm errors.  When reporting a fatal error, exit
    // with status 70 to generate crash diagnostics.  For BSD systems this is
    // defined as an internal software error.  Otherwise, exit with status 1.
    llvm::sys::Process::Exit(GenCrashDiag ? 70 : 1);
}

#ifdef CLANG_HAVE_RLIMITS
#if defined(__linux__) && defined(__PIE__)
static size_t
getCurrentStackAllocation() {
    // If we can't compute the current stack usage, allow for 512K of command
    // line arguments and environment.
    size_t Usage = 512 * 1024;
    if (FILE* StatFile = fopen("/proc/self/stat", "r")) {
        // We assume that the stack extends from its current address to the end of
        // the environment space. In reality, there is another string literal (the
        // program name) after the environment, but this is close enough (we only
        // need to be within 100K or so).
        unsigned long StackPtr, EnvEnd;
        // Disable silly GCC -Wformat warning that complains about length
        // modifiers on ignored format specifiers. We want to retain these
        // for documentation purposes even though they have no effect.
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#endif
        if (fscanf(StatFile,
                   "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu "
                   "%*lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %*lu %*ld %*lu %*lu "
                   "%*lu %*lu %lu %*lu %*lu %*lu %*lu %*lu %*llu %*lu %*lu %*d %*d "
                   "%*u %*u %*llu %*lu %*ld %*lu %*lu %*lu %*lu %*lu %*lu %lu %*d",
                   &StackPtr,
                   &EnvEnd)
            == 2) {
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
            Usage = StackPtr < EnvEnd ? EnvEnd - StackPtr : StackPtr - EnvEnd;
        }
        fclose(StatFile);
    }
    return Usage;
}

#include <alloca.h>

LLVM_ATTRIBUTE_NOINLINE
static void
ensureStackAddressSpace() {
    // Linux kernels prior to 4.1 will sometimes locate the heap of a PIE binary
    // relatively close to the stack (they are only guaranteed to be 128MiB
    // apart). This results in crashes if we happen to heap-allocate more than
    // 128MiB before we reach our stack high-water mark.
    //
    // To avoid these crashes, ensure that we have sufficient virtual memory
    // pages allocated before we start running.
    size_t    Curr = getCurrentStackAllocation();
    const int kTargetStack = DesiredStackSize - 256 * 1024;
    if (Curr < kTargetStack) {
        volatile char* volatile Alloc =
            static_cast<volatile char*>(alloca(kTargetStack - Curr));
        Alloc[0] = 0;
        Alloc[kTargetStack - Curr - 1] = 0;
    }
}
#else
static void
ensureStackAddressSpace() {}
#endif

/// Attempt to ensure that we have at least 8MiB of usable stack space.
static void
ensureSufficientStack() {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
        return;

    // Increase the soft stack limit to our desired level, if necessary and
    // possible.
    if (rlim.rlim_cur != RLIM_INFINITY && rlim.rlim_cur < rlim_t(DesiredStackSize)) {
        // Try to allocate sufficient stack.
        if (rlim.rlim_max == RLIM_INFINITY || rlim.rlim_max >= rlim_t(DesiredStackSize))
            rlim.rlim_cur = DesiredStackSize;
        else if (rlim.rlim_cur == rlim.rlim_max)
            return;
        else
            rlim.rlim_cur = rlim.rlim_max;

        if (setrlimit(RLIMIT_STACK, &rlim) != 0 || rlim.rlim_cur != DesiredStackSize)
            return;
    }

    // We should now have a stack of size at least DesiredStackSize. Ensure
    // that we can actually use that much, if necessary.
    ensureStackAddressSpace();
}
#else
static void
ensureSufficientStack() {}
#endif

/// Print supported cpus of the given target.
static int
PrintSupportedCPUs(std::string TargetStr) {
    std::string         Error;
    const llvm::Target* TheTarget =
        llvm::TargetRegistry::lookupTarget(TargetStr, Error);
    if (!TheTarget) {
        llvm::errs() << Error;
        return 1;
    }

    // the target machine will handle the mcpu printing
    llvm::TargetOptions                  Options;
    std::unique_ptr<llvm::TargetMachine> TheTargetMachine(
        TheTarget->createTargetMachine(TargetStr, "", "+cpuhelp", Options, std::nullopt)
    );
    return 0;
}

// clang-format off
#define mdc_STR(x) (mdc_Str) { x, mdc_strlen(x) }

#ifndef mdc_assert
#define mdc_assert(condition) assert(condition)
#endif
// clang-format on

typedef struct mdc_Str {
    const char* ptr;
    intptr_t    len;
} mdc_Str;

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

bool
mdc_streq(mdc_Str str1, mdc_Str str2) {
    bool result = false;
    if (str1.len == str2.len) {
        result = mdc_memeq(str1.ptr, str2.ptr, str1.len);
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
cc1_main(int argc, char** argv) {
    mdc_assert(argc >= 2);
    mdc_assert(mdc_streq(mdc_STR(argv[1]), mdc_STR("-cc1")));

    // NOTE(khvorov) Init
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

    const char*                   Argv0 = argv[0];
    SmallVector<const char*, 256> Args(argv, argv + argc);
    ArrayRef<const char*>         Argv = makeArrayRef(Args).slice(1);

    std::unique_ptr<CompilerInstance> Clang(new CompilerInstance());
    IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());

    // Register the support for object-file-wrapped Clang modules.
    auto PCHOps = Clang->getPCHContainerOperations();
    PCHOps->registerWriter(std::make_unique<ObjectFilePCHContainerWriter>());
    PCHOps->registerReader(std::make_unique<ObjectFilePCHContainerReader>());

    // Initialize targets first, so that --version shows registered targets.
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();

    // Buffer diagnostics from argument parsing so that we can output them using a
    // well formed diagnostic object.
    IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = new DiagnosticOptions();
    TextDiagnosticBuffer*                 DiagsBuffer = new TextDiagnosticBuffer;
    DiagnosticsEngine                     Diags(DiagID, &*DiagOpts, DiagsBuffer);

    // Setup round-trip remarks for the DiagnosticsEngine used in CreateFromArgs.
    if (find(Argv, StringRef("-Rround-trip-cc1-args")) != Argv.end())
        Diags.setSeverity(diag::remark_cc1_round_trip_generated, diag::Severity::Remark, {});

    bool Success = CompilerInvocation::CreateFromArgs(Clang->getInvocation(), Argv, Diags, Argv0);

    if (Clang->getFrontendOpts().TimeTrace || !Clang->getFrontendOpts().TimeTracePath.empty()) {
        Clang->getFrontendOpts().TimeTrace = 1;
        llvm::timeTraceProfilerInitialize(
            Clang->getFrontendOpts().TimeTraceGranularity,
            Argv0
        );
    }
    // --print-supported-cpus takes priority over the actual compilation.
    if (Clang->getFrontendOpts().PrintSupportedCPUs)
        return PrintSupportedCPUs(Clang->getTargetOpts().Triple);

    // Infer the builtin include path if unspecified.
    if (Clang->getHeaderSearchOpts().UseBuiltinIncludes && Clang->getHeaderSearchOpts().ResourceDir.empty()) {
        Clang->getHeaderSearchOpts().ResourceDir = CompilerInvocation::GetResourcesPath(Argv0, 0);
    }

    // Create the actual diagnostics engine.
    Clang->createDiagnostics();
    if (!Clang->hasDiagnostics())
        return 1;

    // Set an error handler, so that any LLVM backend diagnostics go through our
    // error handler.
    llvm::install_fatal_error_handler(LLVMErrorHandler, static_cast<void*>(&Clang->getDiagnostics()));

    DiagsBuffer->FlushDiagnostics(Clang->getDiagnostics());
    if (!Success) {
        Clang->getDiagnosticClient().finish();
        return 1;
    }

    // Execute the frontend actions.
    {
        llvm::TimeTraceScope TimeScope("ExecuteCompiler");
        Success = ExecuteCompilerInvocation(Clang.get());
    }

    // If any timers were active but haven't been destroyed yet, print their
    // results now.  This happens in -disable-free mode.
    llvm::TimerGroup::printAll(llvm::errs());
    llvm::TimerGroup::clearAll();

    if (llvm::timeTraceProfilerEnabled()) {
        SmallString<128> Path(Clang->getFrontendOpts().OutputFile);
        llvm::sys::path::replace_extension(Path, "json");
        if (!Clang->getFrontendOpts().TimeTracePath.empty()) {
            // replace the suffix to '.json' directly
            SmallString<128> TracePath(Clang->getFrontendOpts().TimeTracePath);
            if (llvm::sys::fs::is_directory(TracePath))
                llvm::sys::path::append(TracePath, llvm::sys::path::filename(Path));
            Path.assign(TracePath);
        }
        if (auto profilerOutput = Clang->createOutputFile(
                Path.str(),
                /*Binary=*/false,
                /*RemoveFileOnSignal=*/false,
                /*useTemporary=*/false
            )) {
            llvm::timeTraceProfilerWrite(*profilerOutput);
            profilerOutput.reset();
            llvm::timeTraceProfilerCleanup();
            Clang->clearOutputFiles(false);
        }
    }

    // Our error handler depends on the Diagnostics object, which we're
    // potentially about to delete. Uninstall the handler now so that any
    // later errors use the default handling behavior instead.
    llvm::remove_fatal_error_handler();

    // When running with -disable-free, don't do any destruction or shutdown.
    if (Clang->getFrontendOpts().DisableFree) {
        llvm::BuryPointer(std::move(Clang));
        return !Success;
    }

    return !Success;
}
