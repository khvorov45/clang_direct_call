//===-LTOCodeGenerator.cpp - LLVM Link Time Optimizer ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the Link Time Optimization library. This library is
// intended to be used by linker to optimize code at link time.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_LTO_legacy_LTOCodeGenerator.h"

#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_ADT_StringExtras.h"
#include "llvm_include_llvm_Analysis_Passes.h"
#include "llvm_include_llvm_Analysis_TargetLibraryInfo.h"
#include "llvm_include_llvm_Analysis_TargetTransformInfo.h"
#include "llvm_include_llvm_Bitcode_BitcodeWriter.h"
#include "llvm_include_llvm_CodeGen_CommandFlags.h"
#include "llvm_include_llvm_CodeGen_ParallelCG.h"
#include "llvm_include_llvm_CodeGen_TargetSubtargetInfo.h"
#include "llvm_include_llvm_Config_config.h"
#include "llvm_include_llvm_IR_Constants.h"
#include "llvm_include_llvm_IR_DataLayout.h"
#include "llvm_include_llvm_IR_DebugInfo.h"
#include "llvm_include_llvm_IR_DerivedTypes.h"
#include "llvm_include_llvm_IR_DiagnosticInfo.h"
#include "llvm_include_llvm_IR_DiagnosticPrinter.h"
#include "llvm_include_llvm_IR_LLVMContext.h"
#include "llvm_include_llvm_IR_LLVMRemarkStreamer.h"
#include "llvm_include_llvm_IR_LegacyPassManager.h"
#include "llvm_include_llvm_IR_Mangler.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_IR_PassTimingInfo.h"
#include "llvm_include_llvm_IR_Verifier.h"
#include "llvm_include_llvm_LTO_LTO.h"
#include "llvm_include_llvm_LTO_LTOBackend.h"
#include "llvm_include_llvm_LTO_legacy_LTOModule.h"
#include "llvm_include_llvm_LTO_legacy_UpdateCompilerUsed.h"
#include "llvm_include_llvm_Linker_Linker.h"
#include "llvm_include_llvm_MC_MCAsmInfo.h"
#include "llvm_include_llvm_MC_MCContext.h"
#include "llvm_include_llvm_MC_SubtargetFeature.h"
#include "llvm_include_llvm_MC_TargetRegistry.h"
#include "llvm_include_llvm_Remarks_HotnessThresholdParser.h"
#include "llvm_include_llvm_Support_CommandLine.h"
#include "llvm_include_llvm_Support_FileSystem.h"
#include "llvm_include_llvm_Support_Host.h"
#include "llvm_include_llvm_Support_MemoryBuffer.h"
#include "llvm_include_llvm_Support_Process.h"
#include "llvm_include_llvm_Support_Signals.h"
#include "llvm_include_llvm_Support_TargetSelect.h"
#include "llvm_include_llvm_Support_ToolOutputFile.h"
#include "llvm_include_llvm_Support_YAMLTraits.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include "llvm_include_llvm_Target_TargetOptions.h"
#include "llvm_include_llvm_Transforms_IPO.h"
#include "llvm_include_llvm_Transforms_IPO_Internalize.h"
#include "llvm_include_llvm_Transforms_IPO_WholeProgramDevirt.h"
#include "llvm_include_llvm_Transforms_ObjCARC.h"
#include "llvm_include_llvm_Transforms_Utils_ModuleUtils.h"
#include <optional>
#include <system_error>
using namespace llvm;

const char*
LTOCodeGenerator::getVersionString() {
    return PACKAGE_NAME " version " PACKAGE_VERSION;
}

