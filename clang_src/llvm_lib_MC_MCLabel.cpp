//===- lib/MC/MCLabel.cpp - MCLabel implementation ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCLabel.h"
#include "llvm_include_llvm_Config_llvm-config.h"
#include "llvm_include_llvm_Support_Compiler.h"
#include "llvm_include_llvm_Support_Debug.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace llvm;

void MCLabel::print(raw_ostream &OS) const {
  OS << '"' << getInstance() << '"';
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void MCLabel::dump() const {
  print(dbgs());
}
#endif
