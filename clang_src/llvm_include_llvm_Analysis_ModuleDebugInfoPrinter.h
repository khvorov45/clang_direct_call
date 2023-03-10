//===- ModuleDebugInfoPrinter.h - -----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_MODULEDEBUGINFOPRINTER_H
#define LLVM_ANALYSIS_MODULEDEBUGINFOPRINTER_H

#include "llvm_include_llvm_IR_DebugInfo.h"
#include "llvm_include_llvm_IR_PassManager.h"

namespace llvm {
class raw_ostream;

class ModuleDebugInfoPrinterPass
    : public PassInfoMixin<ModuleDebugInfoPrinterPass> {
  DebugInfoFinder Finder;
  raw_ostream &OS;

public:
  explicit ModuleDebugInfoPrinterPass(raw_ostream &OS);
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};
} // end namespace llvm

#endif // LLVM_ANALYSIS_MODULEDEBUGINFOPRINTER_H
