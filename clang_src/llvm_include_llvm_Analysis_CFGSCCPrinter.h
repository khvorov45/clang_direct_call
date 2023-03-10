//===-- CFGSCCPrinter.h ---------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_CFGSCCPRINTER_H
#define LLVM_ANALYSIS_CFGSCCPRINTER_H

#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {

class CFGSCCPrinterPass : public PassInfoMixin<CFGSCCPrinterPass> {
  raw_ostream &OS;

public:
  explicit CFGSCCPrinterPass(raw_ostream &OS) : OS(OS) {}
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};
} // namespace llvm

#endif
