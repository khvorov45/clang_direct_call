//===----- ExpandReductions.h - Expand experimental reduction intrinsics --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_EXPANDREDUCTIONS_H
#define LLVM_CODEGEN_EXPANDREDUCTIONS_H

#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {

class ExpandReductionsPass
    : public PassInfoMixin<ExpandReductionsPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};
} // end namespace llvm

#endif // LLVM_CODEGEN_EXPANDREDUCTIONS_H
