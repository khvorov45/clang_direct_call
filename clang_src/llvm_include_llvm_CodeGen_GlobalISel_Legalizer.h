//== llvm/CodeGen/GlobalISel/Legalizer.h ---------------- -*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file A pass to convert the target-illegal operations created by IR -> MIR
/// translation into ones the target expects to be able to select. This may
/// occur in multiple phases, for example G_ADD <2 x i8> -> G_ADD <2 x i16> ->
/// G_ADD <4 x i16>.
///
/// The LegalizeHelper class is where most of the work happens, and is designed
/// to be callable from other passes that find themselves with an illegal
/// instruction.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_GLOBALISEL_LEGALIZER_H
#define LLVM_CODEGEN_GLOBALISEL_LEGALIZER_H

#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"

namespace llvm {

class LegalizerInfo;
class MachineIRBuilder;
class MachineInstr;
class GISelChangeObserver;
class LostDebugLocObserver;

class Legalizer : public MachineFunctionPass {
public:
  static char ID;

  struct MFResult {
    bool Changed;
    const MachineInstr *FailedOn;
  };

private:
  /// Initialize the field members using \p MF.
  void init(MachineFunction &MF);

public:
  // Ctor, nothing fancy.
  Legalizer();

  StringRef getPassName() const override { return "Legalizer"; }

  void getAnalysisUsage(AnalysisUsage &AU) const override;

  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::IsSSA);
  }

  MachineFunctionProperties getSetProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::Legalized);
  }

  MachineFunctionProperties getClearedProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::NoPHIs);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  static MFResult
  legalizeMachineFunction(MachineFunction &MF, const LegalizerInfo &LI,
                          ArrayRef<GISelChangeObserver *> AuxObservers,
                          LostDebugLocObserver &LocObserver,
                          MachineIRBuilder &MIRBuilder);
};
} // End namespace llvm.

#endif
