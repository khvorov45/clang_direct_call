//===- StripNonLineTableDebugInfo.cpp -- Strip parts of Debug Info --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Utils_StripNonLineTableDebugInfo.h"
#include "llvm_include_llvm_IR_DebugInfo.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Transforms_Utils.h"
using namespace llvm;

namespace {

/// This pass strips all debug info that is not related line tables.
/// The result will be the same as if the program where compiled with
/// -gline-tables-only.
struct StripNonLineTableDebugLegacyPass : public ModulePass {
  static char ID; // Pass identification, replacement for typeid
  StripNonLineTableDebugLegacyPass() : ModulePass(ID) {
    initializeStripNonLineTableDebugLegacyPassPass(
        *PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnModule(Module &M) override {
    return llvm::stripNonLineTableDebugInfo(M);
  }
};
}

char StripNonLineTableDebugLegacyPass::ID = 0;
INITIALIZE_PASS(StripNonLineTableDebugLegacyPass,
                "strip-nonlinetable-debuginfo",
                "Strip all debug info except linetables", false, false)

ModulePass *llvm::createStripNonLineTableDebugLegacyPass() {
  return new StripNonLineTableDebugLegacyPass();
}

PreservedAnalyses
StripNonLineTableDebugInfoPass::run(Module &M, ModuleAnalysisManager &AM) {
  llvm::stripNonLineTableDebugInfo(M);
  return PreservedAnalyses::all();
}
