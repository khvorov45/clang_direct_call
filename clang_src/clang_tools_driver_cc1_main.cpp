#include "llvm_lib_Target_X86_TargetInfo_X86TargetInfo.h"
#include "llvm_lib_Target_X86_X86TargetMachine.h"
#include "llvm_lib_Target_X86_X86.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Support_TargetSelect.h"
#include "clang_include_clang_CodeGen_ObjectFilePCHContainerOperations.h"
#include "clang_include_clang_Frontend_CompilerInstance.h"
#include "clang_include_clang_Frontend_TextDiagnosticBuffer.h"
#include "clang_include_clang_FrontendTool_Utils.h"

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

static llvm::TargetMachine*
LLVMX86TargetMachineProc(const llvm::Target& T, const llvm::Triple& TT, llvm::StringRef CPU, llvm::StringRef FS, const llvm::TargetOptions& Options, std::optional<llvm::Reloc::Model> RM, std::optional<llvm::CodeModel::Model> CM, llvm::CodeGenOpt::Level OL, bool JIT) {
    return new llvm::X86TargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT);
}

extern "C" int
cc1_main(int argc, char** argv) {
    // NOTE(khvorov) Init
    {
        llvm::Target& x8664Target = llvm::getTheX86_64Target();
        x8664Target.Name = "x86-64";
        x8664Target.ShortDesc = "64-bit X86: EM64T and AMD64";
        x8664Target.BackendName = "X86";
        x8664Target.HasJIT = true;

        llvm::TargetRegistry::setTarget(x8664Target);
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

        LLVMInitializeX86TargetMC();
        LLVMInitializeX86AsmPrinter();
        LLVMInitializeX86AsmParser();
    }

    std::unique_ptr<clang::CompilerInstance>        Clang(new clang::CompilerInstance());
    clang::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagID(new clang::DiagnosticIDs());

    // Register the support for object-file-wrapped Clang modules.
    auto PCHOps = Clang->getPCHContainerOperations();
    PCHOps->registerWriter(std::make_unique<clang::ObjectFilePCHContainerWriter>());
    PCHOps->registerReader(std::make_unique<clang::ObjectFilePCHContainerReader>());

    // Buffer diagnostics from argument parsing so that we can output them using a
    // well formed diagnostic object.
    clang::IntrusiveRefCntPtr<clang::DiagnosticOptions> DiagOpts = new clang::DiagnosticOptions();
    clang::TextDiagnosticBuffer*                        DiagsBuffer = new clang::TextDiagnosticBuffer;
    clang::DiagnosticsEngine                            Diags(DiagID, &*DiagOpts, DiagsBuffer);

    bool Success = clang::CompilerInvocation::CreateFromArgs(Clang->getInvocation(), Diags, argc, argv);

    Clang->createDiagnostics();
    Success = Clang->hasDiagnostics();

    if (Success) {
        DiagsBuffer->FlushDiagnostics(Clang->getDiagnostics());
        Success = ExecuteCompilerInvocation(Clang.get());
    } else {
        Clang->getDiagnosticClient().finish();
    }

    int result = !Success;
    return result;
}
