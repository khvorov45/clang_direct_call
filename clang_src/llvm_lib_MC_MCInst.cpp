//===- lib/MC/MCInst.cpp - MCInst implementation --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCInst.h"
#include "llvm_include_llvm_Config_llvm-config.h"
#include "llvm_include_llvm_MC_MCExpr.h"
#include "llvm_include_llvm_MC_MCInstPrinter.h"
#include "llvm_include_llvm_MC_MCRegisterInfo.h"
#include "llvm_include_llvm_Support_Casting.h"
#include "llvm_include_llvm_Support_Compiler.h"
#include "llvm_include_llvm_Support_Debug.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace llvm;

void MCOperand::print(raw_ostream &OS, const MCRegisterInfo *RegInfo) const {
  OS << "<MCOperand ";
  if (!isValid())
    OS << "INVALID";
  else if (isReg()) {
    OS << "Reg:";
    if (RegInfo)
      OS << RegInfo->getName(getReg());
    else
      OS << getReg();
  } else if (isImm())
    OS << "Imm:" << getImm();
  else if (isSFPImm())
    OS << "SFPImm:" << bit_cast<float>(getSFPImm());
  else if (isDFPImm())
    OS << "DFPImm:" << bit_cast<double>(getDFPImm());
  else if (isExpr()) {
    OS << "Expr:(" << *getExpr() << ")";
  } else if (isInst()) {
    OS << "Inst:(";
    getInst()->print(OS, RegInfo);
    OS << ")";
  } else
    OS << "UNDEFINED";
  OS << ">";
}

bool MCOperand::evaluateAsConstantImm(int64_t &Imm) const {
  if (isImm()) {
    Imm = getImm();
    return true;
  }
  return false;
}

bool MCOperand::isBareSymbolRef() const {
  assert(isExpr() &&
         "isBareSymbolRef expects only expressions");
  const MCExpr *Expr = getExpr();
  MCExpr::ExprKind Kind = getExpr()->getKind();
  return Kind == MCExpr::SymbolRef &&
    cast<MCSymbolRefExpr>(Expr)->getKind() == MCSymbolRefExpr::VK_None;
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void MCOperand::dump() const {
  print(dbgs());
  dbgs() << "\n";
}
#endif

void MCInst::print(raw_ostream &OS, const MCRegisterInfo *RegInfo) const {
  OS << "<MCInst " << getOpcode();
  for (unsigned i = 0, e = getNumOperands(); i != e; ++i) {
    OS << " ";
    getOperand(i).print(OS, RegInfo);
  }
  OS << ">";
}

void MCInst::dump_pretty(raw_ostream &OS, const MCInstPrinter *Printer,
                         StringRef Separator,
                         const MCRegisterInfo *RegInfo) const {
  StringRef InstName = Printer ? Printer->getOpcodeName(getOpcode()) : "";
  dump_pretty(OS, InstName, Separator, RegInfo);
}

void MCInst::dump_pretty(raw_ostream &OS, StringRef Name, StringRef Separator,
                         const MCRegisterInfo *RegInfo) const {
  OS << "<MCInst #" << getOpcode();

  // Show the instruction opcode name if we have it.
  if (!Name.empty())
    OS << ' ' << Name;

  for (unsigned i = 0, e = getNumOperands(); i != e; ++i) {
    OS << Separator;
    getOperand(i).print(OS, RegInfo);
  }
  OS << ">";
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void MCInst::dump() const {
  print(dbgs());
  dbgs() << "\n";
}
#endif
