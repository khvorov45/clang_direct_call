#ifndef LLVM_MC_TARGETREGISTRY_H
#define LLVM_MC_TARGETREGISTRY_H

#include "llvm_include_llvm-c_DisassemblerTypes.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_ADT_Triple.h"
#include "llvm_include_llvm_ADT_iterator_range.h"
#include "llvm_include_llvm_MC_MCObjectFileInfo.h"
#include "llvm_include_llvm_Support_CodeGen.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_FormattedStream.h"
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <optional>
#include <string>

namespace llvm {

class AsmPrinter;
class MCAsmBackend;
class MCAsmInfo;
class MCAsmParser;
class MCCodeEmitter;
class MCContext;
class MCDisassembler;
class MCInstPrinter;
class MCInstrAnalysis;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCRelocationInfo;
class MCStreamer;
class MCSubtargetInfo;
class MCSymbolizer;
class MCTargetAsmParser;
class MCTargetOptions;
class MCTargetStreamer;
class raw_ostream;
class TargetMachine;
class TargetOptions;

namespace mca {
    class CustomBehaviour;
    class InstrPostProcess;
    class InstrumentManager;
    struct SourceMgr;
}  // namespace mca

MCStreamer*       createNullStreamer(MCContext& Ctx);
MCStreamer*       createAsmStreamer(MCContext& Ctx, std::unique_ptr<formatted_raw_ostream> OS, bool isVerboseAsm, bool useDwarfDirectory, MCInstPrinter* InstPrint, std::unique_ptr<MCCodeEmitter>&& CE, std::unique_ptr<MCAsmBackend>&& TAB, bool ShowInst);
MCStreamer*       createELFStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer*       createMachOStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll, bool DWARFMustBeAtTheEnd, bool LabelSections = false);
MCStreamer*       createWasmStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer*       createXCOFFStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer*       createSPIRVStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer*       createDXContainerStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCRelocationInfo* createMCRelocationInfo(const Triple& TT, MCContext& Ctx);
MCSymbolizer*     createMCSymbolizer(const Triple& TT, LLVMOpInfoCallback GetOpInfo, LLVMSymbolLookupCallback SymbolLookUp, void* DisInfo, MCContext* Ctx, std::unique_ptr<MCRelocationInfo>&& RelInfo);

struct Target {
    using MCAsmInfoCtorFnTy = MCAsmInfo* (*)(const MCRegisterInfo& MRI, const Triple& TT, const MCTargetOptions& Options);
    using MCObjectFileInfoCtorFnTy = MCObjectFileInfo* (*)(MCContext& Ctx, bool PIC, bool LargeCodeModel);
    using MCInstrInfoCtorFnTy = MCInstrInfo* (*)();
    using MCInstrAnalysisCtorFnTy = MCInstrAnalysis* (*)(const MCInstrInfo* Info);
    using MCRegInfoCtorFnTy = MCRegisterInfo* (*)(const Triple& TT);
    using MCSubtargetInfoCtorFnTy = MCSubtargetInfo* (*)(const Triple& TT, StringRef CPU, StringRef Features);
    using TargetMachineCtorTy = TargetMachine* (*)(const Target& T, const Triple& TT, StringRef CPU, StringRef Features, const TargetOptions& Options, std::optional<Reloc::Model> RM, std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT);
    using AsmPrinterCtorTy = AsmPrinter* (*)(TargetMachine& TM, std::unique_ptr<MCStreamer>&& Streamer);
    using MCAsmBackendCtorTy = MCAsmBackend* (*)(const Target& T, const MCSubtargetInfo& STI, const MCRegisterInfo& MRI, const MCTargetOptions& Options);
    using MCAsmParserCtorTy = MCTargetAsmParser* (*)(const MCSubtargetInfo& STI, MCAsmParser& P, const MCInstrInfo& MII, const MCTargetOptions& Options);
    using MCDisassemblerCtorTy = MCDisassembler* (*)(const Target& T, const MCSubtargetInfo& STI, MCContext& Ctx);
    using MCInstPrinterCtorTy = MCInstPrinter* (*)(const Triple& T, unsigned SyntaxVariant, const MCAsmInfo& MAI, const MCInstrInfo& MII, const MCRegisterInfo& MRI);
    using MCCodeEmitterCtorTy = MCCodeEmitter* (*)(const MCInstrInfo& II, MCContext& Ctx);
    using ELFStreamerCtorTy = MCStreamer* (*)(const Triple& T, MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, bool RelaxAll);
    using MachOStreamerCtorTy = MCStreamer* (*)(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, bool RelaxAll, bool DWARFMustBeAtTheEnd);
    using COFFStreamerCtorTy = MCStreamer* (*)(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, bool RelaxAll, bool IncrementalLinkerCompatible);
    using WasmStreamerCtorTy = MCStreamer* (*)(const Triple& T, MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, bool RelaxAll);
    using XCOFFStreamerCtorTy = MCStreamer* (*)(const Triple& T, MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, bool RelaxAll);
    using SPIRVStreamerCtorTy = MCStreamer* (*)(const Triple& T, MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, bool RelaxAll);
    using DXContainerStreamerCtorTy = MCStreamer* (*)(const Triple& T, MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, bool RelaxAll);
    using NullTargetStreamerCtorTy = MCTargetStreamer* (*)(MCStreamer& S);
    using AsmTargetStreamerCtorTy = MCTargetStreamer* (*)(MCStreamer& S, formatted_raw_ostream& OS, MCInstPrinter* InstPrint, bool IsVerboseAsm);
    using ObjectTargetStreamerCtorTy = MCTargetStreamer* (*)(MCStreamer& S, const MCSubtargetInfo& STI);
    using MCRelocationInfoCtorTy = MCRelocationInfo* (*)(const Triple& TT, MCContext& Ctx);
    using MCSymbolizerCtorTy = MCSymbolizer* (*)(const Triple& TT, LLVMOpInfoCallback GetOpInfo, LLVMSymbolLookupCallback SymbolLookUp, void* DisInfo, MCContext* Ctx, std::unique_ptr<MCRelocationInfo>&& RelInfo);
    using CustomBehaviourCtorTy = mca::CustomBehaviour* (*)(const MCSubtargetInfo& STI, const mca::SourceMgr& SrcMgr, const MCInstrInfo& MCII);
    using InstrPostProcessCtorTy = mca::InstrPostProcess* (*)(const MCSubtargetInfo& STI, const MCInstrInfo& MCII);
    using InstrumentManagerCtorTy = mca::InstrumentManager* (*)(const MCSubtargetInfo& STI, const MCInstrInfo& MCII);

