//===- MC/TargetRegistry.h - Target Registration ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file exposes the TargetRegistry interface, which tools can use to access
// the appropriate target specific classes (TargetMachine, AsmPrinter, etc.)
// which have been registered.
//
// Target specific class implementations should register themselves using the
// appropriate TargetRegistry interfaces.
//
//===----------------------------------------------------------------------===//

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

MCStreamer* createNullStreamer(MCContext& Ctx);
// Takes ownership of \p TAB and \p CE.

/// Create a machine code streamer which will print out assembly for the native
/// target, suitable for compiling with a native assembler.
///
/// \param InstPrint - If given, the instruction printer to use. If not given
/// the MCInst representation will be printed.  This method takes ownership of
/// InstPrint.
///
/// \param CE - If given, a code emitter to use to show the instruction
/// encoding inline with the assembly. This method takes ownership of \p CE.
///
/// \param TAB - If given, a target asm backend to use to show the fixup
/// information in conjunction with encoding information. This method takes
/// ownership of \p TAB.
///
/// \param ShowInst - Whether to show the MCInst representation inline with
/// the assembly.
MCStreamer* createAsmStreamer(MCContext& Ctx, std::unique_ptr<formatted_raw_ostream> OS, bool isVerboseAsm, bool useDwarfDirectory, MCInstPrinter* InstPrint, std::unique_ptr<MCCodeEmitter>&& CE, std::unique_ptr<MCAsmBackend>&& TAB, bool ShowInst);

MCStreamer* createELFStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer* createMachOStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll, bool DWARFMustBeAtTheEnd, bool LabelSections = false);
MCStreamer* createWasmStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer* createXCOFFStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer* createSPIRVStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);
MCStreamer* createDXContainerStreamer(MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& CE, bool RelaxAll);

MCRelocationInfo*       createMCRelocationInfo(const Triple& TT, MCContext& Ctx);
MCSymbolizer*           createMCSymbolizer(const Triple& TT, LLVMOpInfoCallback GetOpInfo, LLVMSymbolLookupCallback SymbolLookUp, void* DisInfo, MCContext* Ctx, std::unique_ptr<MCRelocationInfo>&& RelInfo);
mca::CustomBehaviour*   createCustomBehaviour(const MCSubtargetInfo& STI, const mca::SourceMgr& SrcMgr, const MCInstrInfo& MCII);
mca::InstrPostProcess*  createInstrPostProcess(const MCSubtargetInfo& STI, const MCInstrInfo& MCII);
mca::InstrumentManager* createInstrumentManager(const MCSubtargetInfo& STI, const MCInstrInfo& MCII);

/// Target - Wrapper for Target specific information.
///
/// For registration purposes, this is a POD type so that targets can be
/// registered without the use of static constructors.
///
/// Targets should implement a single global instance of this class (which
/// will be zero initialized), and pass that instance to the TargetRegistry as
/// part of their initialization.
struct Target {
    using MCAsmInfoCtorFnTy = MCAsmInfo* (*)(const MCRegisterInfo& MRI, const Triple& TT, const MCTargetOptions& Options);
    using MCObjectFileInfoCtorFnTy = MCObjectFileInfo* (*)(MCContext& Ctx, bool PIC, bool LargeCodeModel);
    using MCInstrInfoCtorFnTy = MCInstrInfo* (*)();
    using MCInstrAnalysisCtorFnTy = MCInstrAnalysis* (*)(const MCInstrInfo* Info);
    using MCRegInfoCtorFnTy = MCRegisterInfo* (*)(const Triple& TT);
    using MCSubtargetInfoCtorFnTy = MCSubtargetInfo* (*)(const Triple& TT, StringRef CPU, StringRef Features);
    using TargetMachineCtorTy = TargetMachine* (*)(const Target& T, const Triple& TT, StringRef CPU, StringRef Features, const TargetOptions& Options, std::optional<Reloc::Model> RM, std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT);
    // If it weren't for layering issues (this header is in llvm/Support, but
    // depends on MC?) this should take the Streamer by value rather than rvalue
    // reference.
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

    /// createMCInstrInfo - Create a MCInstrInfo implementation.
    ///
    MCInstrInfo*
    createMCInstrInfo() const {
        if (!MCInstrInfoCtorFn)
            return nullptr;
        return MCInstrInfoCtorFn();
    }

