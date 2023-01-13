//===- DwarfEHPrepare - Prepare exception handling for code generation ----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass mulches exception handling code into a form adapted to code
// generation. Required if using dwarf exception handling.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_ADT_BitVector.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_ADT_Triple.h"
#include "llvm_include_llvm_Analysis_CFG.h"
#include "llvm_include_llvm_Analysis_DomTreeUpdater.h"
#include "llvm_include_llvm_Analysis_EHPersonalities.h"
#include "llvm_include_llvm_Analysis_TargetTransformInfo.h"
#include "llvm_include_llvm_CodeGen_RuntimeLibcalls.h"
#include "llvm_include_llvm_CodeGen_TargetLowering.h"
#include "llvm_include_llvm_CodeGen_TargetPassConfig.h"
#include "llvm_include_llvm_CodeGen_TargetSubtargetInfo.h"
#include "llvm_include_llvm_IR_BasicBlock.h"
#include "llvm_include_llvm_IR_Constants.h"
#include "llvm_include_llvm_IR_DebugInfoMetadata.h"
#include "llvm_include_llvm_IR_DerivedTypes.h"
#include "llvm_include_llvm_IR_Dominators.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_Instructions.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_IR_Type.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Support_Casting.h"
#include "llvm_include_llvm_Target_TargetMachine.h"
#include "llvm_include_llvm_Transforms_Utils_Local.h"
#include <cstddef>

using namespace llvm;

#define DEBUG_TYPE "dwarfehprepare"

STATISTIC(NumResumesLowered, "Number of resume calls lowered");
STATISTIC(NumCleanupLandingPadsUnreachable,
          "Number of cleanup landing pads found unreachable");
STATISTIC(NumCleanupLandingPadsRemaining,
          "Number of cleanup landing pads remaining");
STATISTIC(NumNoUnwind, "Number of functions with nounwind");
STATISTIC(NumUnwind, "Number of functions with unwind");

namespace {

class DwarfEHPrepare {
  CodeGenOpt::Level OptLevel;

  Function &F;
  const TargetLowering &TLI;
  DomTreeUpdater *DTU;
  const TargetTransformInfo *TTI;
  const Triple &TargetTriple;

  /// Return the exception object from the value passed into
  /// the 'resume' instruction (typically an aggregate). Clean up any dead
  /// instructions, including the 'resume' instruction.
  Value *GetExceptionObject(ResumeInst *RI);

  /// Replace resumes that are not reachable from a cleanup landing pad with
  /// unreachable and then simplify those blocks.
  size_t
  pruneUnreachableResumes(SmallVectorImpl<ResumeInst *> &Resumes,
                          SmallVectorImpl<LandingPadInst *> &CleanupLPads);

  /// Convert the ResumeInsts that are still present
  /// into calls to the appropriate _Unwind_Resume function.
  bool InsertUnwindResumeCalls();

public:
  DwarfEHPrepare(CodeGenOpt::Level OptLevel_, Function &F_,
                 const TargetLowering &TLI_, DomTreeUpdater *DTU_,
                 const TargetTransformInfo *TTI_, const Triple &TargetTriple_)
      : OptLevel(OptLevel_), F(F_), TLI(TLI_), DTU(DTU_), TTI(TTI_),
        TargetTriple(TargetTriple_) {}

  bool run();
};

} // namespace

Value *DwarfEHPrepare::GetExceptionObject(ResumeInst *RI) {
  Value *V = RI->getOperand(0);
  Value *ExnObj = nullptr;
  InsertValueInst *SelIVI = dyn_cast<InsertValueInst>(V);
  LoadInst *SelLoad = nullptr;
  InsertValueInst *ExcIVI = nullptr;
  bool EraseIVIs = false;

  if (SelIVI) {
    if (SelIVI->getNumIndices() == 1 && *SelIVI->idx_begin() == 1) {
      ExcIVI = dyn_cast<InsertValueInst>(SelIVI->getOperand(0));
      if (ExcIVI && isa<UndefValue>(ExcIVI->getOperand(0)) &&
          ExcIVI->getNumIndices() == 1 && *ExcIVI->idx_begin() == 0) {
        ExnObj = ExcIVI->getOperand(1);
        SelLoad = dyn_cast<LoadInst>(SelIVI->getOperand(1));
        EraseIVIs = true;
      }
    }
  }

  if (!ExnObj)
    ExnObj = ExtractValueInst::Create(RI->getOperand(0), 0, "exn.obj", RI);

  RI->eraseFromParent();

  if (EraseIVIs) {
    if (SelIVI->use_empty())
      SelIVI->eraseFromParent();
    if (ExcIVI->use_empty())
      ExcIVI->eraseFromParent();
    if (SelLoad && SelLoad->use_empty())
      SelLoad->eraseFromParent();
  }

  return ExnObj;
}

