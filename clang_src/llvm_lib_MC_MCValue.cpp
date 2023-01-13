//===- lib/MC/MCValue.cpp - MCValue implementation ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCValue.h"
#include "llvm_include_llvm_Config_llvm-config.h"
#include "llvm_include_llvm_MC_MCExpr.h"
#include "llvm_include_llvm_Support_Debug.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace llvm;

void MCValue::print(raw_ostream &OS) const {
  if (isAbsolute()) {
    OS << getConstant();
    return;
  }

  // FIXME: prints as a number, which isn't ideal. But the meaning will be
  // target-specific anyway.
  if (getRefKind())
    OS << ':' << getRefKind() <<  ':';

  OS << *getSymA();

  if (getSymB()) {
    OS << " - ";
    OS << *getSymB();
  }

  if (getConstant())
    OS << " + " << getConstant();
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void MCValue::dump() const {
  print(dbgs());
}
#endif

MCSymbolRefExpr::VariantKind MCValue::getAccessVariant() const {
  const MCSymbolRefExpr *B = getSymB();
  if (B) {
    if (B->getKind() != MCSymbolRefExpr::VK_None)
      llvm_unreachable("unsupported");
  }

  const MCSymbolRefExpr *A = getSymA();
  if (!A)
    return MCSymbolRefExpr::VK_None;

  return A->getKind();
}