namespace llvm {
cl::opt<bool> LTODiscardValueNames(
    "lto-discard-value-names",
    cl::desc("Strip names from Value during LTO (other than GlobalValue)."),
#ifdef NDEBUG
    cl::init(true),
#else
    cl::init(false),
#endif
    cl::Hidden
);

cl::opt<bool> RemarksWithHotness(
    "lto-pass-remarks-with-hotness",
    cl::desc("With PGO, include profile count in optimization remarks"),
    cl::Hidden
);

cl::opt<std::optional<uint64_t>, false, remarks::HotnessThresholdParser>
    RemarksHotnessThreshold(
        "lto-pass-remarks-hotness-threshold",
        cl::desc("Minimum profile count required for an "
                 "optimization remark to be output."
                 " Use 'auto' to apply the threshold from profile summary."),
        cl::value_desc("uint or 'auto'"),
        cl::init(0),
        cl::Hidden
    );

cl::opt<std::string>
    RemarksFilename("lto-pass-remarks-output", cl::desc("Output filename for pass remarks"), cl::value_desc("filename"));

cl::opt<std::string>
    RemarksPasses("lto-pass-remarks-filter", cl::desc("Only record optimization remarks from passes whose "
                                                      "names match the given regular expression"),
                  cl::value_desc("regex"));

cl::opt<std::string> RemarksFormat(
    "lto-pass-remarks-format",
    cl::desc("The format used for serializing remarks (default: YAML)"),
    cl::value_desc("format"),
    cl::init("yaml")
);

cl::opt<std::string> LTOStatsFile(
    "lto-stats-file",
    cl::desc("Save statistics to the specified file"),
    cl::Hidden
);

cl::opt<std::string> AIXSystemAssemblerPath(
    "lto-aix-system-assembler",
    cl::desc("Path to a system assembler, picked up on AIX only"),
    cl::value_desc("path")
);

cl::opt<bool>
    LTORunCSIRInstr("cs-profile-generate", cl::desc("Perform context sensitive PGO instrumentation"));

cl::opt<std::string>
    LTOCSIRProfile("cs-profile-path", cl::desc("Context sensitive profile file path"));
}  // namespace llvm

LTOCodeGenerator::LTOCodeGenerator(LLVMContext& Context) :
    Context(Context),
    MergedModule(new Module("ld-temp.o", Context)),
    TheLinker(new Linker(*MergedModule)) {
    Context.setDiscardValueNames(LTODiscardValueNames);
    Context.enableDebugTypeODRUniquing();

    Config.CodeModel = std::nullopt;
    Config.StatsFile = LTOStatsFile;
    Config.PreCodeGenPassesHook = [](legacy::PassManager& PM) {
        PM.add(createObjCARCContractPass());
    };

    Config.RunCSIRInstr = LTORunCSIRInstr;
    Config.CSIRProfile = LTOCSIRProfile;
}

LTOCodeGenerator::~LTOCodeGenerator() = default;

void
LTOCodeGenerator::setAsmUndefinedRefs(LTOModule* Mod) {
    for (const StringRef& Undef : Mod->getAsmUndefinedRefs())
        AsmUndefinedRefs.insert(Undef);
}

bool
LTOCodeGenerator::addModule(LTOModule* Mod) {
    assert(&Mod->getModule().getContext() == &Context && "Expected module in same context");

    bool ret = TheLinker->linkInModule(Mod->takeModule());
    setAsmUndefinedRefs(Mod);

    // We've just changed the input, so let's make sure we verify it.
    HasVerifiedInput = false;

    return !ret;
}

void
LTOCodeGenerator::setModule(std::unique_ptr<LTOModule> Mod) {
    assert(&Mod->getModule().getContext() == &Context && "Expected module in same context");

    AsmUndefinedRefs.clear();

    MergedModule = Mod->takeModule();
    TheLinker = std::make_unique<Linker>(*MergedModule);
    setAsmUndefinedRefs(&*Mod);

    // We've just changed the input, so let's make sure we verify it.
    HasVerifiedInput = false;
}

void
LTOCodeGenerator::setTargetOptions(const TargetOptions& Options) {
    Config.Options = Options;
}

void
LTOCodeGenerator::setDebugInfo(lto_debug_model Debug) {
    switch (Debug) {
        case LTO_DEBUG_MODEL_NONE:
            EmitDwarfDebugInfo = false;
            return;

        case LTO_DEBUG_MODEL_DWARF:
            EmitDwarfDebugInfo = true;
            return;
    }
    llvm_unreachable("Unknown debug format!");
}

void
LTOCodeGenerator::setOptLevel(unsigned Level) {
    Config.OptLevel = Level;
    Config.PTO.LoopVectorization = Config.OptLevel > 1;
    Config.PTO.SLPVectorization = Config.OptLevel > 1;
    switch (Config.OptLevel) {
        case 0:
            Config.CGOptLevel = CodeGenOpt::None;
            return;
        case 1:
            Config.CGOptLevel = CodeGenOpt::Less;
            return;
        case 2:
            Config.CGOptLevel = CodeGenOpt::Default;
            return;
        case 3:
            Config.CGOptLevel = CodeGenOpt::Aggressive;
            return;
    }
    llvm_unreachable("Unknown optimization level!");
}

