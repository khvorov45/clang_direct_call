//===- LowerConstantIntrinsics.cpp - Lower constant intrinsic calls -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass lowers all remaining 'objectsize' 'is.constant' intrinsic calls
// and provides constant propagation and basic CFG cleanup on the result.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Scalar_LowerConstantIntrinsics.h"
#include "llvm_include_llvm_ADT_PostOrderIterator.h"
#include "llvm_include_llvm_ADT_SetVector.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_Analysis_DomTreeUpdater.h"
#include "llvm_include_llvm_Analysis_GlobalsModRef.h"
#include "llvm_include_llvm_Analysis_InstructionSimplify.h"
#include "llvm_include_llvm_Analysis_MemoryBuiltins.h"
#include "llvm_include_llvm_Analysis_TargetLibraryInfo.h"
#include "llvm_include_llvm_IR_BasicBlock.h"
#include "llvm_include_llvm_IR_Constants.h"
#include "llvm_include_llvm_IR_Dominators.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_Instructions.h"
#include "llvm_include_llvm_IR_IntrinsicInst.h"
#include "llvm_include_llvm_IR_PatternMatch.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Transforms_Scalar.h"
#include "llvm_include_llvm_Transforms_Utils_Local.h"
#include <optional>

using namespace llvm;
using namespace llvm::PatternMatch;

#define DEBUG_TYPE "lower-is-constant-intrinsic"

STATISTIC(IsConstantIntrinsicsHandled,
          "Number of 'is.constant' intrinsic calls handled");
STATISTIC(ObjectSizeIntrinsicsHandled,
          "Number of 'objectsize' intrinsic calls handled");

static Value *lowerIsConstantIntrinsic(IntrinsicInst *II) {
  if (auto *C = dyn_cast<Constant>(II->getOperand(0)))
    if (C->isManifestConstant())
      return ConstantInt::getTrue(II->getType());
  return ConstantInt::getFalse(II->getType());
}

static bool replaceConditionalBranchesOnConstant(Instruction *II,
                                                 Value *NewValue,
                                                 DomTreeUpdater *DTU) {
  bool HasDeadBlocks = false;
  SmallSetVector<Instruction *, 8> UnsimplifiedUsers;
  replaceAndRecursivelySimplify(II, NewValue, nullptr, nullptr, nullptr,
                                &UnsimplifiedUsers);
  // UnsimplifiedUsers can contain PHI nodes that may be removed when
  // replacing the branch instructions, so use a value handle worklist
  // to handle those possibly removed instructions.
  SmallVector<WeakVH, 8> Worklist(UnsimplifiedUsers.begin(),
                                  UnsimplifiedUsers.end());

  for (auto &VH : Worklist) {
    BranchInst *BI = dyn_cast_or_null<BranchInst>(VH);
    if (!BI)
      continue;
    if (BI->isUnconditional())
      continue;

    BasicBlock *Target, *Other;
    if (match(BI->getOperand(0), m_Zero())) {
      Target = BI->getSuccessor(1);
      Other = BI->getSuccessor(0);
    } else if (match(BI->getOperand(0), m_One())) {
      Target = BI->getSuccessor(0);
      Other = BI->getSuccessor(1);
    } else {
      Target = nullptr;
      Other = nullptr;
    }
    if (Target && Target != Other) {
      BasicBlock *Source = BI->getParent();
      Other->removePredecessor(Source);
      BI->eraseFromParent();
      BranchInst::Create(Target, Source);
      if (DTU)
        DTU->applyUpdates({{DominatorTree::Delete, Source, Other}});
      if (pred_empty(Other))
        HasDeadBlocks = true;
    }
  }
  return HasDeadBlocks;
}

