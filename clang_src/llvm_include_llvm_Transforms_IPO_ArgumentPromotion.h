//===- ArgumentPromotion.h - Promote by-reference arguments -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_IPO_ARGUMENTPROMOTION_H
#define LLVM_TRANSFORMS_IPO_ARGUMENTPROMOTION_H

#include "llvm_include_llvm_Analysis_CGSCCPassManager.h"
#include "llvm_include_llvm_Analysis_LazyCallGraph.h"
#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {

/// Argument promotion pass.
///
/// This pass walks the functions in each SCC and for each one tries to
/// transform it and all of its callers to replace indirect arguments with
/// direct (by-value) arguments.
class ArgumentPromotionPass : public PassInfoMixin<ArgumentPromotionPass> {
  unsigned MaxElements;

public:
  ArgumentPromotionPass(unsigned MaxElements = 2u) : MaxElements(MaxElements) {}

  PreservedAnalyses run(LazyCallGraph::SCC &C, CGSCCAnalysisManager &AM,
                        LazyCallGraph &CG, CGSCCUpdateResult &UR);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_IPO_ARGUMENTPROMOTION_H