bool
LTOCodeGenerator::writeMergedModules(StringRef Path) {
    if (!determineTarget())
        return false;

    // We always run the verifier once on the merged module.
    verifyMergedModuleOnce();

    // mark which symbols can not be internalized
    applyScopeRestrictions();

    // create output file
    std::error_code EC;
    ToolOutputFile  Out(Path, EC, sys::fs::OF_None);
    if (EC) {
        std::string ErrMsg = "could not open bitcode file for writing: ";
        ErrMsg += Path.str() + ": " + EC.message();
        emitError(ErrMsg);
        return false;
    }

    // write bitcode to it
    WriteBitcodeToFile(*MergedModule, Out.os(), ShouldEmbedUselists);
    Out.os().close();

    if (Out.os().has_error()) {
        std::string ErrMsg = "could not write bitcode file: ";
        ErrMsg += Path.str() + ": " + Out.os().error().message();
        emitError(ErrMsg);
        Out.os().clear_error();
        return false;
    }

    Out.keep();
    return true;
}

bool
LTOCodeGenerator::useAIXSystemAssembler() {
    const auto& Triple = TargetMach->getTargetTriple();
    return Triple.isOSAIX();
}

bool
LTOCodeGenerator::runAIXSystemAssembler(SmallString<128>& AssemblyFile) {
    assert(useAIXSystemAssembler() && "Runing AIX system assembler when integrated assembler is available!");

    // Set the system assembler path.
    SmallString<256> AssemblerPath("/usr/bin/as");
    if (!llvm::AIXSystemAssemblerPath.empty()) {
        if (llvm::sys::fs::real_path(llvm::AIXSystemAssemblerPath, AssemblerPath,
                                     /* expand_tilde */ true)) {
            emitError(
                "Cannot find the assembler specified by lto-aix-system-assembler"
            );
            return false;
        }
    }

    // Setup the LDR_CNTRL variable
    std::string LDR_CNTRL_var = "LDR_CNTRL=MAXDATA32=0xA0000000@DSA";
    if (std::optional<std::string> V = sys::Process::GetEnv("LDR_CNTRL"))
        LDR_CNTRL_var += ("@" + *V);

    // Prepare inputs for the assember.
    const auto& Triple = TargetMach->getTargetTriple();
    const char* Arch = Triple.isArch64Bit() ? "-a64" : "-a32";
    std::string ObjectFileName(AssemblyFile);
    ObjectFileName[ObjectFileName.size() - 1] = 'o';
    SmallVector<StringRef, 8> Args = {
        "/bin/env",
        LDR_CNTRL_var,
        AssemblerPath,
        Arch,
        "-many",
        "-o",
        ObjectFileName,
        AssemblyFile};

    // Invoke the assembler.
    int RC = sys::ExecuteAndWait(Args[0], Args);

    // Handle errors.
    if (RC < -1) {
        emitError("LTO assembler exited abnormally");
        return false;
    }
    if (RC < 0) {
        emitError("Unable to invoke LTO assembler");
        return false;
    }
    if (RC > 0) {
        emitError("LTO assembler invocation returned non-zero");
        return false;
    }

    // Cleanup.
    remove(AssemblyFile.c_str());

    // Fix the output file name.
    AssemblyFile = ObjectFileName;

    return true;
}

