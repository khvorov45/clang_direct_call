//===-- ResetMachineFunctionPass.cpp - Reset Machine Function ----*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file implements a pass that will conditionally reset a machine
/// function as if it was just created. This is used to provide a fallback
/// mechanism when GlobalISel fails, thus the condition for the reset to
/// happen is that the MachineFunction has the FailedISel property.
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_ADT_ScopeExit.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"
#include "llvm_include_llvm_CodeGen_MachineRegisterInfo.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_CodeGen_StackProtector.h"
#include "llvm_include_llvm_IR_DiagnosticInfo.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Support_Debug.h"
using namespace llvm;

#define DEBUG_TYPE "reset-machine-function"

STATISTIC(NumFunctionsReset, "Number of functions reset");
STATISTIC(NumFunctionsVisited, "Number of functions visited");

namespace {
  class ResetMachineFunction : public MachineFunctionPass {
    /// Tells whether or not this pass should emit a fallback
    /// diagnostic when it resets a function.
    bool EmitFallbackDiag;
    /// Whether we should abort immediately instead of resetting the function.
    bool AbortOnFailedISel;

  public:
    static char ID; // Pass identification, replacement for typeid
    ResetMachineFunction(bool EmitFallbackDiag = false,
                         bool AbortOnFailedISel = false)
        : MachineFunctionPass(ID), EmitFallbackDiag(EmitFallbackDiag),
          AbortOnFailedISel(AbortOnFailedISel) {}

    StringRef getPassName() const override { return "ResetMachineFunction"; }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addPreserved<StackProtector>();
      MachineFunctionPass::getAnalysisUsage(AU);
    }

    bool runOnMachineFunction(MachineFunction &MF) override {
      ++NumFunctionsVisited;
      // No matter what happened, whether we successfully selected the function
      // or not, nothing is going to use the vreg types after us. Make sure they
      // disappear.
      auto ClearVRegTypesOnReturn =
          make_scope_exit([&MF]() { MF.getRegInfo().clearVirtRegTypes(); });

      if (MF.getProperties().hasProperty(
              MachineFunctionProperties::Property::FailedISel)) {
        if (AbortOnFailedISel)
          report_fatal_error("Instruction selection failed");
        LLVM_DEBUG(dbgs() << "Resetting: " << MF.getName() << '\n');
        ++NumFunctionsReset;
        MF.reset();
        MF.initTargetMachineFunctionInfo(MF.getSubtarget());

        if (EmitFallbackDiag) {
          const Function &F = MF.getFunction();
          DiagnosticInfoISelFallback DiagFallback(F);
          F.getContext().diagnose(DiagFallback);
        }
        return true;
      }
      return false;
    }

  };
} // end anonymous namespace

char ResetMachineFunction::ID = 0;
INITIALIZE_PASS(ResetMachineFunction, DEBUG_TYPE,
                "Reset machine function if ISel failed", false, false)

MachineFunctionPass *
llvm::createResetMachineFunctionPass(bool EmitFallbackDiag = false,
                                     bool AbortOnFailedISel = false) {
  return new ResetMachineFunction(EmitFallbackDiag, AbortOnFailedISel);
}
