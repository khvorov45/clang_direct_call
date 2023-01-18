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
}  // namespace llvm

struct LLVMTarget;

typedef llvm::MCAsmInfo* (*LLVMTargetMCAsmInfoCtorFnTy)(const llvm::MCRegisterInfo& MRI, const llvm::Triple& TT, const llvm::MCTargetOptions& Options);
typedef llvm::MCObjectFileInfo* (*LLVMTargetMCObjectFileInfoCtorFnTy)(llvm::MCContext& Ctx, bool PIC, bool LargeCodeModel);
typedef llvm::MCInstrInfo* (*LLVMTargetMCInstrInfoCtorFnTy)();
typedef llvm::MCInstrAnalysis* (*LLVMTargetMCInstrAnalysisCtorFnTy)(const llvm::MCInstrInfo* Info);
typedef llvm::MCRegisterInfo* (*LLVMTargetMCRegInfoCtorFnTy)(const llvm::Triple& TT);
typedef llvm::MCSubtargetInfo* (*LLVMTargetMCSubtargetInfoCtorFnTy)(const llvm::Triple& TT, llvm::StringRef CPU, llvm::StringRef Features);
typedef llvm::TargetMachine* (*LLVMTargetTargetMachineCtorTy)(const LLVMTarget& T, const llvm::Triple& TT, llvm::StringRef CPU, llvm::StringRef Features, const llvm::TargetOptions& Options, std::optional<llvm::Reloc::Model> RM, std::optional<llvm::CodeModel::Model> CM, llvm::CodeGenOpt::Level OL, bool JIT);
typedef llvm::AsmPrinter* (*LLVMTargetAsmPrinterCtorTy)(llvm::TargetMachine& TM, std::unique_ptr<llvm::MCStreamer>&& Streamer);
typedef llvm::MCAsmBackend* (*LLVMTargetMCAsmBackendCtorTy)(const LLVMTarget& T, const llvm::MCSubtargetInfo& STI, const llvm::MCRegisterInfo& MRI, const llvm::MCTargetOptions& Options);
typedef llvm::MCTargetAsmParser* (*LLVMTargetMCAsmParserCtorTy)(const llvm::MCSubtargetInfo& STI, llvm::MCAsmParser& P, const llvm::MCInstrInfo& MII, const llvm::MCTargetOptions& Options);
typedef llvm::MCDisassembler* (*LLVMTargetMCDisassemblerCtorTy)(const LLVMTarget& T, const llvm::MCSubtargetInfo& STI, llvm::MCContext& Ctx);
typedef llvm::MCInstPrinter* (*LLVMTargetMCInstPrinterCtorTy)(const llvm::Triple& T, unsigned SyntaxVariant, const llvm::MCAsmInfo& MAI, const llvm::MCInstrInfo& MII, const llvm::MCRegisterInfo& MRI);
typedef llvm::MCCodeEmitter* (*LLVMTargetMCCodeEmitterCtorTy)(const llvm::MCInstrInfo& II, llvm::MCContext& Ctx);
typedef llvm::MCStreamer* (*LLVMTargetELFStreamerCtorTy)(const llvm::Triple& T, llvm::MCContext& Ctx, std::unique_ptr<llvm::MCAsmBackend>&& TAB, std::unique_ptr<llvm::MCObjectWriter>&& OW, std::unique_ptr<llvm::MCCodeEmitter>&& Emitter, bool RelaxAll);
typedef llvm::MCStreamer* (*LLVMTargetMachOStreamerCtorTy)(llvm::MCContext& Ctx, std::unique_ptr<llvm::MCAsmBackend>&& TAB, std::unique_ptr<llvm::MCObjectWriter>&& OW, std::unique_ptr<llvm::MCCodeEmitter>&& Emitter, bool RelaxAll, bool DWARFMustBeAtTheEnd);
typedef llvm::MCStreamer* (*LLVMTargetCOFFStreamerCtorTy)(llvm::MCContext& Ctx, std::unique_ptr<llvm::MCAsmBackend>&& TAB, std::unique_ptr<llvm::MCObjectWriter>&& OW, std::unique_ptr<llvm::MCCodeEmitter>&& Emitter, bool RelaxAll, bool IncrementalLinkerCompatible);
typedef llvm::MCStreamer* (*LLVMTargetWasmStreamerCtorTy)(const llvm::Triple& T, llvm::MCContext& Ctx, std::unique_ptr<llvm::MCAsmBackend>&& TAB, std::unique_ptr<llvm::MCObjectWriter>&& OW, std::unique_ptr<llvm::MCCodeEmitter>&& Emitter, bool RelaxAll);
typedef llvm::MCStreamer* (*LLVMTargetXCOFFStreamerCtorTy)(const llvm::Triple& T, llvm::MCContext& Ctx, std::unique_ptr<llvm::MCAsmBackend>&& TAB, std::unique_ptr<llvm::MCObjectWriter>&& OW, std::unique_ptr<llvm::MCCodeEmitter>&& Emitter, bool RelaxAll);
typedef llvm::MCStreamer* (*LLVMTargetSPIRVStreamerCtorTy)(const llvm::Triple& T, llvm::MCContext& Ctx, std::unique_ptr<llvm::MCAsmBackend>&& TAB, std::unique_ptr<llvm::MCObjectWriter>&& OW, std::unique_ptr<llvm::MCCodeEmitter>&& Emitter, bool RelaxAll);
typedef llvm::MCStreamer* (*LLVMTargetDXContainerStreamerCtorTy)(const llvm::Triple& T, llvm::MCContext& Ctx, std::unique_ptr<llvm::MCAsmBackend>&& TAB, std::unique_ptr<llvm::MCObjectWriter>&& OW, std::unique_ptr<llvm::MCCodeEmitter>&& Emitter, bool RelaxAll);
typedef llvm::MCTargetStreamer* (*LLVMTargetNullTargetStreamerCtorTy)(llvm::MCStreamer& S);
typedef llvm::MCTargetStreamer* (*LLVMTargetAsmTargetStreamerCtorTy)(llvm::MCStreamer& S, llvm::formatted_raw_ostream& OS, llvm::MCInstPrinter* InstPrint, bool IsVerboseAsm);
typedef llvm::MCTargetStreamer* (*LLVMTargetObjectTargetStreamerCtorTy)(llvm::MCStreamer& S, const llvm::MCSubtargetInfo& STI);
typedef llvm::MCRelocationInfo* (*LLVMTargetMCRelocationInfoCtorTy)(const llvm::Triple& TT, llvm::MCContext& Ctx);
typedef llvm::MCSymbolizer* (*LLVMTargetMCSymbolizerCtorTy)(const llvm::Triple& TT, LLVMOpInfoCallback GetOpInfo, LLVMSymbolLookupCallback SymbolLookUp, void* DisInfo, llvm::MCContext* Ctx, std::unique_ptr<llvm::MCRelocationInfo>&& RelInfo);
typedef llvm::mca::CustomBehaviour* (*LLVMTargetCustomBehaviourCtorTy)(const llvm::MCSubtargetInfo& STI, const llvm::mca::SourceMgr& SrcMgr, const llvm::MCInstrInfo& MCII);
typedef llvm::mca::InstrPostProcess* (*LLVMTargetInstrPostProcessCtorTy)(const llvm::MCSubtargetInfo& STI, const llvm::MCInstrInfo& MCII);
typedef llvm::mca::InstrumentManager* (*LLVMTargetInstrumentManagerCtorTy)(const llvm::MCSubtargetInfo& STI, const llvm::MCInstrInfo& MCII);

