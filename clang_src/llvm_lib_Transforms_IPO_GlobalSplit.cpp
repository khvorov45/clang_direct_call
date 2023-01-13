//===- GlobalSplit.cpp - global variable splitter -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass uses inrange annotations on GEP indices to split globals where
// beneficial. Clang currently attaches these annotations to references to
// virtual table globals under the Itanium ABI for the benefit of the
// whole-program virtual call optimization and control flow integrity passes.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_IPO_GlobalSplit.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_ADT_StringExtras.h"
#include "llvm_include_llvm_IR_Constant.h"
#include "llvm_include_llvm_IR_Constants.h"
#include "llvm_include_llvm_IR_DataLayout.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_GlobalValue.h"
#include "llvm_include_llvm_IR_GlobalVariable.h"
#include "llvm_include_llvm_IR_Intrinsics.h"
#include "llvm_include_llvm_IR_LLVMContext.h"
#include "llvm_include_llvm_IR_Metadata.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_IR_Operator.h"
#include "llvm_include_llvm_IR_Type.h"
#include "llvm_include_llvm_IR_User.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Support_Casting.h"
#include "llvm_include_llvm_Transforms_IPO.h"
#include <cstdint>
#include <vector>

using namespace llvm;

static bool splitGlobal(GlobalVariable &GV) {
  // If the address of the global is taken outside of the module, we cannot
  // apply this transformation.
  if (!GV.hasLocalLinkage())
    return false;

  // We currently only know how to split ConstantStructs.
  auto *Init = dyn_cast_or_null<ConstantStruct>(GV.getInitializer());
  if (!Init)
    return false;

  // Verify that each user of the global is an inrange getelementptr constant.
  // From this it follows that any loads from or stores to that global must use
  // a pointer derived from an inrange getelementptr constant, which is
  // sufficient to allow us to apply the splitting transform.
  for (User *U : GV.users()) {
    if (!isa<Constant>(U))
      return false;

    auto *GEP = dyn_cast<GEPOperator>(U);
    if (!GEP || !GEP->getInRangeIndex() || *GEP->getInRangeIndex() != 1 ||
        !isa<ConstantInt>(GEP->getOperand(1)) ||
        !cast<ConstantInt>(GEP->getOperand(1))->isZero() ||
        !isa<ConstantInt>(GEP->getOperand(2)))
      return false;
  }

  SmallVector<MDNode *, 2> Types;
  GV.getMetadata(LLVMContext::MD_type, Types);

  const DataLayout &DL = GV.getParent()->getDataLayout();
  const StructLayout *SL = DL.getStructLayout(Init->getType());

  IntegerType *Int32Ty = Type::getInt32Ty(GV.getContext());

  std::vector<GlobalVariable *> SplitGlobals(Init->getNumOperands());
  for (unsigned I = 0; I != Init->getNumOperands(); ++I) {
    // Build a global representing this split piece.
    auto *SplitGV =
        new GlobalVariable(*GV.getParent(), Init->getOperand(I)->getType(),
                           GV.isConstant(), GlobalValue::PrivateLinkage,
                           Init->getOperand(I), GV.getName() + "." + utostr(I));
    SplitGlobals[I] = SplitGV;

    unsigned SplitBegin = SL->getElementOffset(I);
    unsigned SplitEnd = (I == Init->getNumOperands() - 1)
                            ? SL->getSizeInBytes()
                            : SL->getElementOffset(I + 1);

    // Rebuild type metadata, adjusting by the split offset.
    // FIXME: See if we can use DW_OP_piece to preserve debug metadata here.
    for (MDNode *Type : Types) {
      uint64_t ByteOffset = cast<ConstantInt>(
              cast<ConstantAsMetadata>(Type->getOperand(0))->getValue())
              ->getZExtValue();
      // Type metadata may be attached one byte after the end of the vtable, for
      // classes without virtual methods in Itanium ABI. AFAIK, it is never
      // attached to the first byte of a vtable. Subtract one to get the right
      // slice.
      // This is making an assumption that vtable groups are the only kinds of
      // global variables that !type metadata can be attached to, and that they
      // are either Itanium ABI vtable groups or contain a single vtable (i.e.
      // Microsoft ABI vtables).
      uint64_t AttachedTo = (ByteOffset == 0) ? ByteOffset : ByteOffset - 1;
      if (AttachedTo < SplitBegin || AttachedTo >= SplitEnd)
        continue;
      SplitGV->addMetadata(
          LLVMContext::MD_type,
          *MDNode::get(GV.getContext(),
                       {ConstantAsMetadata::get(
                            ConstantInt::get(Int32Ty, ByteOffset - SplitBegin)),
                        Type->getOperand(1)}));
    }

    if (GV.hasMetadata(LLVMContext::MD_vcall_visibility))
      SplitGV->setVCallVisibilityMetadata(GV.getVCallVisibility());
  }

  for (User *U : GV.users()) {
    auto *GEP = cast<GEPOperator>(U);
    unsigned I = cast<ConstantInt>(GEP->getOperand(2))->getZExtValue();
    if (I >= SplitGlobals.size())
      continue;

    SmallVector<Value *, 4> Ops;
    Ops.push_back(ConstantInt::get(Int32Ty, 0));
    for (unsigned I = 3; I != GEP->getNumOperands(); ++I)
      Ops.push_back(GEP->getOperand(I));

    auto *NewGEP = ConstantExpr::getGetElementPtr(
        SplitGlobals[I]->getInitializer()->getType(), SplitGlobals[I], Ops,
        GEP->isInBounds());
    GEP->replaceAllUsesWith(NewGEP);
  }

  // Finally, remove the original global. Any remaining uses refer to invalid
  // elements of the global, so replace with poison.
  if (!GV.use_empty())
    GV.replaceAllUsesWith(PoisonValue::get(GV.getType()));
  GV.eraseFromParent();
  return true;
}

static bool splitGlobals(Module &M) {
  // First, see if the module uses either of the llvm.type.test or
  // llvm.type.checked.load intrinsics, which indicates that splitting globals
  // may be beneficial.
  Function *TypeTestFunc =
      M.getFunction(Intrinsic::getName(Intrinsic::type_test));
  Function *TypeCheckedLoadFunc =
      M.getFunction(Intrinsic::getName(Intrinsic::type_checked_load));
  if ((!TypeTestFunc || TypeTestFunc->use_empty()) &&
      (!TypeCheckedLoadFunc || TypeCheckedLoadFunc->use_empty()))
    return false;

  bool Changed = false;
  for (GlobalVariable &GV : llvm::make_early_inc_range(M.globals()))
    Changed |= splitGlobal(GV);
  return Changed;
}

namespace {

struct GlobalSplit : public ModulePass {
  static char ID;

  GlobalSplit() : ModulePass(ID) {
    initializeGlobalSplitPass(*PassRegistry::getPassRegistry());
  }

  bool runOnModule(Module &M) override {
    if (skipModule(M))
      return false;

    return splitGlobals(M);
  }
};

} // end anonymous namespace

char GlobalSplit::ID = 0;

INITIALIZE_PASS(GlobalSplit, "globalsplit", "Global splitter", false, false)

ModulePass *llvm::createGlobalSplitPass() {
  return new GlobalSplit;
}

PreservedAnalyses GlobalSplitPass::run(Module &M, ModuleAnalysisManager &AM) {
  if (!splitGlobals(M))
    return PreservedAnalyses::all();
  return PreservedAnalyses::none();
}