    const char* Name;
    const char* ShortDesc;
    const char* BackendName;
    bool        HasJIT;

    MCAsmInfoCtorFnTy        MCAsmInfoCtorFn;
    MCObjectFileInfoCtorFnTy MCObjectFileInfoCtorFn;
    MCInstrInfoCtorFnTy      MCInstrInfoCtorFn;
    MCInstrAnalysisCtorFnTy  MCInstrAnalysisCtorFn;
    MCRegInfoCtorFnTy        MCRegInfoCtorFn;
    MCSubtargetInfoCtorFnTy  MCSubtargetInfoCtorFn;
    TargetMachineCtorTy      TargetMachineCtorFn;
    MCAsmBackendCtorTy       MCAsmBackendCtorFn;
    MCAsmParserCtorTy        MCAsmParserCtorFn;
    AsmPrinterCtorTy         AsmPrinterCtorFn;
    MCDisassemblerCtorTy     MCDisassemblerCtorFn;
    MCInstPrinterCtorTy      MCInstPrinterCtorFn;
    MCCodeEmitterCtorTy      MCCodeEmitterCtorFn;

    COFFStreamerCtorTy         COFFStreamerCtorFn = nullptr;
    MachOStreamerCtorTy        MachOStreamerCtorFn = nullptr;
    ELFStreamerCtorTy          ELFStreamerCtorFn = nullptr;
    WasmStreamerCtorTy         WasmStreamerCtorFn = nullptr;
    XCOFFStreamerCtorTy        XCOFFStreamerCtorFn = nullptr;
    SPIRVStreamerCtorTy        SPIRVStreamerCtorFn = nullptr;
    DXContainerStreamerCtorTy  DXContainerStreamerCtorFn = nullptr;
    NullTargetStreamerCtorTy   NullTargetStreamerCtorFn = nullptr;
    AsmTargetStreamerCtorTy    AsmTargetStreamerCtorFn = nullptr;
    ObjectTargetStreamerCtorTy ObjectTargetStreamerCtorFn = nullptr;
    MCRelocationInfoCtorTy     MCRelocationInfoCtorFn = nullptr;
    MCSymbolizerCtorTy         MCSymbolizerCtorFn = nullptr;
    CustomBehaviourCtorTy      CustomBehaviourCtorFn = nullptr;
    InstrPostProcessCtorTy     InstrPostProcessCtorFn = nullptr;
    InstrumentManagerCtorTy    InstrumentManagerCtorFn = nullptr;

    Target() = default;

    MCInstrAnalysis*
    createMCInstrAnalysis(const MCInstrInfo* Info) const {
        if (!MCInstrAnalysisCtorFn)
            return nullptr;
        return MCInstrAnalysisCtorFn(Info);
    }

