//===- llvm/LinkAllPasses.h ------------ Reference All Passes ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This header file pulls in all transformation and analysis passes for tools
// like opt and bugpoint that need this functionality.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LINKALLPASSES_H
#define LLVM_LINKALLPASSES_H

#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_Analysis_AliasAnalysisEvaluator.h"
#include "llvm_include_llvm_Analysis_AliasSetTracker.h"
#include "llvm_include_llvm_Analysis_BasicAliasAnalysis.h"
#include "llvm_include_llvm_Analysis_CallPrinter.h"
#include "llvm_include_llvm_Analysis_DomPrinter.h"
#include "llvm_include_llvm_Analysis_GlobalsModRef.h"
#include "llvm_include_llvm_Analysis_IntervalPartition.h"
#include "llvm_include_llvm_Analysis_Lint.h"
#include "llvm_include_llvm_Analysis_Passes.h"
#include "llvm_include_llvm_Analysis_PostDominators.h"
#include "llvm_include_llvm_Analysis_RegionPass.h"
#include "llvm_include_llvm_Analysis_RegionPrinter.h"
#include "llvm_include_llvm_Analysis_ScalarEvolution.h"
#include "llvm_include_llvm_Analysis_ScalarEvolutionAliasAnalysis.h"
#include "llvm_include_llvm_Analysis_ScopedNoAliasAA.h"
#include "llvm_include_llvm_Analysis_TargetLibraryInfo.h"
#include "llvm_include_llvm_Analysis_TypeBasedAliasAnalysis.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_IRPrintingPasses.h"
#include "llvm_include_llvm_Support_Valgrind.h"
#include "llvm_include_llvm_Transforms_AggressiveInstCombine_AggressiveInstCombine.h"
#include "llvm_include_llvm_Transforms_IPO.h"
#include "llvm_include_llvm_Transforms_IPO_AlwaysInliner.h"
#include "llvm_include_llvm_Transforms_IPO_Attributor.h"
#include "llvm_include_llvm_Transforms_IPO_FunctionAttrs.h"
#include "llvm_include_llvm_Transforms_InstCombine_InstCombine.h"
#include "llvm_include_llvm_Transforms_Instrumentation.h"
#include "llvm_include_llvm_Transforms_Instrumentation_BoundsChecking.h"
#include "llvm_include_llvm_Transforms_ObjCARC.h"
#include "llvm_include_llvm_Transforms_Scalar.h"
#include "llvm_include_llvm_Transforms_Scalar_GVN.h"
#include "llvm_include_llvm_Transforms_Scalar_InstSimplifyPass.h"
#include "llvm_include_llvm_Transforms_Scalar_Scalarizer.h"
#include "llvm_include_llvm_Transforms_Utils.h"
#include "llvm_include_llvm_Transforms_Utils_SymbolRewriter.h"
#include "llvm_include_llvm_Transforms_Utils_UnifyFunctionExitNodes.h"
#include "llvm_include_llvm_Transforms_Vectorize.h"
#include <cstdlib>