bool
LTOCodeGenerator::compileOptimizedToFile(const char** Name) {
    if (useAIXSystemAssembler())
        setFileType(CGFT_AssemblyFile);

    // make unique temp output file to put generated code
    SmallString<128> Filename;

    auto AddStream =
        [&](size_t       Task,
            const Twine& ModuleName) -> std::unique_ptr<CachedFileStream> {
        StringRef Extension(Config.CGFileType == CGFT_AssemblyFile ? "s" : "o");

        int             FD;
        std::error_code EC =
            sys::fs::createTemporaryFile("lto-llvm", Extension, FD, Filename);
        if (EC)
            emitError(EC.message());

        return std::make_unique<CachedFileStream>(
            std::make_unique<llvm::raw_fd_ostream>(FD, true)
        );
    };

    bool genResult = compileOptimized(AddStream, 1);

    if (!genResult) {
        sys::fs::remove(Twine(Filename));
        return false;
    }

    // If statistics were requested, save them to the specified file or
    // print them out after codegen.
    if (StatsFile)
        PrintStatisticsJSON(StatsFile->os());
    else if (AreStatisticsEnabled())
        PrintStatistics();

    if (useAIXSystemAssembler())
        if (!runAIXSystemAssembler(Filename))
            return false;

    NativeObjectPath = Filename.c_str();
    *Name = NativeObjectPath.c_str();
    return true;
}

std::unique_ptr<MemoryBuffer>
LTOCodeGenerator::compileOptimized() {
    const char* name;
    if (!compileOptimizedToFile(&name))
        return nullptr;

    // read .o file into memory buffer
    ErrorOr<std::unique_ptr<MemoryBuffer>> BufferOrErr = MemoryBuffer::getFile(
        name,
        /*IsText=*/false,
        /*RequiresNullTerminator=*/false
    );
    if (std::error_code EC = BufferOrErr.getError()) {
        emitError(EC.message());
        sys::fs::remove(NativeObjectPath);
        return nullptr;
    }

    // remove temp files
    sys::fs::remove(NativeObjectPath);

    return std::move(*BufferOrErr);
}

bool
LTOCodeGenerator::compile_to_file(const char** Name) {
    if (!optimize())
        return false;

    return compileOptimizedToFile(Name);
}

std::unique_ptr<MemoryBuffer>
LTOCodeGenerator::compile() {
    if (!optimize())
        return nullptr;

    return compileOptimized();
}

bool
LTOCodeGenerator::determineTarget() {
    if (TargetMach)
        return true;

    TripleStr = MergedModule->getTargetTriple();
    if (TripleStr.empty()) {
        TripleStr = sys::getDefaultTargetTriple();
        MergedModule->setTargetTriple(TripleStr);
    }
    llvm::Triple Triple(TripleStr);

    // create target machine from info for merged modules
    std::string ErrMsg;
    MArch = LLVMTargetRegistryTheTarget;
    if (!MArch) {
        emitError(ErrMsg);
        return false;
    }

    // Construct LTOModule, hand over ownership of module and target. Use MAttr as
    // the default set of features.
    SubtargetFeatures Features(join(Config.MAttrs, ""));
    Features.getDefaultSubtargetFeatures(Triple);
    FeatureStr = Features.getString();
    // Set a default CPU for Darwin triples.
    if (Config.CPU.empty() && Triple.isOSDarwin()) {
        if (Triple.getArch() == llvm::Triple::x86_64)
            Config.CPU = "core2";
        else if (Triple.getArch() == llvm::Triple::x86)
            Config.CPU = "yonah";
        else if (Triple.isArm64e())
            Config.CPU = "apple-a12";
        else if (Triple.getArch() == llvm::Triple::aarch64 || Triple.getArch() == llvm::Triple::aarch64_32)
            Config.CPU = "cyclone";
    }

    // If data-sections is not explicitly set or unset, set data-sections by
    // default to match the behaviour of lld and gold plugin.
    if (!codegen::getExplicitDataSections())
        Config.Options.DataSections = true;

    TargetMach = createTargetMachine();
    assert(TargetMach && "Unable to create target machine");

    return true;
}

std::unique_ptr<TargetMachine>
LTOCodeGenerator::createTargetMachine() {
    assert(MArch && "MArch is not set!");
    TargetMachine* targetMachine = 0;
    if (MArch->TargetMachineCtorFn) {
        targetMachine = MArch->TargetMachineCtorFn(*MArch, llvm::Triple(TripleStr), Config.CPU, FeatureStr, Config.Options, Config.RelocModel, std::nullopt, Config.CGOptLevel, false);
    }
    return std::unique_ptr<TargetMachine>(targetMachine);
}

