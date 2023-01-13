//===-- KCFI.cpp - Generic KCFI operand bundle lowering ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass emits generic KCFI indirect call checks for targets that don't
// support lowering KCFI operand bundles in the back-end.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Instrumentation_KCFI.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_IR_Constants.h"
#include "llvm_include_llvm_IR_DiagnosticInfo.h"
#include "llvm_include_llvm_IR_DiagnosticPrinter.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_GlobalObject.h"
#include "llvm_include_llvm_IR_IRBuilder.h"
#include "llvm_include_llvm_IR_InstIterator.h"
#include "llvm_include_llvm_IR_Instructions.h"
#include "llvm_include_llvm_IR_Intrinsics.h"
#include "llvm_include_llvm_IR_MDBuilder.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Target_TargetMachine.h"
#include "llvm_include_llvm_Transforms_Instrumentation.h"
#include "llvm_include_llvm_Transforms_Utils_BasicBlockUtils.h"

using namespace llvm;

#define DEBUG_TYPE "kcfi"

STATISTIC(NumKCFIChecks, "Number of kcfi operands transformed into checks");

namespace {
class DiagnosticInfoKCFI : public DiagnosticInfo {
  const Twine &Msg;

public:
  DiagnosticInfoKCFI(const Twine &DiagMsg,
                     DiagnosticSeverity Severity = DS_Error)
      : DiagnosticInfo(DK_Linker, Severity), Msg(DiagMsg) {}
  void print(DiagnosticPrinter &DP) const override { DP << Msg; }
};
} // namespace

PreservedAnalyses KCFIPass::run(Function &F, FunctionAnalysisManager &AM) {
  Module &M = *F.getParent();
  if (!M.getModuleFlag("kcfi"))
    return PreservedAnalyses::all();

  // Find call instructions with KCFI operand bundles.
  SmallVector<CallInst *> KCFICalls;
  for (Instruction &I : instructions(F)) {
    if (auto *CI = dyn_cast<CallInst>(&I))
      if (CI->getOperandBundle(LLVMContext::OB_kcfi))
        KCFICalls.push_back(CI);
  }

  if (KCFICalls.empty())
    return PreservedAnalyses::all();

  LLVMContext &Ctx = M.getContext();
  // patchable-function-prefix emits nops between the KCFI type identifier
  // and the function start. As we don't know the size of the emitted nops,
  // don't allow this attribute with generic lowering.
  if (F.hasFnAttribute("patchable-function-prefix"))
    Ctx.diagnose(
        DiagnosticInfoKCFI("-fpatchable-function-entry=N,M, where M>0 is not "
                           "compatible with -fsanitize=kcfi on this target"));

  IntegerType *Int32Ty = Type::getInt32Ty(Ctx);
  MDNode *VeryUnlikelyWeights =
      MDBuilder(Ctx).createBranchWeights(1, (1U << 20) - 1);

  for (CallInst *CI : KCFICalls) {
    // Get the expected hash value.
    const uint32_t ExpectedHash =
        cast<ConstantInt>(CI->getOperandBundle(LLVMContext::OB_kcfi)->Inputs[0])
            ->getZExtValue();

    // Drop the KCFI operand bundle.
    CallBase *Call =
        CallBase::removeOperandBundle(CI, LLVMContext::OB_kcfi, CI);
    assert(Call != CI);
    Call->copyMetadata(*CI);
    CI->replaceAllUsesWith(Call);
    CI->eraseFromParent();

    if (!Call->isIndirectCall())
      continue;

    // Emit a check and trap if the target hash doesn't match.
    IRBuilder<> Builder(Call);
    Value *HashPtr = Builder.CreateConstInBoundsGEP1_32(
        Int32Ty, Call->getCalledOperand(), -1);
    Value *Test = Builder.CreateICmpNE(Builder.CreateLoad(Int32Ty, HashPtr),
                                       ConstantInt::get(Int32Ty, ExpectedHash));
    Instruction *ThenTerm =
        SplitBlockAndInsertIfThen(Test, Call, false, VeryUnlikelyWeights);
    Builder.SetInsertPoint(ThenTerm);
    Builder.CreateCall(Intrinsic::getDeclaration(&M, Intrinsic::trap));
    ++NumKCFIChecks;
  }

  return PreservedAnalyses::none();
}
