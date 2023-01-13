//===- StructurizeCFG.h ---------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_STRUCTURIZECFG_H
#define LLVM_TRANSFORMS_SCALAR_STRUCTURIZECFG_H

#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {
struct StructurizeCFGPass : PassInfoMixin<StructurizeCFGPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};
} // namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_STRUCTURIZECFG_H
