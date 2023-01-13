//===-- TargetOptionsImpl.cpp - Options that apply to all targets ----------==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the methods in the TargetOptions.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_CodeGen_MachineFrameInfo.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_TargetFrameLowering.h"
#include "llvm_include_llvm_CodeGen_TargetSubtargetInfo.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_Target_TargetOptions.h"
using namespace llvm;

/// DisableFramePointerElim - This returns true if frame pointer elimination
/// optimization should be disabled for the given machine function.
bool TargetOptions::DisableFramePointerElim(const MachineFunction &MF) const {
  // Check to see if the target want to forcably keep frame pointer.
  if (MF.getSubtarget().getFrameLowering()->keepFramePointer(MF))
    return true;

  const Function &F = MF.getFunction();

  if (!F.hasFnAttribute("frame-pointer"))
    return false;
  StringRef FP = F.getFnAttribute("frame-pointer").getValueAsString();
  if (FP == "all")
    return true;
  if (FP == "non-leaf")
    return MF.getFrameInfo().hasCalls();
  if (FP == "none")
    return false;
  llvm_unreachable("unknown frame pointer flag");
}

/// HonorSignDependentRoundingFPMath - Return true if the codegen must assume
/// that the rounding mode of the FPU can change from its default.
bool TargetOptions::HonorSignDependentRoundingFPMath() const {
  return !UnsafeFPMath && HonorSignDependentRoundingFPMathOption;
}

/// NOTE: There are targets that still do not support the debug entry values
/// production and that is being controlled with the SupportsDebugEntryValues.
/// In addition, SCE debugger does not have the feature implemented, so prefer
/// not to emit the debug entry values in that case.
/// The EnableDebugEntryValues can be used for the testing purposes.
bool TargetOptions::ShouldEmitDebugEntryValues() const {
  return (SupportsDebugEntryValues && DebuggerTuning != DebuggerKind::SCE) ||
         EnableDebugEntryValues;
}
