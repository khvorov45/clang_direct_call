//===-- ExtractGV.h -------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_IPO_EXTRACTGV_H
#define LLVM_TRANSFORMS_IPO_EXTRACTGV_H

#include "llvm_include_llvm_ADT_SetVector.h"
#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {

class ExtractGVPass : public PassInfoMixin<ExtractGVPass> {
private:
  SetVector<GlobalValue *> Named;
  bool deleteStuff;
  bool keepConstInit;

public:
  ExtractGVPass(std::vector<GlobalValue *> &GVs, bool deleteS = true,
                bool keepConstInit = false);
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &);
};
} // namespace llvm

#endif // LLVM_TRANSFORMS_IPO_EXTRACTGV_H
