//===-- LLVMTargetMachine.cpp - Implement the LLVMTargetMachine class -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the LLVMTargetMachine class.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Analysis_Passes.h"
#include "llvm_include_llvm_CodeGen_AsmPrinter.h"
#include "llvm_include_llvm_CodeGen_BasicTTIImpl.h"
#include "llvm_include_llvm_CodeGen_MachineModuleInfo.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_CodeGen_TargetPassConfig.h"
#include "llvm_include_llvm_IR_LegacyPassManager.h"
#include "llvm_include_llvm_MC_MCAsmBackend.h"
#include "llvm_include_llvm_MC_MCAsmInfo.h"
#include "llvm_include_llvm_MC_MCCodeEmitter.h"
#include "llvm_include_llvm_MC_MCContext.h"
#include "llvm_include_llvm_MC_MCInstrInfo.h"
#include "llvm_include_llvm_MC_MCObjectWriter.h"
#include "llvm_include_llvm_MC_MCRegisterInfo.h"
#include "llvm_include_llvm_MC_MCStreamer.h"
#include "llvm_include_llvm_MC_MCSubtargetInfo.h"
#include "llvm_include_llvm_MC_TargetRegistry.h"
#include "llvm_include_llvm_Support_CommandLine.h"
#include "llvm_include_llvm_Support_FormattedStream.h"
#include "llvm_include_llvm_Target_TargetMachine.h"
#include "llvm_include_llvm_Target_TargetOptions.h"
using namespace llvm;

static cl::opt<bool>
    EnableTrapUnreachable("trap-unreachable", cl::Hidden, cl::desc("Enable generating trap for unreachable"));

void
LLVMTargetMachine::initAsmInfo() {
    assert(TheTarget.MCRegInfoCtorFn);
    MRI.reset(TheTarget.MCRegInfoCtorFn(getTargetTriple()));
    assert(TheTarget.MCInstrInfoCtorFn);
    MII.reset(TheTarget.MCInstrInfoCtorFn());
    // FIXME: Having an MCSubtargetInfo on the target machine is a hack due
    // to some backends having subtarget feature dependent module level
    // code generation. This is similar to the hack in the AsmPrinter for
    // module level assembly etc.
    assert(TheTarget.MCSubtargetInfoCtorFn);
    STI.reset(TheTarget.MCSubtargetInfoCtorFn(getTargetTriple(), getTargetCPU(), getTargetFeatureString()));

    assert(TheTarget.MCAsmInfoCtorFn);
    MCAsmInfo* TmpAsmInfo = TheTarget.MCAsmInfoCtorFn(*MRI, getTargetTriple(), Options.MCOptions);

    // TargetSelect.h moved to a different directory between LLVM 2.9 and 3.0,
    // and if the old one gets included then MCAsmInfo will be NULL and
    // we'll crash later.
    // Provide the user with a useful error message about what's wrong.
    assert(TmpAsmInfo &&
           "MCAsmInfo not initialized. "
           "Make sure you include the correct TargetSelect.h"
           "and that InitializeAllTargetMCs() is being invoked!");

    if (Options.BinutilsVersion.first > 0)
        TmpAsmInfo->setBinutilsVersion(Options.BinutilsVersion);

    if (Options.DisableIntegratedAS) {
        TmpAsmInfo->setUseIntegratedAssembler(false);
        // If there is explict option disable integratedAS, we can't use it for
        // inlineasm either.
        TmpAsmInfo->setParseInlineAsmUsingAsmParser(false);
    }

    TmpAsmInfo->setPreserveAsmComments(Options.MCOptions.PreserveAsmComments);

    TmpAsmInfo->setCompressDebugSections(Options.CompressDebugSections);

    TmpAsmInfo->setRelaxELFRelocations(Options.RelaxELFRelocations);

    if (Options.ExceptionModel != ExceptionHandling::None)
        TmpAsmInfo->setExceptionsType(Options.ExceptionModel);

    AsmInfo.reset(TmpAsmInfo);
}

LLVMTargetMachine::LLVMTargetMachine(const LLVMTarget& T, StringRef DataLayoutString, const Triple& TT, StringRef CPU, StringRef FS, const TargetOptions& Options, Reloc::Model RM, CodeModel::Model CM, CodeGenOpt::Level OL) :
    TargetMachine(T, DataLayoutString, TT, CPU, FS, Options) {
    this->RM = RM;
    this->CMModel = CM;
    this->OptLevel = OL;

    if (EnableTrapUnreachable)
        this->Options.TrapUnreachable = true;
}

