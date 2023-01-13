//===- LoopFlatten.h - Loop Flatten ----------------  -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides the interface for the Loop Flatten Pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_LOOPFLATTEN_H
#define LLVM_TRANSFORMS_SCALAR_LOOPFLATTEN_H

#include "llvm_include_llvm_Analysis_LoopAnalysisManager.h"
#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {
class LPMUpdater;
class LoopNest;

class LoopFlattenPass : public PassInfoMixin<LoopFlattenPass> {
public:
  LoopFlattenPass() = default;

  PreservedAnalyses run(LoopNest &LN, LoopAnalysisManager &LAM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_LOOPFLATTEN_H
