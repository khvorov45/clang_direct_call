//===- ModuleSymbolTable.cpp - symbol table for in-memory IR --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class represents a symbol table built from in-memory IR. It provides
// access to GlobalValues and should only be used if such access is required
// (e.g. in the LTO implementation).
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Object_ModuleSymbolTable.h"
#include "llvm_lib_Object_RecordStreamer.h"
#include "llvm_include_llvm_ADT_STLExtras.h"
#include "llvm_include_llvm_ADT_StringMap.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_ADT_Triple.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_GlobalAlias.h"
#include "llvm_include_llvm_IR_GlobalValue.h"
#include "llvm_include_llvm_IR_GlobalVariable.h"
#include "llvm_include_llvm_IR_InlineAsm.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_MC_MCAsmInfo.h"
#include "llvm_include_llvm_MC_MCContext.h"
#include "llvm_include_llvm_MC_MCInstrInfo.h"
#include "llvm_include_llvm_MC_MCObjectFileInfo.h"
#include "llvm_include_llvm_MC_MCParser_MCAsmParser.h"
#include "llvm_include_llvm_MC_MCParser_MCTargetAsmParser.h"
#include "llvm_include_llvm_MC_MCRegisterInfo.h"
#include "llvm_include_llvm_MC_MCSubtargetInfo.h"
#include "llvm_include_llvm_MC_MCSymbol.h"
#include "llvm_include_llvm_MC_MCTargetOptions.h"
#include "llvm_include_llvm_MC_TargetRegistry.h"
#include "llvm_include_llvm_Object_SymbolicFile.h"
#include "llvm_include_llvm_Support_Casting.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_MemoryBuffer.h"
#include "llvm_include_llvm_Support_SMLoc.h"
#include "llvm_include_llvm_Support_SourceMgr.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

using namespace llvm;
using namespace object;

void ModuleSymbolTable::addModule(Module *M) {
  if (FirstMod)
    assert(FirstMod->getTargetTriple() == M->getTargetTriple());
  else
    FirstMod = M;

  for (GlobalValue &GV : M->global_values())
    SymTab.push_back(&GV);

  CollectAsmSymbols(*M, [this](StringRef Name, BasicSymbolRef::Flags Flags) {
    SymTab.push_back(new (AsmSymbols.Allocate())
                         AsmSymbol(std::string(Name), Flags));
  });
}

static void
initializeRecordStreamer(const Module &M,
                         function_ref<void(RecordStreamer &)> Init) {
  StringRef InlineAsm = M.getModuleInlineAsm();
  if (InlineAsm.empty())
    return;

  std::string Err;
  const Triple TT(M.getTargetTriple());
  const Target *T = LLVMTargetRegistryTheTarget;
  assert(T && T->MCAsmParserCtorFn);

  std::unique_ptr<MCRegisterInfo> MRI(T->createMCRegInfo(TT.str()));
  if (!MRI)
    return;

  MCTargetOptions MCOptions;
  std::unique_ptr<MCAsmInfo> MAI(T->createMCAsmInfo(*MRI, TT.str(), MCOptions));
  if (!MAI)
    return;

  std::unique_ptr<MCSubtargetInfo> STI(
      T->createMCSubtargetInfo(TT.str(), "", ""));
  if (!STI)
    return;

  std::unique_ptr<MCInstrInfo> MCII(T->createMCInstrInfo());
  if (!MCII)
    return;

  std::unique_ptr<MemoryBuffer> Buffer(MemoryBuffer::getMemBuffer(InlineAsm));
  SourceMgr SrcMgr;
  SrcMgr.AddNewSourceBuffer(std::move(Buffer), SMLoc());

  MCContext MCCtx(TT, MAI.get(), MRI.get(), STI.get(), &SrcMgr);
  std::unique_ptr<MCObjectFileInfo> MOFI(
      T->createMCObjectFileInfo(MCCtx, /*PIC=*/false));
  MOFI->setSDKVersion(M.getSDKVersion());
  MCCtx.setObjectFileInfo(MOFI.get());
  RecordStreamer Streamer(MCCtx, M);
  T->createNullTargetStreamer(Streamer);

  std::unique_ptr<MCAsmParser> Parser(
      createMCAsmParser(SrcMgr, MCCtx, Streamer, *MAI));

  std::unique_ptr<MCTargetAsmParser> TAP(
      T->createMCAsmParser(*STI, *Parser, *MCII, MCOptions));
  if (!TAP)
    return;

  // Module-level inline asm is assumed to use At&t syntax (see
  // AsmPrinter::doInitialization()).
  Parser->setAssemblerDialect(InlineAsm::AD_ATT);

  Parser->setTargetParser(*TAP);
  if (Parser->Run(false))
    return;

  Init(Streamer);
}

