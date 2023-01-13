//===- ElimAvailExtern.cpp - DCE unreachable internal functions -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This transform is designed to eliminate available external global
// definitions from the program, turning them into declarations.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_IPO_ElimAvailExtern.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_IR_Constant.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_GlobalValue.h"
#include "llvm_include_llvm_IR_GlobalVariable.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Transforms_IPO.h"
#include "llvm_include_llvm_Transforms_Utils_GlobalStatus.h"

using namespace llvm;

#define DEBUG_TYPE "elim-avail-extern"

STATISTIC(NumFunctions, "Number of functions removed");
STATISTIC(NumVariables, "Number of global variables removed");

static bool eliminateAvailableExternally(Module &M) {
  bool Changed = false;

  // Drop initializers of available externally global variables.
  for (GlobalVariable &GV : M.globals()) {
    if (!GV.hasAvailableExternallyLinkage())
      continue;
    if (GV.hasInitializer()) {
      Constant *Init = GV.getInitializer();
      GV.setInitializer(nullptr);
      if (isSafeToDestroyConstant(Init))
        Init->destroyConstant();
    }
    GV.removeDeadConstantUsers();
    GV.setLinkage(GlobalValue::ExternalLinkage);
    NumVariables++;
    Changed = true;
  }

  // Drop the bodies of available externally functions.
  for (Function &F : M) {
    if (!F.hasAvailableExternallyLinkage())
      continue;
    if (!F.isDeclaration())
      // This will set the linkage to external
      F.deleteBody();
    F.removeDeadConstantUsers();
    NumFunctions++;
    Changed = true;
  }

  return Changed;
}

PreservedAnalyses
EliminateAvailableExternallyPass::run(Module &M, ModuleAnalysisManager &) {
  if (!eliminateAvailableExternally(M))
    return PreservedAnalyses::all();
  return PreservedAnalyses::none();
}

namespace {

struct EliminateAvailableExternallyLegacyPass : public ModulePass {
  static char ID; // Pass identification, replacement for typeid

  EliminateAvailableExternallyLegacyPass() : ModulePass(ID) {
    initializeEliminateAvailableExternallyLegacyPassPass(
        *PassRegistry::getPassRegistry());
  }

  // run - Do the EliminateAvailableExternally pass on the specified module,
  // optionally updating the specified callgraph to reflect the changes.
  bool runOnModule(Module &M) override {
    if (skipModule(M))
      return false;
    return eliminateAvailableExternally(M);
  }
};

} // end anonymous namespace

char EliminateAvailableExternallyLegacyPass::ID = 0;

INITIALIZE_PASS(EliminateAvailableExternallyLegacyPass, "elim-avail-extern",
                "Eliminate Available Externally Globals", false, false)

ModulePass *llvm::createEliminateAvailableExternallyPass() {
  return new EliminateAvailableExternallyLegacyPass();
}