    /// createMCInstrAnalysis - Create a MCInstrAnalysis implementation.
    ///
    MCInstrAnalysis*
    createMCInstrAnalysis(const MCInstrInfo* Info) const {
        if (!MCInstrAnalysisCtorFn)
            return nullptr;
        return MCInstrAnalysisCtorFn(Info);
    }

    /// createMCRegInfo - Create a MCRegisterInfo implementation.
    ///
    MCRegisterInfo*
    createMCRegInfo(StringRef TT) const {
        if (!MCRegInfoCtorFn)
            return nullptr;
        return MCRegInfoCtorFn(Triple(TT));
    }

    /// createMCSubtargetInfo - Create a MCSubtargetInfo implementation.
    ///
    /// \param TheTriple This argument is used to determine the target machine
    /// feature set; it should always be provided. Generally this should be
    /// either the target triple from the module, or the target triple of the
    /// host if that does not exist.
    /// \param CPU This specifies the name of the target CPU.
    /// \param Features This specifies the string representation of the
    /// additional target features.
    MCSubtargetInfo*
    createMCSubtargetInfo(StringRef TheTriple, StringRef CPU, StringRef Features) const {
        if (!MCSubtargetInfoCtorFn)
            return nullptr;
        return MCSubtargetInfoCtorFn(Triple(TheTriple), CPU, Features);
    }

    /// createTargetMachine - Create a target specific machine implementation
    /// for the specified \p Triple.
    ///
    /// \param TT This argument is used to determine the target machine
    /// feature set; it should always be provided. Generally this should be
    /// either the target triple from the module, or the target triple of the
    /// host if that does not exist.
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

    /// createMCAsmBackend - Create a target specific assembly parser.
    MCAsmBackend*
    createMCAsmBackend(const MCSubtargetInfo& STI, const MCRegisterInfo& MRI, const MCTargetOptions& Options) const {
        if (!MCAsmBackendCtorFn)
            return nullptr;
        return MCAsmBackendCtorFn(*this, STI, MRI, Options);
    }

    /// createMCAsmParser - Create a target specific assembly parser.
    ///
    /// \param Parser The target independent parser implementation to use for
    /// parsing and lexing.
    MCTargetAsmParser*
    createMCAsmParser(const MCSubtargetInfo& STI, MCAsmParser& Parser, const MCInstrInfo& MII, const MCTargetOptions& Options) const {
        if (!MCAsmParserCtorFn)
            return nullptr;
        return MCAsmParserCtorFn(STI, Parser, MII, Options);
    }

    /// createAsmPrinter - Create a target specific assembly printer pass.  This
    /// takes ownership of the MCStreamer object.
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

    /// createMCCodeEmitter - Create a target specific code emitter.
    MCCodeEmitter*
    createMCCodeEmitter(const MCInstrInfo& II, MCContext& Ctx) const {
        if (!MCCodeEmitterCtorFn)
            return nullptr;
        return MCCodeEmitterCtorFn(II, Ctx);
    }

