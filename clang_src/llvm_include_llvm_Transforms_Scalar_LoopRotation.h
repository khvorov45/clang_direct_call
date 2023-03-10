//===- LoopRotation.h - Loop Rotation -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides the interface for the Loop Rotation pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_LOOPROTATION_H
#define LLVM_TRANSFORMS_SCALAR_LOOPROTATION_H

#include "llvm_include_llvm_Analysis_LoopAnalysisManager.h"
#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {
class LPMUpdater;
class Loop;

/// A simple loop rotation transformation.
class LoopRotatePass : public PassInfoMixin<LoopRotatePass> {
public:
  LoopRotatePass(bool EnableHeaderDuplication = true,
                 bool PrepareForLTO = false);
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);

private:
  const bool EnableHeaderDuplication;
  const bool PrepareForLTO;
};
}

#endif // LLVM_TRANSFORMS_SCALAR_LOOPROTATION_H
