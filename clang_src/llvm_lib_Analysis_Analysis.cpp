//===-- Analysis.cpp ------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm-c_Analysis.h"
#include "llvm_include_llvm-c_Initialization.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_IR_Verifier.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_PassRegistry.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <cstring>

using namespace llvm;

/// initializeAnalysis - Initialize all passes linked into the Analysis library.
void llvm::initializeAnalysis(PassRegistry &Registry) {
  initializeAAEvalLegacyPassPass(Registry);
  initializeBasicAAWrapperPassPass(Registry);
  initializeBlockFrequencyInfoWrapperPassPass(Registry);
  initializeBranchProbabilityInfoWrapperPassPass(Registry);
  initializeCallGraphWrapperPassPass(Registry);
  initializeCallGraphDOTPrinterPass(Registry);
  initializeCallGraphPrinterLegacyPassPass(Registry);
  initializeCallGraphViewerPass(Registry);
  initializeCostModelAnalysisPass(Registry);
  initializeCFGViewerLegacyPassPass(Registry);
  initializeCFGPrinterLegacyPassPass(Registry);
  initializeCFGOnlyViewerLegacyPassPass(Registry);
  initializeCFGOnlyPrinterLegacyPassPass(Registry);
  initializeCycleInfoWrapperPassPass(Registry);
  initializeDependenceAnalysisWrapperPassPass(Registry);
  initializeDelinearizationPass(Registry);
  initializeDemandedBitsWrapperPassPass(Registry);
  initializeDominanceFrontierWrapperPassPass(Registry);
  initializeDomViewerWrapperPassPass(Registry);
  initializeDomPrinterWrapperPassPass(Registry);
  initializeDomOnlyViewerWrapperPassPass(Registry);
  initializePostDomViewerWrapperPassPass(Registry);
  initializeDomOnlyPrinterWrapperPassPass(Registry);
  initializePostDomPrinterWrapperPassPass(Registry);
  initializePostDomOnlyViewerWrapperPassPass(Registry);
  initializePostDomOnlyPrinterWrapperPassPass(Registry);
  initializeAAResultsWrapperPassPass(Registry);
  initializeGlobalsAAWrapperPassPass(Registry);
  initializeIVUsersWrapperPassPass(Registry);
  initializeInstCountLegacyPassPass(Registry);
  initializeIntervalPartitionPass(Registry);
  initializeIRSimilarityIdentifierWrapperPassPass(Registry);
  initializeLazyBranchProbabilityInfoPassPass(Registry);
  initializeLazyBlockFrequencyInfoPassPass(Registry);
  initializeLazyValueInfoWrapperPassPass(Registry);
  initializeLazyValueInfoPrinterPass(Registry);
  initializeLegacyDivergenceAnalysisPass(Registry);
  initializeLintLegacyPassPass(Registry);
  initializeLoopInfoWrapperPassPass(Registry);
  initializeMemDepPrinterPass(Registry);
  initializeMemDerefPrinterPass(Registry);
  initializeMemoryDependenceWrapperPassPass(Registry);
  initializeModuleDebugInfoLegacyPrinterPass(Registry);
  initializeModuleSummaryIndexWrapperPassPass(Registry);
  initializeMustExecutePrinterPass(Registry);
  initializeMustBeExecutedContextPrinterPass(Registry);
  initializeOptimizationRemarkEmitterWrapperPassPass(Registry);
  initializePhiValuesWrapperPassPass(Registry);
  initializePostDominatorTreeWrapperPassPass(Registry);
  initializeRegionInfoPassPass(Registry);
  initializeRegionViewerPass(Registry);
  initializeRegionPrinterPass(Registry);
  initializeRegionOnlyViewerPass(Registry);
  initializeRegionOnlyPrinterPass(Registry);
  initializeSCEVAAWrapperPassPass(Registry);
  initializeScalarEvolutionWrapperPassPass(Registry);
  initializeStackSafetyGlobalInfoWrapperPassPass(Registry);
  initializeStackSafetyInfoWrapperPassPass(Registry);
  initializeTargetTransformInfoWrapperPassPass(Registry);
  initializeTypeBasedAAWrapperPassPass(Registry);
  initializeScopedNoAliasAAWrapperPassPass(Registry);
  initializeLCSSAVerificationPassPass(Registry);
  initializeMemorySSAWrapperPassPass(Registry);
  initializeMemorySSAPrinterLegacyPassPass(Registry);
}

void LLVMInitializeAnalysis(LLVMPassRegistryRef R) {
  initializeAnalysis(*unwrap(R));
}

void LLVMInitializeIPA(LLVMPassRegistryRef R) {
  initializeAnalysis(*unwrap(R));
}

LLVMBool LLVMVerifyModule(LLVMModuleRef M, LLVMVerifierFailureAction Action,
                          char **OutMessages) {
  raw_ostream *DebugOS = Action != LLVMReturnStatusAction ? &errs() : nullptr;
  std::string Messages;
  raw_string_ostream MsgsOS(Messages);

  LLVMBool Result = verifyModule(*unwrap(M), OutMessages ? &MsgsOS : DebugOS);

  // Duplicate the output to stderr.
  if (DebugOS && OutMessages)
    *DebugOS << MsgsOS.str();

  if (Action == LLVMAbortProcessAction && Result)
    report_fatal_error("Broken module found, compilation aborted!");

  if (OutMessages)
    *OutMessages = strdup(MsgsOS.str().c_str());

  return Result;
}

LLVMBool LLVMVerifyFunction(LLVMValueRef Fn, LLVMVerifierFailureAction Action) {
  LLVMBool Result = verifyFunction(
      *unwrap<Function>(Fn), Action != LLVMReturnStatusAction ? &errs()
                                                              : nullptr);

  if (Action == LLVMAbortProcessAction && Result)
    report_fatal_error("Broken function found, compilation aborted!");

  return Result;
}

void LLVMViewFunctionCFG(LLVMValueRef Fn) {
  Function *F = unwrap<Function>(Fn);
  F->viewCFG();
}

void LLVMViewFunctionCFGOnly(LLVMValueRef Fn) {
  Function *F = unwrap<Function>(Fn);
  F->viewCFGOnly();
}
