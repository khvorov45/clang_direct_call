//===- InstructionNamer.cpp - Give anonymous instructions names -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This is a little utility pass that gives instructions names, this is mostly
// useful when diffing the effect of an optimization because deleting an
// unnamed instruction can change all other instruction numbering, making the
// diff very noisy.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Utils_InstructionNamer.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_PassManager.h"
#include "llvm_include_llvm_IR_Type.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Transforms_Utils.h"

using namespace llvm;

namespace {
void nameInstructions(Function &F) {
  for (auto &Arg : F.args()) {
    if (!Arg.hasName())
      Arg.setName("arg");
  }

  for (BasicBlock &BB : F) {
    if (!BB.hasName())
      BB.setName("bb");

    for (Instruction &I : BB) {
      if (!I.hasName() && !I.getType()->isVoidTy())
        I.setName("i");
    }
  }
}

struct InstNamer : public FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  InstNamer() : FunctionPass(ID) {
    initializeInstNamerPass(*PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &Info) const override {
    Info.setPreservesAll();
  }

  bool runOnFunction(Function &F) override {
    nameInstructions(F);
    return true;
  }
};

  char InstNamer::ID = 0;
  } // namespace

INITIALIZE_PASS(InstNamer, "instnamer",
                "Assign names to anonymous instructions", false, false)
char &llvm::InstructionNamerID = InstNamer::ID;
//===----------------------------------------------------------------------===//
//
// InstructionNamer - Give any unnamed non-void instructions "tmp" names.
//
FunctionPass *llvm::createInstructionNamerPass() {
  return new InstNamer();
}

PreservedAnalyses InstructionNamerPass::run(Function &F,
                                            FunctionAnalysisManager &FAM) {
  nameInstructions(F);
  return PreservedAnalyses::all();
}
