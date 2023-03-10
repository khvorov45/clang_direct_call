//===- SeparateConstOffsetFromGEP.h ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_SEPARATECONSTOFFSETFROMGEP_H
#define LLVM_TRANSFORMS_SCALAR_SEPARATECONSTOFFSETFROMGEP_H

#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {

class SeparateConstOffsetFromGEPPass
    : public PassInfoMixin<SeparateConstOffsetFromGEPPass> {
  bool LowerGEP;

public:
  SeparateConstOffsetFromGEPPass(bool LowerGEP = false) : LowerGEP(LowerGEP) {}
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_SEPARATECONSTOFFSETFROMGEP_H
