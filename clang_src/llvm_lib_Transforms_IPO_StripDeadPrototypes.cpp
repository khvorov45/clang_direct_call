//===-- StripDeadPrototypes.cpp - Remove unused function declarations ----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass loops over all of the functions in the input module, looking for
// dead declarations and removes them. Dead declarations are declarations of
// functions for which no implementation is available (i.e., declarations for
// unused library functions).
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_IPO_StripDeadPrototypes.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Transforms_IPO.h"

using namespace llvm;

#define DEBUG_TYPE "strip-dead-prototypes"

STATISTIC(NumDeadPrototypes, "Number of dead prototypes removed");

static bool stripDeadPrototypes(Module &M) {
  bool MadeChange = false;

  // Erase dead function prototypes.
  for (Function &F : llvm::make_early_inc_range(M)) {
    // Function must be a prototype and unused.
    if (F.isDeclaration() && F.use_empty()) {
      F.eraseFromParent();
      ++NumDeadPrototypes;
      MadeChange = true;
    }
  }

  // Erase dead global var prototypes.
  for (GlobalVariable &GV : llvm::make_early_inc_range(M.globals())) {
    // Global must be a prototype and unused.
    if (GV.isDeclaration() && GV.use_empty())
      GV.eraseFromParent();
  }

  // Return an indication of whether we changed anything or not.
  return MadeChange;
}

PreservedAnalyses StripDeadPrototypesPass::run(Module &M,
                                               ModuleAnalysisManager &) {
  if (stripDeadPrototypes(M))
    return PreservedAnalyses::none();
  return PreservedAnalyses::all();
}

namespace {

class StripDeadPrototypesLegacyPass : public ModulePass {
public:
  static char ID; // Pass identification, replacement for typeid
  StripDeadPrototypesLegacyPass() : ModulePass(ID) {
    initializeStripDeadPrototypesLegacyPassPass(
        *PassRegistry::getPassRegistry());
  }
  bool runOnModule(Module &M) override {
    if (skipModule(M))
      return false;

    return stripDeadPrototypes(M);
  }
};

} // end anonymous namespace

char StripDeadPrototypesLegacyPass::ID = 0;
INITIALIZE_PASS(StripDeadPrototypesLegacyPass, "strip-dead-prototypes",
                "Strip Unused Function Prototypes", false, false)

ModulePass *llvm::createStripDeadPrototypesPass() {
  return new StripDeadPrototypesLegacyPass();
}
