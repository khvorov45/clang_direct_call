//===- LoopDeletion.h - Loop Deletion ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides the interface for the Loop Deletion Pass.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_LOOPDELETION_H
#define LLVM_TRANSFORMS_SCALAR_LOOPDELETION_H

#include "llvm_include_llvm_Analysis_LoopAnalysisManager.h"
#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {

class Loop;
class LPMUpdater;

class LoopDeletionPass : public PassInfoMixin<LoopDeletionPass> {
public:
  LoopDeletionPass() = default;

  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_LOOPDELETION_H