struct LLVMTarget {
    const char* Name;
    const char* ShortDesc;
    const char* BackendName;
    bool        HasJIT;

    LLVMTargetMCAsmInfoCtorFnTy          MCAsmInfoCtorFn;
    LLVMTargetMCObjectFileInfoCtorFnTy   MCObjectFileInfoCtorFn;
    LLVMTargetMCInstrInfoCtorFnTy        MCInstrInfoCtorFn;
    LLVMTargetMCInstrAnalysisCtorFnTy    MCInstrAnalysisCtorFn;
    LLVMTargetMCRegInfoCtorFnTy          MCRegInfoCtorFn;
    LLVMTargetMCSubtargetInfoCtorFnTy    MCSubtargetInfoCtorFn;
    LLVMTargetTargetMachineCtorTy        TargetMachineCtorFn;
    LLVMTargetMCAsmBackendCtorTy         MCAsmBackendCtorFn;
    LLVMTargetMCAsmParserCtorTy          MCAsmParserCtorFn;
    LLVMTargetAsmPrinterCtorTy           AsmPrinterCtorFn;
    LLVMTargetMCDisassemblerCtorTy       MCDisassemblerCtorFn;
    LLVMTargetMCInstPrinterCtorTy        MCInstPrinterCtorFn;
    LLVMTargetMCCodeEmitterCtorTy        MCCodeEmitterCtorFn;
    LLVMTargetCOFFStreamerCtorTy         COFFStreamerCtorFn;
    LLVMTargetMachOStreamerCtorTy        MachOStreamerCtorFn;
    LLVMTargetELFStreamerCtorTy          ELFStreamerCtorFn;
    LLVMTargetWasmStreamerCtorTy         WasmStreamerCtorFn;
    LLVMTargetXCOFFStreamerCtorTy        XCOFFStreamerCtorFn;
    LLVMTargetSPIRVStreamerCtorTy        SPIRVStreamerCtorFn;
    LLVMTargetDXContainerStreamerCtorTy  DXContainerStreamerCtorFn;
    LLVMTargetNullTargetStreamerCtorTy   NullTargetStreamerCtorFn;
    LLVMTargetAsmTargetStreamerCtorTy    AsmTargetStreamerCtorFn;
    LLVMTargetObjectTargetStreamerCtorTy ObjectTargetStreamerCtorFn;
    LLVMTargetMCRelocationInfoCtorTy     MCRelocationInfoCtorFn;
    LLVMTargetMCSymbolizerCtorTy         MCSymbolizerCtorFn;
    LLVMTargetCustomBehaviourCtorTy      CustomBehaviourCtorFn;
    LLVMTargetInstrPostProcessCtorTy     InstrPostProcessCtorFn;
    LLVMTargetInstrumentManagerCtorTy    InstrumentManagerCtorFn;
};

extern LLVMTarget* LLVMTargetRegistryTheTarget;

#endif  // LLVM_MC_TARGETREGISTRY_H
