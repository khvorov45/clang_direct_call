//===-- CGProfile.cpp -----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Instrumentation_CGProfile.h"

#include "llvm_include_llvm_ADT_MapVector.h"
#include "llvm_include_llvm_Analysis_BlockFrequencyInfo.h"
#include "llvm_include_llvm_Analysis_LazyBlockFrequencyInfo.h"
#include "llvm_include_llvm_Analysis_TargetTransformInfo.h"
#include "llvm_include_llvm_IR_Constants.h"
#include "llvm_include_llvm_IR_MDBuilder.h"
#include "llvm_include_llvm_IR_PassManager.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_ProfileData_InstrProf.h"
#include "llvm_include_llvm_Transforms_Instrumentation.h"
#include <optional>

using namespace llvm;

static bool
addModuleFlags(Module &M,
               MapVector<std::pair<Function *, Function *>, uint64_t> &Counts) {
  if (Counts.empty())
    return false;

  LLVMContext &Context = M.getContext();
  MDBuilder MDB(Context);
  std::vector<Metadata *> Nodes;

  for (auto E : Counts) {
    Metadata *Vals[] = {ValueAsMetadata::get(E.first.first),
                        ValueAsMetadata::get(E.first.second),
                        MDB.createConstant(ConstantInt::get(
                            Type::getInt64Ty(Context), E.second))};
    Nodes.push_back(MDNode::get(Context, Vals));
  }

  M.addModuleFlag(Module::Append, "CG Profile",
                  MDTuple::getDistinct(Context, Nodes));
  return true;
}

static bool runCGProfilePass(
    Module &M, function_ref<BlockFrequencyInfo &(Function &)> GetBFI,
    function_ref<TargetTransformInfo &(Function &)> GetTTI, bool LazyBFI) {
  MapVector<std::pair<Function *, Function *>, uint64_t> Counts;
  InstrProfSymtab Symtab;
  auto UpdateCounts = [&](TargetTransformInfo &TTI, Function *F,
                          Function *CalledF, uint64_t NewCount) {
    if (NewCount == 0)
      return;
    if (!CalledF || !TTI.isLoweredToCall(CalledF) ||
        CalledF->hasDLLImportStorageClass())
      return;
    uint64_t &Count = Counts[std::make_pair(F, CalledF)];
    Count = SaturatingAdd(Count, NewCount);
  };
  // Ignore error here.  Indirect calls are ignored if this fails.
  (void)(bool) Symtab.create(M);
  for (auto &F : M) {
    // Avoid extra cost of running passes for BFI when the function doesn't have
    // entry count. Since LazyBlockFrequencyInfoPass only exists in LPM, check
    // if using LazyBlockFrequencyInfoPass.
    // TODO: Remove LazyBFI when LazyBlockFrequencyInfoPass is available in NPM.
    if (F.isDeclaration() || (LazyBFI && !F.getEntryCount()))
      continue;
    auto &BFI = GetBFI(F);
    if (BFI.getEntryFreq() == 0)
      continue;
    TargetTransformInfo &TTI = GetTTI(F);
    for (auto &BB : F) {
      std::optional<uint64_t> BBCount = BFI.getBlockProfileCount(&BB);
      if (!BBCount)
        continue;
      for (auto &I : BB) {
        CallBase *CB = dyn_cast<CallBase>(&I);
        if (!CB)
          continue;
        if (CB->isIndirectCall()) {
          InstrProfValueData ValueData[8];
          uint32_t ActualNumValueData;
          uint64_t TotalC;
          if (!getValueProfDataFromInst(*CB, IPVK_IndirectCallTarget, 8,
                                        ValueData, ActualNumValueData, TotalC))
            continue;
          for (const auto &VD :
               ArrayRef<InstrProfValueData>(ValueData, ActualNumValueData)) {
            UpdateCounts(TTI, &F, Symtab.getFunction(VD.Value), VD.Count);
          }
          continue;
        }
        UpdateCounts(TTI, &F, CB->getCalledFunction(), *BBCount);
      }
    }
  }

  return addModuleFlags(M, Counts);
}

PreservedAnalyses CGProfilePass::run(Module &M, ModuleAnalysisManager &MAM) {
  FunctionAnalysisManager &FAM =
      MAM.getResult<FunctionAnalysisManagerModuleProxy>(M).getManager();
  auto GetBFI = [&FAM](Function &F) -> BlockFrequencyInfo & {
    return FAM.getResult<BlockFrequencyAnalysis>(F);
  };
  auto GetTTI = [&FAM](Function &F) -> TargetTransformInfo & {
    return FAM.getResult<TargetIRAnalysis>(F);
  };

  runCGProfilePass(M, GetBFI, GetTTI, false);

  return PreservedAnalyses::all();
}