TargetTransformInfo
LLVMTargetMachine::getTargetTransformInfo(const Function& F) const {
    return TargetTransformInfo(BasicTTIImpl(this, F));
}

/// addPassesToX helper drives creation and initialization of TargetPassConfig.
static TargetPassConfig*
addPassesToGenerateCode(LLVMTargetMachine& TM, PassManagerBase& PM, bool DisableVerify, MachineModuleInfoWrapperPass& MMIWP) {
    // Targets may override createPassConfig to provide a target-specific
    // subclass.
    TargetPassConfig* PassConfig = TM.createPassConfig(PM);
    // Set PassConfig options provided by TargetMachine.
    PassConfig->setDisableVerify(DisableVerify);
    PM.add(PassConfig);
    PM.add(&MMIWP);

    if (PassConfig->addISelPasses())
        return nullptr;
    PassConfig->addMachinePasses();
    PassConfig->setInitialized();
    return PassConfig;
}

bool
LLVMTargetMachine::addAsmPrinter(PassManagerBase& PM, raw_pwrite_stream& Out, raw_pwrite_stream* DwoOut, CodeGenFileType FileType, MCContext& Context) {
    Expected<std::unique_ptr<MCStreamer>> MCStreamerOrErr =
        createMCStreamer(Out, DwoOut, FileType, Context);
    if (auto Err = MCStreamerOrErr.takeError())
        return true;

    // Create the AsmPrinter, which takes ownership of AsmStreamer if successful.
    if (getTarget().AsmPrinterCtorFn == 0) {
        return true;
    }
    FunctionPass* Printer = getTarget().AsmPrinterCtorFn(*this, std::move(*MCStreamerOrErr));

    PM.add(Printer);
    return false;
}

static llvm::MCStreamer*
LLVMTargetCreateMCObjectStreamer(
    const LLVMTarget*                     Target,
    const llvm::Triple&                     T,
    llvm::MCContext&                        Ctx,
    std::unique_ptr<llvm::MCAsmBackend>&&   TAB,
    std::unique_ptr<llvm::MCObjectWriter>&& OW,
    std::unique_ptr<llvm::MCCodeEmitter>&&  Emitter,
    const llvm::MCSubtargetInfo&            STI,
    bool                                    RelaxAll,
    bool                                    IncrementalLinkerCompatible,
    bool                                    DWARFMustBeAtTheEnd
) {
    llvm::MCStreamer* S = nullptr;

    switch (T.getObjectFormat()) {
        case llvm::Triple::UnknownObjectFormat:
            llvm_unreachable("Unknown object format");

        case llvm::Triple::COFF:
            assert(T.isOSWindows() && "only Windows COFF is supported");
            S = Target->COFFStreamerCtorFn(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll, IncrementalLinkerCompatible);
            break;

        case llvm::Triple::MachO:
            if (Target->MachOStreamerCtorFn)
                S = Target->MachOStreamerCtorFn(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll, DWARFMustBeAtTheEnd);
            else
                S = llvm::createMachOStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll, DWARFMustBeAtTheEnd);
            break;

        case llvm::Triple::ELF:
            if (Target->ELFStreamerCtorFn)
                S = Target->ELFStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            else
                S = llvm::createELFStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            break;

        case llvm::Triple::Wasm:
            if (Target->WasmStreamerCtorFn)
                S = Target->WasmStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            else
                S = llvm::createWasmStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            break;

        case llvm::Triple::GOFF:
            llvm::report_fatal_error("GOFF MCObjectStreamer not implemented yet");

        case llvm::Triple::XCOFF:
            if (Target->XCOFFStreamerCtorFn)
                S = Target->XCOFFStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            else
                S = llvm::createXCOFFStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            break;

        case llvm::Triple::SPIRV:
            if (Target->SPIRVStreamerCtorFn)
                S = Target->SPIRVStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            else
                S = llvm::createSPIRVStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            break;

        case llvm::Triple::DXContainer:
            if (Target->DXContainerStreamerCtorFn)
                S = Target->DXContainerStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            else
                S = llvm::createDXContainerStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
            break;
    }

    if (Target->ObjectTargetStreamerCtorFn) {
        Target->ObjectTargetStreamerCtorFn(*S, STI);
    }

    return S;
}

