//===- CFGSCCPrinter.cpp --------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Analysis_CFGSCCPrinter.h"
#include "llvm_include_llvm_ADT_SCCIterator.h"
#include "llvm_include_llvm_IR_CFG.h"

using namespace llvm;

PreservedAnalyses CFGSCCPrinterPass::run(Function &F,
                                         FunctionAnalysisManager &AM) {
  unsigned SccNum = 0;
  OS << "SCCs for Function " << F.getName() << " in PostOrder:";
  for (scc_iterator<Function *> SCCI = scc_begin(&F); !SCCI.isAtEnd(); ++SCCI) {
    const std::vector<BasicBlock *> &NextSCC = *SCCI;
    OS << "\nSCC #" << ++SccNum << ": ";
    bool First = true;
    for (BasicBlock *BB : NextSCC) {
      if (First)
        First = false;
      else
        OS << ", ";
      BB->printAsOperand(OS, false);
    }
    if (NextSCC.size() == 1 && SCCI.hasCycle())
      OS << " (Has self-loop).";
  }
  OS << "\n";

  return PreservedAnalyses::all();
}