    MCRegisterInfo*
    createMCRegInfo(StringRef TT) const {
        if (!MCRegInfoCtorFn)
            return nullptr;
        return MCRegInfoCtorFn(Triple(TT));
    }

    MCSubtargetInfo*
    createMCSubtargetInfo(StringRef TheTriple, StringRef CPU, StringRef Features) const {
        if (!MCSubtargetInfoCtorFn)
            return nullptr;
        return MCSubtargetInfoCtorFn(Triple(TheTriple), CPU, Features);
    }

    TargetMachine*
    createTargetMachine(
        StringRef                       TT,
        StringRef                       CPU,
        StringRef                       Features,
        const TargetOptions&            Options,
        std::optional<Reloc::Model>     RM,
        std::optional<CodeModel::Model> CM = std::nullopt,
        CodeGenOpt::Level               OL = CodeGenOpt::Default,
        bool                            JIT = false
    ) const {
        if (!TargetMachineCtorFn)
            return nullptr;
        return TargetMachineCtorFn(*this, Triple(TT), CPU, Features, Options, RM, CM, OL, JIT);
    }

    MCAsmBackend*
    createMCAsmBackend(const MCSubtargetInfo& STI, const MCRegisterInfo& MRI, const MCTargetOptions& Options) const {
        if (!MCAsmBackendCtorFn)
            return nullptr;
        return MCAsmBackendCtorFn(*this, STI, MRI, Options);
    }

    MCTargetAsmParser*
    createMCAsmParser(const MCSubtargetInfo& STI, MCAsmParser& Parser, const MCInstrInfo& MII, const MCTargetOptions& Options) const {
        if (!MCAsmParserCtorFn)
            return nullptr;
        return MCAsmParserCtorFn(STI, Parser, MII, Options);
    }

    AsmPrinter*
    createAsmPrinter(TargetMachine& TM, std::unique_ptr<MCStreamer>&& Streamer) const {
        if (!AsmPrinterCtorFn)
            return nullptr;
        return AsmPrinterCtorFn(TM, std::move(Streamer));
    }

    MCDisassembler*
    createMCDisassembler(const MCSubtargetInfo& STI, MCContext& Ctx) const {
        if (!MCDisassemblerCtorFn)
            return nullptr;
        return MCDisassemblerCtorFn(*this, STI, Ctx);
    }

    MCInstPrinter*
    createMCInstPrinter(const Triple& T, unsigned SyntaxVariant, const MCAsmInfo& MAI, const MCInstrInfo& MII, const MCRegisterInfo& MRI) const {
        if (!MCInstPrinterCtorFn)
            return nullptr;
        return MCInstPrinterCtorFn(T, SyntaxVariant, MAI, MII, MRI);
    }

    MCCodeEmitter*
    createMCCodeEmitter(const MCInstrInfo& II, MCContext& Ctx) const {
        if (!MCCodeEmitterCtorFn)
            return nullptr;
        return MCCodeEmitterCtorFn(II, Ctx);
    }

    MCStreamer*
    createNullStreamer(MCContext& Ctx) const {
        MCStreamer* S = llvm::createNullStreamer(Ctx);
        createNullTargetStreamer(*S);
        return S;
    }

    MCTargetStreamer*
    createNullTargetStreamer(MCStreamer& S) const {
        if (NullTargetStreamerCtorFn)
            return NullTargetStreamerCtorFn(S);
        return nullptr;
    }

    MCRelocationInfo*
    createMCRelocationInfo(StringRef TT, MCContext& Ctx) const {
        MCRelocationInfoCtorTy Fn = MCRelocationInfoCtorFn
            ? MCRelocationInfoCtorFn
            : llvm::createMCRelocationInfo;
        return Fn(Triple(TT), Ctx);
    }

    MCSymbolizer*
    createMCSymbolizer(StringRef TT, LLVMOpInfoCallback GetOpInfo, LLVMSymbolLookupCallback SymbolLookUp, void* DisInfo, MCContext* Ctx, std::unique_ptr<MCRelocationInfo>&& RelInfo) const {
        MCSymbolizerCtorTy Fn =
            MCSymbolizerCtorFn ? MCSymbolizerCtorFn : llvm::createMCSymbolizer;
        return Fn(Triple(TT), GetOpInfo, SymbolLookUp, DisInfo, Ctx, std::move(RelInfo));
    }
};

}  // end namespace llvm

extern llvm::Target* LLVMTargetRegistryTheTarget;

static inline llvm::MCObjectFileInfo*
LLVMTargetCreateMCObjectFileInfo(llvm::MCContext& Ctx) {
    llvm::MCObjectFileInfo* MOFI = new llvm::MCObjectFileInfo();
    MOFI->initMCObjectFileInfo(Ctx, false, false);
    return MOFI;
}

#endif  // LLVM_MC_TARGETREGISTRY_H
