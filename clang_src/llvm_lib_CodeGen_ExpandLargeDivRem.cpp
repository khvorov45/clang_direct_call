//===--- ExpandLargeDivRem.cpp - Expand large div/rem ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass expands div/rem instructions with a bitwidth above a threshold
// into a call to auto-generated functions.
// This is useful for targets like x86_64 that cannot lower divisions
// with more than 128 bits or targets like x86_32 that cannot lower divisions
// with more than 64 bits.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_ADT_StringExtras.h"
#include "llvm_include_llvm_Analysis_GlobalsModRef.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_CodeGen_TargetLowering.h"
#include "llvm_include_llvm_CodeGen_TargetPassConfig.h"
#include "llvm_include_llvm_CodeGen_TargetSubtargetInfo.h"
#include "llvm_include_llvm_IR_IRBuilder.h"
#include "llvm_include_llvm_IR_InstIterator.h"
#include "llvm_include_llvm_IR_PassManager.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Support_CommandLine.h"
#include "llvm_include_llvm_Target_TargetMachine.h"
#include "llvm_include_llvm_Transforms_Utils_IntegerDivision.h"

using namespace llvm;

static cl::opt<unsigned>
    ExpandDivRemBits("expand-div-rem-bits", cl::Hidden,
                     cl::init(llvm::IntegerType::MAX_INT_BITS),
                     cl::desc("div and rem instructions on integers with "
                              "more than <N> bits are expanded."));

static bool isConstantPowerOfTwo(llvm::Value *V, bool SignedOp) {
  auto *C = dyn_cast<ConstantInt>(V);
  if (!C)
    return false;

  APInt Val = C->getValue();
  if (SignedOp && Val.isNegative())
    Val = -Val;
  return Val.isPowerOf2();
}

static bool isSigned(unsigned int Opcode) {
  return Opcode == Instruction::SDiv || Opcode == Instruction::SRem;
}

static bool runImpl(Function &F, const TargetLowering &TLI) {
  SmallVector<BinaryOperator *, 4> Replace;
  bool Modified = false;

  unsigned MaxLegalDivRemBitWidth = TLI.getMaxDivRemBitWidthSupported();
  if (ExpandDivRemBits != llvm::IntegerType::MAX_INT_BITS)
    MaxLegalDivRemBitWidth = ExpandDivRemBits;

  if (MaxLegalDivRemBitWidth >= llvm::IntegerType::MAX_INT_BITS)
    return false;

  for (auto &I : instructions(F)) {
    switch (I.getOpcode()) {
    case Instruction::UDiv:
    case Instruction::SDiv:
    case Instruction::URem:
    case Instruction::SRem: {
      // TODO: This doesn't handle vectors.
      auto *IntTy = dyn_cast<IntegerType>(I.getType());
      if (!IntTy || IntTy->getIntegerBitWidth() <= MaxLegalDivRemBitWidth)
        continue;

      // The backend has peephole optimizations for powers of two.
      if (isConstantPowerOfTwo(I.getOperand(1), isSigned(I.getOpcode())))
        continue;

      Replace.push_back(&cast<BinaryOperator>(I));
      Modified = true;
      break;
    }
    default:
      break;
    }
  }

  if (Replace.empty())
    return false;

  while (!Replace.empty()) {
    BinaryOperator *I = Replace.pop_back_val();

    if (I->getOpcode() == Instruction::UDiv ||
        I->getOpcode() == Instruction::SDiv) {
      expandDivision(I);
    } else {
      expandRemainder(I);
    }
  }

  return Modified;
}

class ExpandLargeDivRemLegacyPass : public FunctionPass {
public:
  static char ID;

  ExpandLargeDivRemLegacyPass() : FunctionPass(ID) {
    initializeExpandLargeDivRemLegacyPassPass(*PassRegistry::getPassRegistry());
  }

  bool runOnFunction(Function &F) override {
    auto *TM = &getAnalysis<TargetPassConfig>().getTM<TargetMachine>();
    auto *TLI = TM->getSubtargetImpl(F)->getTargetLowering();
    return runImpl(F, *TLI);
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<TargetPassConfig>();
    AU.addPreserved<AAResultsWrapperPass>();
    AU.addPreserved<GlobalsAAWrapperPass>();
  }
};

char ExpandLargeDivRemLegacyPass::ID = 0;
INITIALIZE_PASS_BEGIN(ExpandLargeDivRemLegacyPass, "expand-large-div-rem",
                      "Expand large div/rem", false, false)
INITIALIZE_PASS_END(ExpandLargeDivRemLegacyPass, "expand-large-div-rem",
                    "Expand large div/rem", false, false)

FunctionPass *llvm::createExpandLargeDivRemPass() {
  return new ExpandLargeDivRemLegacyPass();
}