// If a linkonce global is present in the MustPreserveSymbols, we need to make
// sure we honor this. To force the compiler to not drop it, we add it to the
// "llvm.compiler.used" global.
void
LTOCodeGenerator::preserveDiscardableGVs(
    Module&                                      TheModule,
    llvm::function_ref<bool(const GlobalValue&)> mustPreserveGV
) {
    std::vector<GlobalValue*> Used;
    auto                      mayPreserveGlobal = [&](GlobalValue& GV) {
        if (!GV.isDiscardableIfUnused() || GV.isDeclaration() || !mustPreserveGV(GV))
            return;
        if (GV.hasAvailableExternallyLinkage())
            return emitWarning(
                (Twine("Linker asked to preserve available_externally global: '") + GV.getName() + "'").str()
            );
        if (GV.hasInternalLinkage())
            return emitWarning((Twine("Linker asked to preserve internal global: '") + GV.getName() + "'").str());
        Used.push_back(&GV);
    };
    for (auto& GV : TheModule)
        mayPreserveGlobal(GV);
    for (auto& GV : TheModule.globals())
        mayPreserveGlobal(GV);
    for (auto& GV : TheModule.aliases())
        mayPreserveGlobal(GV);

    if (Used.empty())
        return;

    appendToCompilerUsed(TheModule, Used);
}

void
LTOCodeGenerator::applyScopeRestrictions() {
    if (ScopeRestrictionsDone)
        return;

    // Declare a callback for the internalize pass that will ask for every
    // candidate GlobalValue if it can be internalized or not.
    Mangler         Mang;
    SmallString<64> MangledName;
    auto            mustPreserveGV = [&](const GlobalValue& GV) -> bool {
        // Unnamed globals can't be mangled, but they can't be preserved either.
        if (!GV.hasName())
            return false;

        // Need to mangle the GV as the "MustPreserveSymbols" StringSet is filled
        // with the linker supplied name, which on Darwin includes a leading
        // underscore.
        MangledName.clear();
        MangledName.reserve(GV.getName().size() + 1);
        Mang.getNameWithPrefix(MangledName, &GV, /*CannotUsePrivateLabel=*/false);
        return MustPreserveSymbols.count(MangledName);
    };

    // Preserve linkonce value on linker request
    preserveDiscardableGVs(*MergedModule, mustPreserveGV);

    if (!ShouldInternalize)
        return;

    if (ShouldRestoreGlobalsLinkage) {
        // Record the linkage type of non-local symbols so they can be restored
        // prior
        // to module splitting.
        auto RecordLinkage = [&](const GlobalValue& GV) {
            if (!GV.hasAvailableExternallyLinkage() && !GV.hasLocalLinkage() && GV.hasName())
                ExternalSymbols.insert(std::make_pair(GV.getName(), GV.getLinkage()));
        };
        for (auto& GV : *MergedModule)
            RecordLinkage(GV);
        for (auto& GV : MergedModule->globals())
            RecordLinkage(GV);
        for (auto& GV : MergedModule->aliases())
            RecordLinkage(GV);
    }

    // Update the llvm.compiler_used globals to force preserving libcalls and
    // symbols referenced from asm
    updateCompilerUsed(*MergedModule, *TargetMach, AsmUndefinedRefs);

    internalizeModule(*MergedModule, mustPreserveGV);

    ScopeRestrictionsDone = true;
}

/// Restore original linkage for symbols that may have been internalized
void
LTOCodeGenerator::restoreLinkageForExternals() {
    if (!ShouldInternalize || !ShouldRestoreGlobalsLinkage)
        return;

    assert(ScopeRestrictionsDone && "Cannot externalize without internalization!");

    if (ExternalSymbols.empty())
        return;

    auto externalize = [this](GlobalValue& GV) {
        if (!GV.hasLocalLinkage() || !GV.hasName())
            return;

        auto I = ExternalSymbols.find(GV.getName());
        if (I == ExternalSymbols.end())
            return;

        GV.setLinkage(I->second);
    };

    llvm::for_each(MergedModule->functions(), externalize);
    llvm::for_each(MergedModule->globals(), externalize);
    llvm::for_each(MergedModule->aliases(), externalize);
}

void
LTOCodeGenerator::verifyMergedModuleOnce() {
    // Only run on the first call.
    if (HasVerifiedInput)
        return;
    HasVerifiedInput = true;

    bool BrokenDebugInfo = false;
    if (verifyModule(*MergedModule, &dbgs(), &BrokenDebugInfo))
        report_fatal_error("Broken module found, compilation aborted!");
    if (BrokenDebugInfo) {
        emitWarning("Invalid debug info found, debug info will be stripped");
        StripDebugInfo(*MergedModule);
    }
}

