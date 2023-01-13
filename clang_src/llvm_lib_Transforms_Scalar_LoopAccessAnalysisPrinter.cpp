//===- LoopAccessAnalysisPrinter.cpp - Loop Access Analysis Printer --------==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Scalar_LoopAccessAnalysisPrinter.h"
#include "llvm_include_llvm_ADT_PriorityWorklist.h"
#include "llvm_include_llvm_Analysis_LoopAccessAnalysis.h"
#include "llvm_include_llvm_Analysis_LoopInfo.h"
#include "llvm_include_llvm_Transforms_Utils_LoopUtils.h"

using namespace llvm;

#define DEBUG_TYPE "loop-accesses"

PreservedAnalyses LoopAccessInfoPrinterPass::run(Function &F,
                                                 FunctionAnalysisManager &AM) {
  auto &LAIs = AM.getResult<LoopAccessAnalysis>(F);
  auto &LI = AM.getResult<LoopAnalysis>(F);
  OS << "Loop access info in function '" << F.getName() << "':\n";

  SmallPriorityWorklist<Loop *, 4> Worklist;
  appendLoopsToWorklist(LI, Worklist);
  while (!Worklist.empty()) {
    Loop *L = Worklist.pop_back_val();
    OS.indent(2) << L->getHeader()->getName() << ":\n";
    LAIs.getInfo(*L).print(OS, 4);
  }
  return PreservedAnalyses::all();
}