namespace {
  struct ForcePassLinking {
    ForcePassLinking() {
      // We must reference the passes in such a way that compilers will not
      // delete it all as dead code, even with whole program optimization,
      // yet is effectively a NO-OP. As the compiler isn't smart enough
      // to know that getenv() never returns -1, this will do the job.
      // This is so that globals in the translation units where these functions
      // are defined are forced to be initialized, populating various
      // registries.
      if (std::getenv("bar") != (char*) -1)
        return;

      (void) llvm::createAAEvalPass();
      (void) llvm::createAggressiveDCEPass();
      (void)llvm::createBitTrackingDCEPass();
      (void) llvm::createAlignmentFromAssumptionsPass();
      (void) llvm::createBasicAAWrapperPass();
      (void) llvm::createSCEVAAWrapperPass();
      (void) llvm::createTypeBasedAAWrapperPass();
      (void) llvm::createScopedNoAliasAAWrapperPass();
      (void) llvm::createBreakCriticalEdgesPass();
      (void) llvm::createCallGraphDOTPrinterPass();
      (void) llvm::createCallGraphViewerPass();
      (void) llvm::createCFGSimplificationPass();
      (void) llvm::createStructurizeCFGPass();
      (void) llvm::createLibCallsShrinkWrapPass();
      (void) llvm::createCalledValuePropagationPass();
      (void) llvm::createConstantMergePass();
      (void) llvm::createCostModelAnalysisPass();
      (void) llvm::createDeadArgEliminationPass();
      (void) llvm::createDeadCodeEliminationPass();
      (void) llvm::createDeadStoreEliminationPass();
      (void) llvm::createDependenceAnalysisWrapperPass();
      (void) llvm::createDomOnlyPrinterWrapperPassPass();
      (void) llvm::createDomPrinterWrapperPassPass();
      (void) llvm::createDomOnlyViewerWrapperPassPass();
      (void) llvm::createDomViewerWrapperPassPass();
      (void) llvm::createFunctionInliningPass();
      (void) llvm::createAlwaysInlinerLegacyPass();
      (void) llvm::createGlobalDCEPass();
      (void) llvm::createGlobalOptimizerPass();
      (void) llvm::createGlobalsAAWrapperPass();
      (void) llvm::createGuardWideningPass();
      (void) llvm::createLoopGuardWideningPass();
      (void) llvm::createIPSCCPPass();
      (void) llvm::createInductiveRangeCheckEliminationPass();
      (void) llvm::createIndVarSimplifyPass();
      (void) llvm::createInstSimplifyLegacyPass();
      (void) llvm::createInstructionCombiningPass();
      (void) llvm::createInternalizePass();
      (void) llvm::createJMCInstrumenterPass();
      (void) llvm::createLCSSAPass();
      (void) llvm::createLegacyDivergenceAnalysisPass();
      (void) llvm::createLICMPass();
      (void) llvm::createLoopSinkPass();
      (void) llvm::createLazyValueInfoPass();
      (void) llvm::createLoopExtractorPass();
      (void) llvm::createLoopInterchangePass();
      (void) llvm::createLoopFlattenPass();
      (void) llvm::createLoopPredicationPass();
      (void) llvm::createLoopSimplifyPass();
      (void) llvm::createLoopSimplifyCFGPass();
      (void) llvm::createLoopStrengthReducePass();
      (void) llvm::createLoopRerollPass();
      (void) llvm::createLoopUnrollPass();
      (void) llvm::createLoopUnrollAndJamPass();
      (void) llvm::createLoopVersioningLICMPass();
      (void) llvm::createLoopIdiomPass();
      (void) llvm::createLoopRotatePass();
      (void) llvm::createLowerConstantIntrinsicsPass();
      (void) llvm::createLowerExpectIntrinsicPass();
      (void) llvm::createLowerGlobalDtorsLegacyPass();
      (void) llvm::createLowerInvokePass();
      (void) llvm::createLowerSwitchPass();
      (void) llvm::createNaryReassociatePass();
      (void) llvm::createObjCARCContractPass();
      (void) llvm::createPromoteMemoryToRegisterPass();
      (void) llvm::createDemoteRegisterToMemoryPass();
      (void)llvm::createPostDomOnlyPrinterWrapperPassPass();
      (void)llvm::createPostDomPrinterWrapperPassPass();
      (void)llvm::createPostDomOnlyViewerWrapperPassPass();
      (void)llvm::createPostDomViewerWrapperPassPass();
      (void) llvm::createReassociatePass();
      (void) llvm::createRedundantDbgInstEliminationPass();
      (void) llvm::createRegionInfoPass();
      (void) llvm::createRegionOnlyPrinterPass();
      (void) llvm::createRegionOnlyViewerPass();
      (void) llvm::createRegionPrinterPass();
      (void) llvm::createRegionViewerPass();
      (void) llvm::createSCCPPass();
      (void) llvm::createSafeStackPass();
      (void) llvm::createSROAPass();
      (void) llvm::createSingleLoopExtractorPass();
      (void) llvm::createStripSymbolsPass();
      (void) llvm::createStripNonDebugSymbolsPass();
      (void) llvm::createStripDeadDebugInfoPass();
      (void) llvm::createStripDeadPrototypesPass();
      (void) llvm::createTailCallEliminationPass();
      (void)llvm::createTLSVariableHoistPass();
      (void) llvm::createJumpThreadingPass();
      (void) llvm::createDFAJumpThreadingPass();
      (void) llvm::createUnifyFunctionExitNodesPass();
      (void) llvm::createInstCountPass();
      (void) llvm::createConstantHoistingPass();
      (void) llvm::createCodeGenPreparePass();
      (void) llvm::createEarlyCSEPass();
      (void) llvm::createGVNHoistPass();
      (void) llvm::createMergedLoadStoreMotionPass();
      (void) llvm::createGVNPass();
      (void) llvm::createNewGVNPass();
      (void) llvm::createMemCpyOptPass();
      (void) llvm::createLoopDeletionPass();
      (void) llvm::createPostDomTree();
      (void) llvm::createInstructionNamerPass();
      (void) llvm::createMetaRenamerPass();
      (void) llvm::createAttributorLegacyPass();
      (void) llvm::createAttributorCGSCCLegacyPass();
      (void) llvm::createPostOrderFunctionAttrsLegacyPass();
      (void) llvm::createReversePostOrderFunctionAttrsPass();
      (void) llvm::createMergeFunctionsPass();
      (void) llvm::createMergeICmpsLegacyPass();
      (void) llvm::createExpandLargeDivRemPass();
      (void) llvm::createExpandMemCmpPass();
      (void) llvm::createExpandVectorPredicationPass();
      std::string buf;
      llvm::raw_string_ostream os(buf);
      (void) llvm::createPrintModulePass(os);
      (void) llvm::createPrintFunctionPass(os);
      (void) llvm::createModuleDebugInfoPrinterPass();
      (void) llvm::createPartialInliningPass();
      (void) llvm::createLintLegacyPassPass();
      (void) llvm::createSinkingPass();
      (void) llvm::createLowerAtomicPass();
      (void) llvm::createCorrelatedValuePropagationPass();
      (void) llvm::createMemDepPrinter();
      (void) llvm::createLoopVectorizePass();
      (void) llvm::createSLPVectorizerPass();
      (void) llvm::createLoadStoreVectorizerPass();
      (void) llvm::createVectorCombinePass();
      (void) llvm::createPartiallyInlineLibCallsPass();
      (void) llvm::createScalarizerPass();
      (void) llvm::createSeparateConstOffsetFromGEPPass();
      (void) llvm::createSpeculativeExecutionPass();
      (void) llvm::createSpeculativeExecutionIfHasBranchDivergencePass();
      (void) llvm::createRewriteSymbolsPass();
      (void) llvm::createStraightLineStrengthReducePass();
      (void) llvm::createMemDerefPrinter();
      (void) llvm::createMustExecutePrinter();
      (void) llvm::createMustBeExecutedContextPrinter();
      (void) llvm::createFloat2IntPass();
      (void) llvm::createEliminateAvailableExternallyPass();
      (void)llvm::createScalarizeMaskedMemIntrinLegacyPass();
      (void) llvm::createWarnMissedTransformationsPass();
      (void) llvm::createHardwareLoopsPass();
      (void) llvm::createInjectTLIMappingsLegacyPass();
      (void) llvm::createUnifyLoopExitsPass();
      (void) llvm::createFixIrreduciblePass();
      (void)llvm::createSelectOptimizePass();

      (void)new llvm::IntervalPartition();
      (void)new llvm::ScalarEvolutionWrapperPass();
      llvm::Function::Create(nullptr, llvm::GlobalValue::ExternalLinkage)->viewCFGOnly();
      llvm::RGPassManager RGM;
      llvm::TargetLibraryInfoImpl TLII;
      llvm::TargetLibraryInfo TLI(TLII);
      llvm::AliasAnalysis AA(TLI);
      llvm::BatchAAResults BAA(AA);
      llvm::AliasSetTracker X(BAA);
      X.add(nullptr, llvm::LocationSize::beforeOrAfterPointer(),
            llvm::AAMDNodes()); // for -print-alias-sets
      (void) llvm::AreStatisticsEnabled();
      (void) llvm::sys::RunningOnValgrind();
    }
  } ForcePassLinking; // Force link by creating a global definition.
}

#endif