void
LTOCodeGenerator::finishOptimizationRemarks() {
    if (DiagnosticOutputFile) {
        DiagnosticOutputFile->keep();
        // FIXME: LTOCodeGenerator dtor is not invoked on Darwin
        DiagnosticOutputFile->os().flush();
    }
}

/// Optimize merged modules using various IPO passes
bool
LTOCodeGenerator::optimize() {
    if (!this->determineTarget())
        return false;

    auto DiagFileOrErr = lto::setupLLVMOptimizationRemarks(
        Context,
        RemarksFilename,
        RemarksPasses,
        RemarksFormat,
        RemarksWithHotness,
        RemarksHotnessThreshold
    );
    if (!DiagFileOrErr) {
        errs() << "Error: " << toString(DiagFileOrErr.takeError()) << "\n";
        report_fatal_error("Can't get an output file for the remarks");
    }
    DiagnosticOutputFile = std::move(*DiagFileOrErr);

    // Setup output file to emit statistics.
    auto StatsFileOrErr = lto::setupStatsFile(LTOStatsFile);
    if (!StatsFileOrErr) {
        errs() << "Error: " << toString(StatsFileOrErr.takeError()) << "\n";
        report_fatal_error("Can't get an output file for the statistics");
    }
    StatsFile = std::move(StatsFileOrErr.get());

    // Currently there is no support for enabling whole program visibility via a
    // linker option in the old LTO API, but this call allows it to be specified
    // via the internal option. Must be done before WPD invoked via the optimizer
    // pipeline run below.
    updatePublicTypeTestCalls(*MergedModule,
                              /* WholeProgramVisibilityEnabledInLTO */ false);
    updateVCallVisibilityInModule(*MergedModule,
                                  /* WholeProgramVisibilityEnabledInLTO */ false,
                                  // FIXME: This needs linker information via a
                                  // TBD new interface.
                                  /* DynamicExportSymbols */ {});

    // We always run the verifier once on the merged module, the `DisableVerify`
    // parameter only applies to subsequent verify.
    verifyMergedModuleOnce();

    // Mark which symbols can not be internalized
    this->applyScopeRestrictions();

    // Write LTOPostLink flag for passes that require all the modules.
    MergedModule->addModuleFlag(Module::Error, "LTOPostLink", 1);

    // Add an appropriate DataLayout instance for this module...
    MergedModule->setDataLayout(TargetMach->createDataLayout());

    if (!SaveIRBeforeOptPath.empty()) {
        std::error_code EC;
        raw_fd_ostream  OS(SaveIRBeforeOptPath, EC, sys::fs::OF_None);
        if (EC)
            report_fatal_error(Twine("Failed to open ") + SaveIRBeforeOptPath + " to save optimized bitcode\n");
        WriteBitcodeToFile(*MergedModule, OS,
                           /* ShouldPreserveUseListOrder */ true);
    }

    ModuleSummaryIndex CombinedIndex(false);
    TargetMach = createTargetMachine();
    if (!opt(Config, TargetMach.get(), 0, *MergedModule, /*IsThinLTO=*/false,
             /*ExportSummary=*/&CombinedIndex,
             /*ImportSummary=*/nullptr,
             /*CmdArgs*/ std::vector<uint8_t>())) {
        emitError("LTO middle-end optimizations failed");
        return false;
    }

    return true;
}

bool
LTOCodeGenerator::compileOptimized(AddStreamFn AddStream, unsigned ParallelismLevel) {
    if (!this->determineTarget())
        return false;

    // We always run the verifier once on the merged module.  If it has already
    // been called in optimize(), this call will return early.
    verifyMergedModuleOnce();

    // Re-externalize globals that may have been internalized to increase scope
    // for splitting
    restoreLinkageForExternals();

    ModuleSummaryIndex CombinedIndex(false);

    Config.CodeGenOnly = true;
    Error Err = backend(Config, AddStream, ParallelismLevel, *MergedModule, CombinedIndex);
    assert(!Err && "unexpected code-generation failure");
    (void)Err;

    // If statistics were requested, save them to the specified file or
    // print them out after codegen.
    if (StatsFile)
        PrintStatisticsJSON(StatsFile->os());
    else if (AreStatisticsEnabled())
        PrintStatistics();

    reportAndResetTimings();

    finishOptimizationRemarks();

    return true;
}

