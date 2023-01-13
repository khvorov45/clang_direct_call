//===- InstCount.h - Collects the count of all instructions -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass collects the count of all instructions and reports them
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_INSTCOUNT_H
#define LLVM_ANALYSIS_INSTCOUNT_H

#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {

class Function;

struct InstCountPass : PassInfoMixin<InstCountPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &);
};

} // end namespace llvm

#endif // LLVM_ANALYSIS_INSTCOUNT_H
