//===- SSAContext.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
///
/// This file defines a specialization of the GenericSSAContext<X>
/// template class for LLVM IR.
///
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_IR_SSAContext.h"
#include "llvm_include_llvm_IR_Argument.h"
#include "llvm_include_llvm_IR_BasicBlock.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_Instruction.h"
#include "llvm_include_llvm_IR_Instructions.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace llvm;

Value *SSAContext::ValueRefNull = nullptr;

void SSAContext::setFunction(Function &Fn) { F = &Fn; }

BasicBlock *SSAContext::getEntryBlock(Function &F) {
  return &F.getEntryBlock();
}

const BasicBlock *SSAContext::getEntryBlock(const Function &F) {
  return &F.getEntryBlock();
}

void SSAContext::appendBlockDefs(SmallVectorImpl<Value *> &defs,
                                 BasicBlock &block) {
  for (auto &instr : block.instructionsWithoutDebug(/*SkipPseudoOp=*/true)) {
    if (instr.isTerminator())
      break;
    if (instr.getType()->isVoidTy())
      continue;
    auto *def = &instr;
    defs.push_back(def);
  }
}

void SSAContext::appendBlockDefs(SmallVectorImpl<const Value *> &defs,
                                 const BasicBlock &block) {
  for (auto &instr : block) {
    if (instr.isTerminator())
      break;
    defs.push_back(&instr);
  }
}

void SSAContext::appendBlockTerms(SmallVectorImpl<Instruction *> &terms,
                                  BasicBlock &block) {
  terms.push_back(block.getTerminator());
}

void SSAContext::appendBlockTerms(SmallVectorImpl<const Instruction *> &terms,
                                  const BasicBlock &block) {
  terms.push_back(block.getTerminator());
}

const BasicBlock *SSAContext::getDefBlock(const Value *value) const {
  if (const auto *instruction = dyn_cast<Instruction>(value))
    return instruction->getParent();
  return nullptr;
}

bool SSAContext::comesBefore(const Instruction *lhs, const Instruction *rhs) {
  return lhs->comesBefore(rhs);
}

bool SSAContext::isConstantValuePhi(const Instruction &Instr) {
  if (auto *Phi = dyn_cast<PHINode>(&Instr))
    return Phi->hasConstantValue();
  return false;
}

Printable SSAContext::print(const Value *V) const {
  return Printable([V](raw_ostream &Out) { V->print(Out); });
}

Printable SSAContext::print(const Instruction *Inst) const {
  return print(cast<Value>(Inst));
}

Printable SSAContext::print(const BasicBlock *BB) const {
  if (!BB)
    return Printable([](raw_ostream &Out) { Out << "<nullptr>"; });
  if (BB->hasName())
    return Printable([BB](raw_ostream &Out) { Out << BB->getName(); });

  return Printable([BB](raw_ostream &Out) {
    ModuleSlotTracker MST{BB->getParent()->getParent(), false};
    MST.incorporateFunction(*BB->getParent());
    Out << MST.getLocalSlot(BB);
  });
}
