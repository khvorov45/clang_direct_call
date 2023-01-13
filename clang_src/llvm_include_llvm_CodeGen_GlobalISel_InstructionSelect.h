//== llvm/CodeGen/GlobalISel/InstructionSelect.h -----------------*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file This file describes the interface of the MachineFunctionPass
/// responsible for selecting (possibly generic) machine instructions to
/// target-specific instructions.
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_GLOBALISEL_INSTRUCTIONSELECT_H
#define LLVM_CODEGEN_GLOBALISEL_INSTRUCTIONSELECT_H

#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"
#include "llvm_include_llvm_Support_CodeGen.h"

namespace llvm {

class BlockFrequencyInfo;
class ProfileSummaryInfo;

/// This pass is responsible for selecting generic machine instructions to
/// target-specific instructions.  It relies on the InstructionSelector provided
/// by the target.
/// Selection is done by examining blocks in post-order, and instructions in
/// reverse order.
///
/// \post for all inst in MF: not isPreISelGenericOpcode(inst.opcode)
class InstructionSelect : public MachineFunctionPass {
public:
  static char ID;
  StringRef getPassName() const override { return "InstructionSelect"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override;

  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties()
        .set(MachineFunctionProperties::Property::IsSSA)
        .set(MachineFunctionProperties::Property::Legalized)
        .set(MachineFunctionProperties::Property::RegBankSelected);
  }

  MachineFunctionProperties getSetProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::Selected);
  }

  InstructionSelect(CodeGenOpt::Level OL);
  InstructionSelect();

  bool runOnMachineFunction(MachineFunction &MF) override;

protected:
  BlockFrequencyInfo *BFI = nullptr;
  ProfileSummaryInfo *PSI = nullptr;

  CodeGenOpt::Level OptLevel = CodeGenOpt::None;
};
} // End namespace llvm.

#endif
