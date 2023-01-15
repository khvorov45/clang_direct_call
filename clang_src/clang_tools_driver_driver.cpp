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

extern int cc1_main(ArrayRef<const char*> Argv, const char* Argv0, void* MainAddr);
extern int cc1as_main(ArrayRef<const char*> Argv, const char* Argv0, void* MainAddr);
extern int cc1gen_reproducer_main(ArrayRef<const char*> Argv, const char* Argv0, void* MainAddr);

static int
ExecuteCC1Tool(SmallVectorImpl<const char*>& ArgV) {
    // If we call the cc1 tool from the clangDriver library (through
    // Driver::CC1Main), we need to clean up the options usage count. The options
    // are currently global, and they might have been used previously by the
    // driver.
    llvm::cl::ResetAllOptionOccurrences();

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

    SmallVector<const char*, 256> Args(argv, argv + argc);

    // NOTE(khvorov) Only support generating objs via the cc1 tool
    mdc_assert(argc >= 2);
    mdc_Str firstArg = mdc_STR(argv[1]);
    mdc_assert(mdc_strStartsWith(firstArg, mdc_STR("-cc1")));
    int result = ExecuteCC1Tool(Args);
    return result;
}