Expected<std::unique_ptr<MCStreamer>>
LLVMTargetMachine::createMCStreamer(
    raw_pwrite_stream& Out,
    raw_pwrite_stream* DwoOut,
    CodeGenFileType    FileType,
    MCContext&         Context
) {
    if (Options.MCOptions.MCSaveTempLabels)
        Context.setAllowTemporaryLabels(false);

    const MCSubtargetInfo& STI = *getMCSubtargetInfo();
    const MCAsmInfo&       MAI = *getMCAsmInfo();
    const MCRegisterInfo&  MRI = *getMCRegisterInfo();
    const MCInstrInfo&     MII = *getMCInstrInfo();

    std::unique_ptr<MCStreamer> AsmStreamer;

    switch (FileType) {
        case CGFT_AssemblyFile: {
            MCInstPrinter* InstPrinter = 0;
            if (getTarget().MCInstPrinterCtorFn) {
                InstPrinter = getTarget().MCInstPrinterCtorFn(getTargetTriple(), MAI.getAssemblerDialect(), MAI, MII, MRI);
            }

            // Create a code emitter if asked to show the encoding.
            std::unique_ptr<MCCodeEmitter> MCE;
            if (Options.MCOptions.ShowMCEncoding) {
                MCCodeEmitter* emitter = 0;
                if (getTarget().MCCodeEmitterCtorFn) {
                    emitter = getTarget().MCCodeEmitterCtorFn(MII, Context);
                }
                MCE.reset(emitter);
            }

            bool UseDwarfDirectory = false;
            switch (Options.MCOptions.MCUseDwarfDirectory) {
                case MCTargetOptions::DisableDwarfDirectory:
                    UseDwarfDirectory = false;
                    break;
                case MCTargetOptions::EnableDwarfDirectory:
                    UseDwarfDirectory = true;
                    break;
                case MCTargetOptions::DefaultDwarfDirectory:
                    UseDwarfDirectory = MAI.enableDwarfFileDirectoryDefault();
                    break;
            }

            MCAsmBackend* targetMCAsmBackend = 0;
            if (getTarget().MCAsmBackendCtorFn) {
                targetMCAsmBackend = getTarget().MCAsmBackendCtorFn(getTarget(), STI, MRI, Options.MCOptions);
            }
            std::unique_ptr<MCAsmBackend> MAB(targetMCAsmBackend);
            auto                          FOut = std::make_unique<formatted_raw_ostream>(Out);
            llvm::MCStreamer*             S = llvm::createAsmStreamer(
                Context,
                std::move(FOut),
                Options.MCOptions.AsmVerbose,
                UseDwarfDirectory,
                InstPrinter,
                std::move(MCE),
                std::move(MAB),
                Options.MCOptions.ShowMCInst
            );
            if (getTarget().AsmTargetStreamerCtorFn) {
                getTarget().AsmTargetStreamerCtorFn(*S, *FOut, InstPrinter, Options.MCOptions.AsmVerbose);
            }
            AsmStreamer.reset(S);
            break;
        }
        case CGFT_ObjectFile: {
            // Create the code emitter for the target if it exists.  If not, .o file
            // emission fails.
            if (!getTarget().MCAsmBackendCtorFn || !getTarget().MCCodeEmitterCtorFn) {
                return make_error<StringError>("failed", inconvertibleErrorCode());
            }
            MCCodeEmitter* MCE = getTarget().MCCodeEmitterCtorFn(MII, Context);
            MCAsmBackend* MAB = getTarget().MCAsmBackendCtorFn(getTarget(), STI, MRI, Options.MCOptions);

            Triple T(getTargetTriple().str());
            AsmStreamer.reset(LLVMTargetCreateMCObjectStreamer(
                &getTarget(),
                T,
                Context,
                std::unique_ptr<MCAsmBackend>(MAB),
                DwoOut ? MAB->createDwoObjectWriter(Out, *DwoOut) : MAB->createObjectWriter(Out),
                std::unique_ptr<MCCodeEmitter>(MCE),
                STI,
                Options.MCOptions.MCRelaxAll,
                Options.MCOptions.MCIncrementalLinkerCompatible,
                /*DWARFMustBeAtTheEnd*/ true
            ));
            break;
        }
        case CGFT_Null:
            // The Null output is intended for use for performance analysis and testing,
            // not real users.
            MCStreamer* streamer = llvm::createNullStreamer(Context);
            if (getTarget().NullTargetStreamerCtorFn) {
                getTarget().NullTargetStreamerCtorFn(*streamer);
            }
            AsmStreamer.reset(streamer);
            break;
    }

    return std::move(AsmStreamer);
}

