//===-- Utils.cpp - TransformUtils Infrastructure -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the common initialization infrastructure for the
// TransformUtils library.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Utils.h"
#include "llvm_include_llvm-c_Initialization.h"
#include "llvm_include_llvm-c_Transforms_Utils.h"
#include "llvm_include_llvm_IR_LegacyPassManager.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_PassRegistry.h"

using namespace llvm;

/// initializeTransformUtils - Initialize all passes in the TransformUtils
/// library.
void llvm::initializeTransformUtils(PassRegistry &Registry) {
  initializeAddDiscriminatorsLegacyPassPass(Registry);
  initializeAssumeSimplifyPassLegacyPassPass(Registry);
  initializeAssumeBuilderPassLegacyPassPass(Registry);
  initializeBreakCriticalEdgesPass(Registry);
  initializeCanonicalizeFreezeInLoopsPass(Registry);
  initializeInstNamerPass(Registry);
  initializeLCSSAWrapperPassPass(Registry);
  initializeLibCallsShrinkWrapLegacyPassPass(Registry);
  initializeLoopSimplifyPass(Registry);
  initializeLowerGlobalDtorsLegacyPassPass(Registry);
  initializeLowerInvokeLegacyPassPass(Registry);
  initializeLowerSwitchLegacyPassPass(Registry);
  initializePromoteLegacyPassPass(Registry);
  initializeStripNonLineTableDebugLegacyPassPass(Registry);
  initializeUnifyFunctionExitNodesLegacyPassPass(Registry);
  initializeMetaRenamerPass(Registry);
  initializeStripGCRelocatesLegacyPass(Registry);
  initializePredicateInfoPrinterLegacyPassPass(Registry);
  initializeInjectTLIMappingsLegacyPass(Registry);
  initializeFixIrreduciblePass(Registry);
  initializeUnifyLoopExitsLegacyPassPass(Registry);
}

/// LLVMInitializeTransformUtils - C binding for initializeTransformUtilsPasses.
void LLVMInitializeTransformUtils(LLVMPassRegistryRef R) {
  initializeTransformUtils(*unwrap(R));
}

void LLVMAddLowerSwitchPass(LLVMPassManagerRef PM) {
  unwrap(PM)->add(createLowerSwitchPass());
}

void LLVMAddPromoteMemoryToRegisterPass(LLVMPassManagerRef PM) {
  unwrap(PM)->add(createPromoteMemoryToRegisterPass());
}

void LLVMAddAddDiscriminatorsPass(LLVMPassManagerRef PM) {
  unwrap(PM)->add(createAddDiscriminatorsPass());
}