void ModuleSymbolTable::CollectAsmSymbols(
    const Module &M,
    function_ref<void(StringRef, BasicSymbolRef::Flags)> AsmSymbol) {
  initializeRecordStreamer(M, [&](RecordStreamer &Streamer) {
    Streamer.flushSymverDirectives();

    for (auto &KV : Streamer) {
      StringRef Key = KV.first();
      RecordStreamer::State Value = KV.second;
      // FIXME: For now we just assume that all asm symbols are executable.
      uint32_t Res = BasicSymbolRef::SF_Executable;
      switch (Value) {
      case RecordStreamer::NeverSeen:
        llvm_unreachable("NeverSeen should have been replaced earlier");
      case RecordStreamer::DefinedGlobal:
        Res |= BasicSymbolRef::SF_Global;
        break;
      case RecordStreamer::Defined:
        break;
      case RecordStreamer::Global:
      case RecordStreamer::Used:
        Res |= BasicSymbolRef::SF_Undefined;
        Res |= BasicSymbolRef::SF_Global;
        break;
      case RecordStreamer::DefinedWeak:
        Res |= BasicSymbolRef::SF_Weak;
        Res |= BasicSymbolRef::SF_Global;
        break;
      case RecordStreamer::UndefinedWeak:
        Res |= BasicSymbolRef::SF_Weak;
        Res |= BasicSymbolRef::SF_Undefined;
      }
      AsmSymbol(Key, BasicSymbolRef::Flags(Res));
    }
  });
}

void ModuleSymbolTable::CollectAsmSymvers(
    const Module &M, function_ref<void(StringRef, StringRef)> AsmSymver) {
  initializeRecordStreamer(M, [&](RecordStreamer &Streamer) {
    for (auto &KV : Streamer.symverAliases())
      for (auto &Alias : KV.second)
        AsmSymver(KV.first->getName(), Alias);
  });
}

void ModuleSymbolTable::printSymbolName(raw_ostream &OS, Symbol S) const {
  if (S.is<AsmSymbol *>()) {
    OS << S.get<AsmSymbol *>()->first;
    return;
  }

  auto *GV = S.get<GlobalValue *>();
  if (GV->hasDLLImportStorageClass())
    OS << "__imp_";

  Mang.getNameWithPrefix(OS, GV, false);
}

uint32_t ModuleSymbolTable::getSymbolFlags(Symbol S) const {
  if (S.is<AsmSymbol *>())
    return S.get<AsmSymbol *>()->second;

  auto *GV = S.get<GlobalValue *>();

  uint32_t Res = BasicSymbolRef::SF_None;
  if (GV->isDeclarationForLinker())
    Res |= BasicSymbolRef::SF_Undefined;
  else if (GV->hasHiddenVisibility() && !GV->hasLocalLinkage())
    Res |= BasicSymbolRef::SF_Hidden;
  if (const GlobalVariable *GVar = dyn_cast<GlobalVariable>(GV)) {
    if (GVar->isConstant())
      Res |= BasicSymbolRef::SF_Const;
  }
  if (const GlobalObject *GO = GV->getAliaseeObject())
    if (isa<Function>(GO) || isa<GlobalIFunc>(GO))
      Res |= BasicSymbolRef::SF_Executable;
  if (isa<GlobalAlias>(GV))
    Res |= BasicSymbolRef::SF_Indirect;
  if (GV->hasPrivateLinkage())
    Res |= BasicSymbolRef::SF_FormatSpecific;
  if (!GV->hasLocalLinkage())
    Res |= BasicSymbolRef::SF_Global;
  if (GV->hasCommonLinkage())
    Res |= BasicSymbolRef::SF_Common;
  if (GV->hasLinkOnceLinkage() || GV->hasWeakLinkage() ||
      GV->hasExternalWeakLinkage())
    Res |= BasicSymbolRef::SF_Weak;

  if (GV->getName().startswith("llvm."))
    Res |= BasicSymbolRef::SF_FormatSpecific;
  else if (auto *Var = dyn_cast<GlobalVariable>(GV)) {
    if (Var->getSection() == "llvm.metadata")
      Res |= BasicSymbolRef::SF_FormatSpecific;
  }

  return Res;
}