bool
LLVMTargetMachine::addPassesToEmitFile(
    PassManagerBase&              PM,
    raw_pwrite_stream&            Out,
    raw_pwrite_stream*            DwoOut,
    CodeGenFileType               FileType,
    bool                          DisableVerify,
    MachineModuleInfoWrapperPass* MMIWP
) {
    // Add common CodeGen passes.
    if (!MMIWP)
        MMIWP = new MachineModuleInfoWrapperPass(this);
    TargetPassConfig* PassConfig =
        addPassesToGenerateCode(*this, PM, DisableVerify, *MMIWP);
    if (!PassConfig)
        return true;

    if (TargetPassConfig::willCompleteCodeGenPipeline()) {
        if (addAsmPrinter(PM, Out, DwoOut, FileType, MMIWP->getMMI().getContext()))
            return true;
    } else {
        // MIR printing is redundant with -filetype=null.
        if (FileType != CGFT_Null)
            PM.add(createPrintMIRPass(Out));
    }

    PM.add(createFreeMachineFunctionPass());
    return false;
}

/// addPassesToEmitMC - Add passes to the specified pass manager to get
/// machine code emitted with the MCJIT. This method returns true if machine
/// code is not supported. It fills the MCContext Ctx pointer which can be
/// used to build custom MCStreamer.
///
bool
LLVMTargetMachine::addPassesToEmitMC(PassManagerBase& PM, MCContext*& Ctx, raw_pwrite_stream& Out, bool DisableVerify) {
    // Add common CodeGen passes.
    MachineModuleInfoWrapperPass* MMIWP = new MachineModuleInfoWrapperPass(this);
    TargetPassConfig*             PassConfig =
        addPassesToGenerateCode(*this, PM, DisableVerify, *MMIWP);
    if (!PassConfig)
        return true;
    assert(TargetPassConfig::willCompleteCodeGenPipeline() && "Cannot emit MC with limited codegen pipeline");

    Ctx = &MMIWP->getMMI().getContext();
    // libunwind is unable to load compact unwind dynamically, so we must generate
    // DWARF unwind info for the JIT.
    Options.MCOptions.EmitDwarfUnwind = EmitDwarfUnwindType::Always;
    if (Options.MCOptions.MCSaveTempLabels)
        Ctx->setAllowTemporaryLabels(false);

    // Create the code emitter for the target if it exists.  If not, .o file
    // emission fails.
    const MCSubtargetInfo& STI = *getMCSubtargetInfo();
    const MCRegisterInfo&  MRI = *getMCRegisterInfo();
    const LLVMTarget*    TheTarget = &getTarget();
    if (TheTarget->MCAsmBackendCtorFn == 0 || TheTarget->MCCodeEmitterCtorFn == 0) {
        return true;
    }
    MCCodeEmitter* MCE = TheTarget->MCCodeEmitterCtorFn(*getMCInstrInfo(), *Ctx);
    MCAsmBackend*  MAB = TheTarget->MCAsmBackendCtorFn(*TheTarget, STI, MRI, Options.MCOptions);

    const Triple&               T = getTargetTriple();
    std::unique_ptr<MCStreamer> AsmStreamer(LLVMTargetCreateMCObjectStreamer(
        &getTarget(),
        T,
        *Ctx,
        std::unique_ptr<MCAsmBackend>(MAB),
        MAB->createObjectWriter(Out),
        std::unique_ptr<MCCodeEmitter>(MCE),
        STI,
        Options.MCOptions.MCRelaxAll,
        Options.MCOptions.MCIncrementalLinkerCompatible,
        /*DWARFMustBeAtTheEnd*/ true
    ));

    // Create the AsmPrinter, which takes ownership of AsmStreamer if successful.
    if (getTarget().AsmPrinterCtorFn == 0) {
        return true;
    }
    FunctionPass* Printer = getTarget().AsmPrinterCtorFn(*this, std::move(AsmStreamer));

    PM.add(Printer);
    PM.add(createFreeMachineFunctionPass());

    return false;  // success!
}