size_t DwarfEHPrepare::pruneUnreachableResumes(
    SmallVectorImpl<ResumeInst *> &Resumes,
    SmallVectorImpl<LandingPadInst *> &CleanupLPads) {
  assert(DTU && "Should have DomTreeUpdater here.");

  BitVector ResumeReachable(Resumes.size());
  size_t ResumeIndex = 0;
  for (auto *RI : Resumes) {
    for (auto *LP : CleanupLPads) {
      if (isPotentiallyReachable(LP, RI, nullptr, &DTU->getDomTree())) {
        ResumeReachable.set(ResumeIndex);
        break;
      }
    }
    ++ResumeIndex;
  }

  // If everything is reachable, there is no change.
  if (ResumeReachable.all())
    return Resumes.size();

  LLVMContext &Ctx = F.getContext();

  // Otherwise, insert unreachable instructions and call simplifycfg.
  size_t ResumesLeft = 0;
  for (size_t I = 0, E = Resumes.size(); I < E; ++I) {
    ResumeInst *RI = Resumes[I];
    if (ResumeReachable[I]) {
      Resumes[ResumesLeft++] = RI;
    } else {
      BasicBlock *BB = RI->getParent();
      new UnreachableInst(Ctx, RI);
      RI->eraseFromParent();
      simplifyCFG(BB, *TTI, DTU);
    }
  }
  Resumes.resize(ResumesLeft);
  return ResumesLeft;
}

bool DwarfEHPrepare::InsertUnwindResumeCalls() {
  SmallVector<ResumeInst *, 16> Resumes;
  SmallVector<LandingPadInst *, 16> CleanupLPads;
  if (F.doesNotThrow())
    NumNoUnwind++;
  else
    NumUnwind++;
  for (BasicBlock &BB : F) {
    if (auto *RI = dyn_cast<ResumeInst>(BB.getTerminator()))
      Resumes.push_back(RI);
    if (auto *LP = BB.getLandingPadInst())
      if (LP->isCleanup())
        CleanupLPads.push_back(LP);
  }

  NumCleanupLandingPadsRemaining += CleanupLPads.size();

  if (Resumes.empty())
    return false;

  // Check the personality, don't do anything if it's scope-based.
  EHPersonality Pers = classifyEHPersonality(F.getPersonalityFn());
  if (isScopedEHPersonality(Pers))
    return false;

  LLVMContext &Ctx = F.getContext();

  size_t ResumesLeft = Resumes.size();
  if (OptLevel != CodeGenOpt::None) {
    ResumesLeft = pruneUnreachableResumes(Resumes, CleanupLPads);
#if LLVM_ENABLE_STATS
    unsigned NumRemainingLPs = 0;
    for (BasicBlock &BB : F) {
      if (auto *LP = BB.getLandingPadInst())
        if (LP->isCleanup())
          NumRemainingLPs++;
    }
    NumCleanupLandingPadsUnreachable += CleanupLPads.size() - NumRemainingLPs;
    NumCleanupLandingPadsRemaining -= CleanupLPads.size() - NumRemainingLPs;
#endif
  }

  if (ResumesLeft == 0)
    return true; // We pruned them all.

  // RewindFunction - _Unwind_Resume or the target equivalent.
  FunctionCallee RewindFunction;
  CallingConv::ID RewindFunctionCallingConv;
  FunctionType *FTy;
  const char *RewindName;
  bool DoesRewindFunctionNeedExceptionObject;

  if ((Pers == EHPersonality::GNU_CXX || Pers == EHPersonality::GNU_CXX_SjLj) &&
      TargetTriple.isTargetEHABICompatible()) {
    RewindName = TLI.getLibcallName(RTLIB::CXA_END_CLEANUP);
    FTy = FunctionType::get(Type::getVoidTy(Ctx), false);
    RewindFunctionCallingConv =
        TLI.getLibcallCallingConv(RTLIB::CXA_END_CLEANUP);
    DoesRewindFunctionNeedExceptionObject = false;
  } else {
    RewindName = TLI.getLibcallName(RTLIB::UNWIND_RESUME);
    FTy =
        FunctionType::get(Type::getVoidTy(Ctx), Type::getInt8PtrTy(Ctx), false);
    RewindFunctionCallingConv = TLI.getLibcallCallingConv(RTLIB::UNWIND_RESUME);
    DoesRewindFunctionNeedExceptionObject = true;
  }
  RewindFunction = F.getParent()->getOrInsertFunction(RewindName, FTy);

  // Create the basic block where the _Unwind_Resume call will live.
  if (ResumesLeft == 1) {
    // Instead of creating a new BB and PHI node, just append the call to
    // _Unwind_Resume to the end of the single resume block.
    ResumeInst *RI = Resumes.front();
    BasicBlock *UnwindBB = RI->getParent();
    Value *ExnObj = GetExceptionObject(RI);
    llvm::SmallVector<Value *, 1> RewindFunctionArgs;
    if (DoesRewindFunctionNeedExceptionObject)
      RewindFunctionArgs.push_back(ExnObj);

    // Call the rewind function.
    CallInst *CI =
        CallInst::Create(RewindFunction, RewindFunctionArgs, "", UnwindBB);
    // The verifier requires that all calls of debug-info-bearing functions
    // from debug-info-bearing functions have a debug location (for inlining
    // purposes). Assign a dummy location to satisfy the constraint.
    Function *RewindFn = dyn_cast<Function>(RewindFunction.getCallee());
    if (RewindFn && RewindFn->getSubprogram())
      if (DISubprogram *SP = F.getSubprogram())
        CI->setDebugLoc(DILocation::get(SP->getContext(), 0, 0, SP));
    CI->setCallingConv(RewindFunctionCallingConv);

    // We never expect _Unwind_Resume to return.
    CI->setDoesNotReturn();
    new UnreachableInst(Ctx, UnwindBB);
    return true;
  }

  std::vector<DominatorTree::UpdateType> Updates;
  Updates.reserve(Resumes.size());

  llvm::SmallVector<Value *, 1> RewindFunctionArgs;

  BasicBlock *UnwindBB = BasicBlock::Create(Ctx, "unwind_resume", &F);
  PHINode *PN = PHINode::Create(Type::getInt8PtrTy(Ctx), ResumesLeft, "exn.obj",
                                UnwindBB);

  // Extract the exception object from the ResumeInst and add it to the PHI node
  // that feeds the _Unwind_Resume call.
  for (ResumeInst *RI : Resumes) {
    BasicBlock *Parent = RI->getParent();
    BranchInst::Create(UnwindBB, Parent);
    Updates.push_back({DominatorTree::Insert, Parent, UnwindBB});

    Value *ExnObj = GetExceptionObject(RI);
    PN->addIncoming(ExnObj, Parent);

    ++NumResumesLowered;
  }

  if (DoesRewindFunctionNeedExceptionObject)
    RewindFunctionArgs.push_back(PN);

  // Call the function.
  CallInst *CI =
      CallInst::Create(RewindFunction, RewindFunctionArgs, "", UnwindBB);
  CI->setCallingConv(RewindFunctionCallingConv);

  // We never expect _Unwind_Resume to return.
  CI->setDoesNotReturn();
  new UnreachableInst(Ctx, UnwindBB);

  if (DTU)
    DTU->applyUpdates(Updates);

  return true;
}