    /// Create a target specific MCStreamer.
    ///
    /// \param T The target triple.
    /// \param Ctx The target context.
    /// \param TAB The target assembler backend object. Takes ownership.
    /// \param OW The stream object.
    /// \param Emitter The target independent assembler object.Takes ownership.
    /// \param RelaxAll Relax all fixups?
    MCStreamer*
    createMCObjectStreamer(const Triple& T, MCContext& Ctx, std::unique_ptr<MCAsmBackend>&& TAB, std::unique_ptr<MCObjectWriter>&& OW, std::unique_ptr<MCCodeEmitter>&& Emitter, const MCSubtargetInfo& STI, bool RelaxAll, bool IncrementalLinkerCompatible, bool DWARFMustBeAtTheEnd) const {
        MCStreamer* S = nullptr;
        switch (T.getObjectFormat()) {
            case Triple::UnknownObjectFormat:
                llvm_unreachable("Unknown object format");
            case Triple::COFF:
                assert(T.isOSWindows() && "only Windows COFF is supported");
                S = COFFStreamerCtorFn(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll, IncrementalLinkerCompatible);
                break;
            case Triple::MachO:
                if (MachOStreamerCtorFn)
                    S = MachOStreamerCtorFn(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll, DWARFMustBeAtTheEnd);
                else
                    S = createMachOStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll, DWARFMustBeAtTheEnd);
                break;
            case Triple::ELF:
                if (ELFStreamerCtorFn)
                    S = ELFStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                else
                    S = createELFStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                break;
            case Triple::Wasm:
                if (WasmStreamerCtorFn)
                    S = WasmStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                else
                    S = createWasmStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                break;
            case Triple::GOFF:
                report_fatal_error("GOFF MCObjectStreamer not implemented yet");
            case Triple::XCOFF:
                if (XCOFFStreamerCtorFn)
                    S = XCOFFStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                else
                    S = createXCOFFStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                break;
            case Triple::SPIRV:
                if (SPIRVStreamerCtorFn)
                    S = SPIRVStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                else
                    S = createSPIRVStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                break;
            case Triple::DXContainer:
                if (DXContainerStreamerCtorFn)
                    S = DXContainerStreamerCtorFn(T, Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                else
                    S = createDXContainerStreamer(Ctx, std::move(TAB), std::move(OW), std::move(Emitter), RelaxAll);
                break;
        }
        if (ObjectTargetStreamerCtorFn)
            ObjectTargetStreamerCtorFn(*S, STI);
        return S;
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

    /// createMCRelocationInfo - Create a target specific MCRelocationInfo.
    ///
    /// \param TT The target triple.
    /// \param Ctx The target context.
    MCRelocationInfo*
    createMCRelocationInfo(StringRef TT, MCContext& Ctx) const {
        MCRelocationInfoCtorTy Fn = MCRelocationInfoCtorFn
            ? MCRelocationInfoCtorFn
            : llvm::createMCRelocationInfo;
        return Fn(Triple(TT), Ctx);
    }

    /// createMCSymbolizer - Create a target specific MCSymbolizer.
    ///
    /// \param TT The target triple.
    /// \param GetOpInfo The function to get the symbolic information for
    /// operands.
    /// \param SymbolLookUp The function to lookup a symbol name.
    /// \param DisInfo The pointer to the block of symbolic information for above
    /// call
    /// back.
    /// \param Ctx The target context.
    /// \param RelInfo The relocation information for this target. Takes
    /// ownership.
    MCSymbolizer*
    createMCSymbolizer(StringRef TT, LLVMOpInfoCallback GetOpInfo, LLVMSymbolLookupCallback SymbolLookUp, void* DisInfo, MCContext* Ctx, std::unique_ptr<MCRelocationInfo>&& RelInfo) const {
        MCSymbolizerCtorTy Fn =
            MCSymbolizerCtorFn ? MCSymbolizerCtorFn : llvm::createMCSymbolizer;
        return Fn(Triple(TT), GetOpInfo, SymbolLookUp, DisInfo, Ctx, std::move(RelInfo));
    }

    /// createCustomBehaviour - Create a target specific CustomBehaviour.
    /// This class is used by llvm-mca and requires backend functionality.
    mca::CustomBehaviour*
    createCustomBehaviour(const MCSubtargetInfo& STI, const mca::SourceMgr& SrcMgr, const MCInstrInfo& MCII) const {
        if (CustomBehaviourCtorFn)
            return CustomBehaviourCtorFn(STI, SrcMgr, MCII);
        return nullptr;
    }

    /// createInstrPostProcess - Create a target specific InstrPostProcess.
    /// This class is used by llvm-mca and requires backend functionality.
    mca::InstrPostProcess*
    createInstrPostProcess(const MCSubtargetInfo& STI, const MCInstrInfo& MCII) const {
        if (InstrPostProcessCtorFn)
            return InstrPostProcessCtorFn(STI, MCII);
        return nullptr;
    }

    /// createInstrumentManager - Create a target specific
    /// InstrumentManager. This class is used by llvm-mca and requires
    /// backend functionality.
    mca::InstrumentManager*
    createInstrumentManager(const MCSubtargetInfo& STI, const MCInstrInfo& MCII) const {
        if (InstrumentManagerCtorFn)
            return InstrumentManagerCtorFn(STI, MCII);
        return nullptr;
    }

    /// @}
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