void
LTOCodeGenerator::setCodeGenDebugOptions(ArrayRef<StringRef> Options) {
    for (StringRef Option : Options)
        CodegenOptions.push_back(Option.str());
}

void
LTOCodeGenerator::parseCodeGenDebugOptions() {
    if (!CodegenOptions.empty())
        llvm::parseCommandLineOptions(CodegenOptions);
}

void
llvm::parseCommandLineOptions(std::vector<std::string>& Options) {
    if (!Options.empty()) {
        // ParseCommandLineOptions() expects argv[0] to be program name.
        std::vector<const char*> CodegenArgv(1, "libLLVMLTO");
        for (std::string& Arg : Options)
            CodegenArgv.push_back(Arg.c_str());
        cl::ParseCommandLineOptions(CodegenArgv.size(), CodegenArgv.data());
    }
}

void
LTOCodeGenerator::DiagnosticHandler(const DiagnosticInfo& DI) {
    // Map the LLVM internal diagnostic severity to the LTO diagnostic severity.
    lto_codegen_diagnostic_severity_t Severity;
    switch (DI.getSeverity()) {
        case DS_Error:
            Severity = LTO_DS_ERROR;
            break;
        case DS_Warning:
            Severity = LTO_DS_WARNING;
            break;
        case DS_Remark:
            Severity = LTO_DS_REMARK;
            break;
        case DS_Note:
            Severity = LTO_DS_NOTE;
            break;
    }
    // Create the string that will be reported to the external diagnostic handler.
    std::string                 MsgStorage;
    raw_string_ostream          Stream(MsgStorage);
    DiagnosticPrinterRawOStream DP(Stream);
    DI.print(DP);
    Stream.flush();

    // If this method has been called it means someone has set up an external
    // diagnostic handler. Assert on that.
    assert(DiagHandler && "Invalid diagnostic handler");
    (*DiagHandler)(Severity, MsgStorage.c_str(), DiagContext);
}

namespace {
struct LTODiagnosticHandler: public DiagnosticHandler {
    LTOCodeGenerator* CodeGenerator;
    LTODiagnosticHandler(LTOCodeGenerator* CodeGenPtr) :
        CodeGenerator(CodeGenPtr) {}
    bool
    handleDiagnostics(const DiagnosticInfo& DI) override {
        CodeGenerator->DiagnosticHandler(DI);
        return true;
    }
};
}  // namespace

void
LTOCodeGenerator::setDiagnosticHandler(lto_diagnostic_handler_t DiagHandler, void* Ctxt) {
    this->DiagHandler = DiagHandler;
    this->DiagContext = Ctxt;
    if (!DiagHandler)
        return Context.setDiagnosticHandler(nullptr);
    // Register the LTOCodeGenerator stub in the LLVMContext to forward the
    // diagnostic to the external DiagHandler.
    Context.setDiagnosticHandler(std::make_unique<LTODiagnosticHandler>(this), true);
}

namespace {
class LTODiagnosticInfo: public DiagnosticInfo {
    const Twine& Msg;

  public:
    LTODiagnosticInfo(const Twine& DiagMsg, DiagnosticSeverity Severity = DS_Error) :
        DiagnosticInfo(DK_Linker, Severity),
        Msg(DiagMsg) {}
    void
    print(DiagnosticPrinter& DP) const override {
        DP << Msg;
    }
};
}  // namespace

void
LTOCodeGenerator::emitError(const std::string& ErrMsg) {
    if (DiagHandler)
        (*DiagHandler)(LTO_DS_ERROR, ErrMsg.c_str(), DiagContext);
    else
        Context.diagnose(LTODiagnosticInfo(ErrMsg));
}

void
LTOCodeGenerator::emitWarning(const std::string& ErrMsg) {
    if (DiagHandler)
        (*DiagHandler)(LTO_DS_WARNING, ErrMsg.c_str(), DiagContext);
    else
        Context.diagnose(LTODiagnosticInfo(ErrMsg, DS_Warning));
}
