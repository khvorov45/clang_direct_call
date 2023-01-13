//===- LoopUnrollAndJamPass.h -----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_LOOPUNROLLANDJAMPASS_H
#define LLVM_TRANSFORMS_SCALAR_LOOPUNROLLANDJAMPASS_H

#include "llvm_include_llvm_Analysis_LoopAnalysisManager.h"
#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {
class LPMUpdater;
class LoopNest;

/// A simple loop rotation transformation.
class LoopUnrollAndJamPass : public PassInfoMixin<LoopUnrollAndJamPass> {
  const int OptLevel;

public:
  explicit LoopUnrollAndJamPass(int OptLevel = 2) : OptLevel(OptLevel) {}
  PreservedAnalyses run(LoopNest &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_LOOPUNROLLANDJAMPASS_H