bool DwarfEHPrepare::run() {
  bool Changed = InsertUnwindResumeCalls();

  return Changed;
}

static bool prepareDwarfEH(CodeGenOpt::Level OptLevel, Function &F,
                           const TargetLowering &TLI, DominatorTree *DT,
                           const TargetTransformInfo *TTI,
                           const Triple &TargetTriple) {
  DomTreeUpdater DTU(DT, DomTreeUpdater::UpdateStrategy::Lazy);

  return DwarfEHPrepare(OptLevel, F, TLI, DT ? &DTU : nullptr, TTI,
                        TargetTriple)
      .run();
}

namespace {

class DwarfEHPrepareLegacyPass : public FunctionPass {

  CodeGenOpt::Level OptLevel;

public:
  static char ID; // Pass identification, replacement for typeid.

  DwarfEHPrepareLegacyPass(CodeGenOpt::Level OptLevel = CodeGenOpt::Default)
      : FunctionPass(ID), OptLevel(OptLevel) {}

  bool runOnFunction(Function &F) override {
    const TargetMachine &TM =
        getAnalysis<TargetPassConfig>().getTM<TargetMachine>();
    const TargetLowering &TLI = *TM.getSubtargetImpl(F)->getTargetLowering();
    DominatorTree *DT = nullptr;
    const TargetTransformInfo *TTI = nullptr;
    if (auto *DTWP = getAnalysisIfAvailable<DominatorTreeWrapperPass>())
      DT = &DTWP->getDomTree();
    if (OptLevel != CodeGenOpt::None) {
      if (!DT)
        DT = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      TTI = &getAnalysis<TargetTransformInfoWrapperPass>().getTTI(F);
    }
    return prepareDwarfEH(OptLevel, F, TLI, DT, TTI, TM.getTargetTriple());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<TargetPassConfig>();
    AU.addRequired<TargetTransformInfoWrapperPass>();
    if (OptLevel != CodeGenOpt::None) {
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<TargetTransformInfoWrapperPass>();
    }
    AU.addPreserved<DominatorTreeWrapperPass>();
  }

  StringRef getPassName() const override {
    return "Exception handling preparation";
  }
};

} // end anonymous namespace

char DwarfEHPrepareLegacyPass::ID = 0;

INITIALIZE_PASS_BEGIN(DwarfEHPrepareLegacyPass, DEBUG_TYPE,
                      "Prepare DWARF exceptions", false, false)
INITIALIZE_PASS_DEPENDENCY(DominatorTreeWrapperPass)
INITIALIZE_PASS_DEPENDENCY(TargetPassConfig)
INITIALIZE_PASS_DEPENDENCY(TargetTransformInfoWrapperPass)
INITIALIZE_PASS_END(DwarfEHPrepareLegacyPass, DEBUG_TYPE,
                    "Prepare DWARF exceptions", false, false)

FunctionPass *llvm::createDwarfEHPass(CodeGenOpt::Level OptLevel) {
  return new DwarfEHPrepareLegacyPass(OptLevel);
}
