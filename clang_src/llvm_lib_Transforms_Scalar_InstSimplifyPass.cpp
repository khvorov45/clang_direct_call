//===- InstSimplifyPass.cpp -----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Scalar_InstSimplifyPass.h"
#include "llvm_include_llvm_ADT_SmallPtrSet.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_Analysis_AssumptionCache.h"
#include "llvm_include_llvm_Analysis_InstructionSimplify.h"
#include "llvm_include_llvm_Analysis_OptimizationRemarkEmitter.h"
#include "llvm_include_llvm_Analysis_TargetLibraryInfo.h"
#include "llvm_include_llvm_IR_Dominators.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Transforms_Scalar.h"
#include "llvm_include_llvm_Transforms_Utils_Local.h"

using namespace llvm;

#define DEBUG_TYPE "instsimplify"

STATISTIC(NumSimplified, "Number of redundant instructions removed");

static bool runImpl(Function &F, const SimplifyQuery &SQ,
                    OptimizationRemarkEmitter *ORE) {
  SmallPtrSet<const Instruction *, 8> S1, S2, *ToSimplify = &S1, *Next = &S2;
  bool Changed = false;

  do {
    for (BasicBlock &BB : F) {
      // Unreachable code can take on strange forms that we are not prepared to
      // handle. For example, an instruction may have itself as an operand.
      if (!SQ.DT->isReachableFromEntry(&BB))
        continue;

      SmallVector<WeakTrackingVH, 8> DeadInstsInBB;
      for (Instruction &I : BB) {
        // The first time through the loop, ToSimplify is empty and we try to
        // simplify all instructions. On later iterations, ToSimplify is not
        // empty and we only bother simplifying instructions that are in it.
        if (!ToSimplify->empty() && !ToSimplify->count(&I))
          continue;

        // Don't waste time simplifying dead/unused instructions.
        if (isInstructionTriviallyDead(&I)) {
          DeadInstsInBB.push_back(&I);
          Changed = true;
        } else if (!I.use_empty()) {
          if (Value *V = simplifyInstruction(&I, SQ, ORE)) {
            // Mark all uses for resimplification next time round the loop.
            for (User *U : I.users())
              Next->insert(cast<Instruction>(U));
            I.replaceAllUsesWith(V);
            ++NumSimplified;
            Changed = true;
            // A call can get simplified, but it may not be trivially dead.
            if (isInstructionTriviallyDead(&I))
              DeadInstsInBB.push_back(&I);
          }
        }
      }
      RecursivelyDeleteTriviallyDeadInstructions(DeadInstsInBB, SQ.TLI);
    }

    // Place the list of instructions to simplify on the next loop iteration
    // into ToSimplify.
    std::swap(ToSimplify, Next);
    Next->clear();
  } while (!ToSimplify->empty());

  return Changed;
}

namespace {
struct InstSimplifyLegacyPass : public FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  InstSimplifyLegacyPass() : FunctionPass(ID) {
    initializeInstSimplifyLegacyPassPass(*PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    AU.addRequired<DominatorTreeWrapperPass>();
    AU.addRequired<AssumptionCacheTracker>();
    AU.addRequired<TargetLibraryInfoWrapperPass>();
    AU.addRequired<OptimizationRemarkEmitterWrapperPass>();
  }

  /// Remove instructions that simplify.
  bool runOnFunction(Function &F) override {
    if (skipFunction(F))
      return false;

    const DominatorTree *DT =
        &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
    const TargetLibraryInfo *TLI =
        &getAnalysis<TargetLibraryInfoWrapperPass>().getTLI(F);
    AssumptionCache *AC =
        &getAnalysis<AssumptionCacheTracker>().getAssumptionCache(F);
    OptimizationRemarkEmitter *ORE =
        &getAnalysis<OptimizationRemarkEmitterWrapperPass>().getORE();
    const DataLayout &DL = F.getParent()->getDataLayout();
    const SimplifyQuery SQ(DL, TLI, DT, AC);
    return runImpl(F, SQ, ORE);
  }
};
} // namespace

char InstSimplifyLegacyPass::ID = 0;
INITIALIZE_PASS_BEGIN(InstSimplifyLegacyPass, "instsimplify",
                      "Remove redundant instructions", false, false)
INITIALIZE_PASS_DEPENDENCY(AssumptionCacheTracker)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(TargetLibraryInfoWrapperPass)
INITIALIZE_PASS_DEPENDENCY(OptimizationRemarkEmitterWrapperPass)
INITIALIZE_PASS_END(InstSimplifyLegacyPass, "instsimplify",
                    "Remove redundant instructions", false, false)

// Public interface to the simplify instructions pass.
FunctionPass *llvm::createInstSimplifyLegacyPass() {
  return new InstSimplifyLegacyPass();
}

PreservedAnalyses InstSimplifyPass::run(Function &F,
                                        FunctionAnalysisManager &AM) {
  auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
  auto &TLI = AM.getResult<TargetLibraryAnalysis>(F);
  auto &AC = AM.getResult<AssumptionAnalysis>(F);
  auto &ORE = AM.getResult<OptimizationRemarkEmitterAnalysis>(F);
  const DataLayout &DL = F.getParent()->getDataLayout();
  const SimplifyQuery SQ(DL, &TLI, &DT, &AC);
  bool Changed = runImpl(F, SQ, &ORE);
  if (!Changed)
    return PreservedAnalyses::all();

  PreservedAnalyses PA;
  PA.preserveSet<CFGAnalyses>();
  return PA;
}
