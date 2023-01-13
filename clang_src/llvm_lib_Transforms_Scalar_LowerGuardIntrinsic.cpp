//===- LowerGuardIntrinsic.cpp - Lower the guard intrinsic ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass lowers the llvm.experimental.guard intrinsic to a conditional call
// to @llvm.experimental.deoptimize.  Once this happens, the guard can no longer
// be widened.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Scalar_LowerGuardIntrinsic.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_Analysis_GuardUtils.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_InstIterator.h"
#include "llvm_include_llvm_IR_Instructions.h"
#include "llvm_include_llvm_IR_Intrinsics.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Transforms_Scalar.h"
#include "llvm_include_llvm_Transforms_Utils_GuardUtils.h"

using namespace llvm;

namespace {
struct LowerGuardIntrinsicLegacyPass : public FunctionPass {
  static char ID;
  LowerGuardIntrinsicLegacyPass() : FunctionPass(ID) {
    initializeLowerGuardIntrinsicLegacyPassPass(
        *PassRegistry::getPassRegistry());
  }

  bool runOnFunction(Function &F) override;
};
}

static bool lowerGuardIntrinsic(Function &F) {
  // Check if we can cheaply rule out the possibility of not having any work to
  // do.
  auto *GuardDecl = F.getParent()->getFunction(
      Intrinsic::getName(Intrinsic::experimental_guard));
  if (!GuardDecl || GuardDecl->use_empty())
    return false;

  SmallVector<CallInst *, 8> ToLower;
  // Traverse through the users of GuardDecl.
  // This is presumably cheaper than traversing all instructions in the
  // function.
  for (auto *U : GuardDecl->users())
    if (auto *CI = dyn_cast<CallInst>(U))
      if (CI->getFunction() == &F)
        ToLower.push_back(CI);

  if (ToLower.empty())
    return false;

  auto *DeoptIntrinsic = Intrinsic::getDeclaration(
      F.getParent(), Intrinsic::experimental_deoptimize, {F.getReturnType()});
  DeoptIntrinsic->setCallingConv(GuardDecl->getCallingConv());

  for (auto *CI : ToLower) {
    makeGuardControlFlowExplicit(DeoptIntrinsic, CI, false);
    CI->eraseFromParent();
  }

  return true;
}

bool LowerGuardIntrinsicLegacyPass::runOnFunction(Function &F) {
  return lowerGuardIntrinsic(F);
}

char LowerGuardIntrinsicLegacyPass::ID = 0;
INITIALIZE_PASS(LowerGuardIntrinsicLegacyPass, "lower-guard-intrinsic",
                "Lower the guard intrinsic to normal control flow", false,
                false)

Pass *llvm::createLowerGuardIntrinsicPass() {
  return new LowerGuardIntrinsicLegacyPass();
}

PreservedAnalyses LowerGuardIntrinsicPass::run(Function &F,
                                               FunctionAnalysisManager &AM) {
  if (lowerGuardIntrinsic(F))
    return PreservedAnalyses::none();

  return PreservedAnalyses::all();
}