static bool lowerConstantIntrinsics(Function &F, const TargetLibraryInfo &TLI,
                                    DominatorTree *DT) {
  std::optional<DomTreeUpdater> DTU;
  if (DT)
    DTU.emplace(DT, DomTreeUpdater::UpdateStrategy::Lazy);

  bool HasDeadBlocks = false;
  const auto &DL = F.getParent()->getDataLayout();
  SmallVector<WeakTrackingVH, 8> Worklist;

  ReversePostOrderTraversal<Function *> RPOT(&F);
  for (BasicBlock *BB : RPOT) {
    for (Instruction &I: *BB) {
      IntrinsicInst *II = dyn_cast<IntrinsicInst>(&I);
      if (!II)
        continue;
      switch (II->getIntrinsicID()) {
      default:
        break;
      case Intrinsic::is_constant:
      case Intrinsic::objectsize:
        Worklist.push_back(WeakTrackingVH(&I));
        break;
      }
    }
  }
  for (WeakTrackingVH &VH: Worklist) {
    // Items on the worklist can be mutated by earlier recursive replaces.
    // This can remove the intrinsic as dead (VH == null), but also replace
    // the intrinsic in place.
    if (!VH)
      continue;
    IntrinsicInst *II = dyn_cast<IntrinsicInst>(&*VH);
    if (!II)
      continue;
    Value *NewValue;
    switch (II->getIntrinsicID()) {
    default:
      continue;
    case Intrinsic::is_constant:
      NewValue = lowerIsConstantIntrinsic(II);
      IsConstantIntrinsicsHandled++;
      break;
    case Intrinsic::objectsize:
      NewValue = lowerObjectSizeCall(II, DL, &TLI, true);
      ObjectSizeIntrinsicsHandled++;
      break;
    }
    HasDeadBlocks |= replaceConditionalBranchesOnConstant(
        II, NewValue, DTU ? &*DTU : nullptr);
  }
  if (HasDeadBlocks)
    removeUnreachableBlocks(F, DTU ? &*DTU : nullptr);
  return !Worklist.empty();
}

PreservedAnalyses
LowerConstantIntrinsicsPass::run(Function &F, FunctionAnalysisManager &AM) {
  if (lowerConstantIntrinsics(F, AM.getResult<TargetLibraryAnalysis>(F),
                              AM.getCachedResult<DominatorTreeAnalysis>(F))) {
    PreservedAnalyses PA;
    PA.preserve<DominatorTreeAnalysis>();
    return PA;
  }

  return PreservedAnalyses::all();
}

namespace {
/// Legacy pass for lowering is.constant intrinsics out of the IR.
///
/// When this pass is run over a function it converts is.constant intrinsics
/// into 'true' or 'false'. This complements the normal constant folding
/// to 'true' as part of Instruction Simplify passes.
class LowerConstantIntrinsics : public FunctionPass {
public:
  static char ID;
  LowerConstantIntrinsics() : FunctionPass(ID) {
    initializeLowerConstantIntrinsicsPass(*PassRegistry::getPassRegistry());
  }

  bool runOnFunction(Function &F) override {
    const TargetLibraryInfo &TLI =
        getAnalysis<TargetLibraryInfoWrapperPass>().getTLI(F);
    DominatorTree *DT = nullptr;
    if (auto *DTWP = getAnalysisIfAvailable<DominatorTreeWrapperPass>())
      DT = &DTWP->getDomTree();
    return lowerConstantIntrinsics(F, TLI, DT);
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<TargetLibraryInfoWrapperPass>();
    AU.addPreserved<GlobalsAAWrapperPass>();
    AU.addPreserved<DominatorTreeWrapperPass>();
  }
};
} // namespace

char LowerConstantIntrinsics::ID = 0;
INITIALIZE_PASS_BEGIN(LowerConstantIntrinsics, "lower-constant-intrinsics",
                      "Lower constant intrinsics", false, false)
INITIALIZE_PASS_DEPENDENCY(TargetLibraryInfoWrapperPass)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_END(LowerConstantIntrinsics, "lower-constant-intrinsics",
                    "Lower constant intrinsics", false, false)

FunctionPass *llvm::createLowerConstantIntrinsicsPass() {
  return new LowerConstantIntrinsics();
}
