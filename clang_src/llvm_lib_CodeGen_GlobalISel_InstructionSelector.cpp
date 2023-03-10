//===- llvm/CodeGen/GlobalISel/InstructionSelector.cpp --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// This file implements the InstructionSelector class.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_CodeGen_GlobalISel_InstructionSelector.h"
#include "llvm_include_llvm_CodeGen_GlobalISel_Utils.h"
#include "llvm_include_llvm_CodeGen_MachineInstr.h"
#include "llvm_include_llvm_CodeGen_MachineOperand.h"
#include "llvm_include_llvm_CodeGen_MachineRegisterInfo.h"

#define DEBUG_TYPE "instructionselector"

using namespace llvm;

InstructionSelector::MatcherState::MatcherState(unsigned MaxRenderers)
    : Renderers(MaxRenderers) {}

InstructionSelector::InstructionSelector() = default;

bool InstructionSelector::isOperandImmEqual(
    const MachineOperand &MO, int64_t Value,
    const MachineRegisterInfo &MRI) const {
  if (MO.isReg() && MO.getReg())
    if (auto VRegVal = getIConstantVRegValWithLookThrough(MO.getReg(), MRI))
      return VRegVal->Value.getSExtValue() == Value;
  return false;
}

bool InstructionSelector::isBaseWithConstantOffset(
    const MachineOperand &Root, const MachineRegisterInfo &MRI) const {
  if (!Root.isReg())
    return false;

  MachineInstr *RootI = MRI.getVRegDef(Root.getReg());
  if (RootI->getOpcode() != TargetOpcode::G_PTR_ADD)
    return false;

  MachineOperand &RHS = RootI->getOperand(2);
  MachineInstr *RHSI = MRI.getVRegDef(RHS.getReg());
  if (RHSI->getOpcode() != TargetOpcode::G_CONSTANT)
    return false;

  return true;
}

bool InstructionSelector::isObviouslySafeToFold(MachineInstr &MI,
                                                MachineInstr &IntoMI) const {
  // Immediate neighbours are already folded.
  if (MI.getParent() == IntoMI.getParent() &&
      std::next(MI.getIterator()) == IntoMI.getIterator())
    return true;

  // Convergent instructions cannot be moved in the CFG.
  if (MI.isConvergent() && MI.getParent() != IntoMI.getParent())
    return false;

  return !MI.mayLoadOrStore() && !MI.mayRaiseFPException() &&
         !MI.hasUnmodeledSideEffects() && MI.implicit_operands().empty();
}
