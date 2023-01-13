//===-- MachineFunctionPass.cpp -------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the definitions of the MachineFunctionPass members.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"
#include "llvm_include_llvm_Analysis_BasicAliasAnalysis.h"
#include "llvm_include_llvm_Analysis_DominanceFrontier.h"
#include "llvm_include_llvm_Analysis_GlobalsModRef.h"
#include "llvm_include_llvm_Analysis_IVUsers.h"
#include "llvm_include_llvm_Analysis_LoopInfo.h"
#include "llvm_include_llvm_Analysis_MemoryDependenceAnalysis.h"
#include "llvm_include_llvm_Analysis_OptimizationRemarkEmitter.h"
#include "llvm_include_llvm_Analysis_ScalarEvolution.h"
#include "llvm_include_llvm_Analysis_ScalarEvolutionAliasAnalysis.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineModuleInfo.h"
#include "llvm_include_llvm_CodeGen_MachineOptimizationRemarkEmitter.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_IR_Dominators.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_PrintPasses.h"

using namespace llvm;
using namespace ore;

Pass *MachineFunctionPass::createPrinterPass(raw_ostream &O,
                                             const std::string &Banner) const {
  return createMachineFunctionPrinterPass(O, Banner);
}

bool MachineFunctionPass::runOnFunction(Function &F) {
  // Do not codegen any 'available_externally' functions at all, they have
  // definitions outside the translation unit.
  if (F.hasAvailableExternallyLinkage())
    return false;

  MachineModuleInfo &MMI = getAnalysis<MachineModuleInfoWrapperPass>().getMMI();
  MachineFunction &MF = MMI.getOrCreateMachineFunction(F);

  MachineFunctionProperties &MFProps = MF.getProperties();

#ifndef NDEBUG
  if (!MFProps.verifyRequiredProperties(RequiredProperties)) {
    errs() << "MachineFunctionProperties required by " << getPassName()
           << " pass are not met by function " << F.getName() << ".\n"
           << "Required properties: ";
    RequiredProperties.print(errs());
    errs() << "\nCurrent properties: ";
    MFProps.print(errs());
    errs() << "\n";
    llvm_unreachable("MachineFunctionProperties check failed");
  }
#endif
  // Collect the MI count of the function before the pass.
  unsigned CountBefore, CountAfter;

  // Check if the user asked for size remarks.
  bool ShouldEmitSizeRemarks =
      F.getParent()->shouldEmitInstrCountChangedRemark();

  // If we want size remarks, collect the number of MachineInstrs in our
  // MachineFunction before the pass runs.
  if (ShouldEmitSizeRemarks)
    CountBefore = MF.getInstructionCount();

  // For --print-changed, if the function name is a candidate, save the
  // serialized MF to be compared later.
  SmallString<0> BeforeStr, AfterStr;
  StringRef PassID;
  if (PrintChanged != ChangePrinter::None) {
    if (const PassInfo *PI = Pass::lookupPassInfo(getPassID()))
      PassID = PI->getPassArgument();
  }
  const bool IsInterestingPass = isPassInPrintList(PassID);
  const bool ShouldPrintChanged = PrintChanged != ChangePrinter::None &&
                                  IsInterestingPass &&
                                  isFunctionInPrintList(MF.getName());
  if (ShouldPrintChanged) {
    raw_svector_ostream OS(BeforeStr);
    MF.print(OS);
  }

  bool RV = runOnMachineFunction(MF);

  if (ShouldEmitSizeRemarks) {
    // We wanted size remarks. Check if there was a change to the number of
    // MachineInstrs in the module. Emit a remark if there was a change.
    CountAfter = MF.getInstructionCount();
    if (CountBefore != CountAfter) {
      MachineOptimizationRemarkEmitter MORE(MF, nullptr);
      MORE.emit([&]() {
        int64_t Delta = static_cast<int64_t>(CountAfter) -
                        static_cast<int64_t>(CountBefore);
        MachineOptimizationRemarkAnalysis R("size-info", "FunctionMISizeChange",
                                            MF.getFunction().getSubprogram(),
                                            &MF.front());
        R << NV("Pass", getPassName())
          << ": Function: " << NV("Function", F.getName()) << ": "
          << "MI Instruction count changed from "
          << NV("MIInstrsBefore", CountBefore) << " to "
          << NV("MIInstrsAfter", CountAfter)
          << "; Delta: " << NV("Delta", Delta);
        return R;
      });
    }
  }

  MFProps.set(SetProperties);
  MFProps.reset(ClearedProperties);

  // For --print-changed, print if the serialized MF has changed. Modes other
  // than quiet/verbose are unimplemented and treated the same as 'quiet'.
  if (ShouldPrintChanged || !IsInterestingPass) {
    if (ShouldPrintChanged) {
      raw_svector_ostream OS(AfterStr);
      MF.print(OS);
    }
    if (IsInterestingPass && BeforeStr != AfterStr) {
      errs() << ("*** IR Dump After " + getPassName() + " (" + PassID +
                 ") on " + MF.getName() + " ***\n");
      switch (PrintChanged) {
      case ChangePrinter::None:
        llvm_unreachable("");
      case ChangePrinter::Quiet:
      case ChangePrinter::Verbose:
      case ChangePrinter::DotCfgQuiet:   // unimplemented
      case ChangePrinter::DotCfgVerbose: // unimplemented
        errs() << AfterStr;
        break;
      case ChangePrinter::DiffQuiet:
      case ChangePrinter::DiffVerbose:
      case ChangePrinter::ColourDiffQuiet:
      case ChangePrinter::ColourDiffVerbose: {
        bool Color = llvm::is_contained(
            {ChangePrinter::ColourDiffQuiet, ChangePrinter::ColourDiffVerbose},
            PrintChanged.getValue());
        StringRef Removed = Color ? "\033[31m-%l\033[0m\n" : "-%l\n";
        StringRef Added = Color ? "\033[32m+%l\033[0m\n" : "+%l\n";
        StringRef NoChange = " %l\n";
        errs() << doSystemDiff(BeforeStr, AfterStr, Removed, Added, NoChange);
        break;
      }
      }
    } else if (llvm::is_contained({ChangePrinter::Verbose,
                                   ChangePrinter::DiffVerbose,
                                   ChangePrinter::ColourDiffVerbose},
                                  PrintChanged.getValue())) {
      const char *Reason =
          IsInterestingPass ? " omitted because no change" : " filtered out";
      errs() << "*** IR Dump After " << getPassName();
      if (!PassID.empty())
        errs() << " (" << PassID << ")";
      errs() << " on " << MF.getName() + Reason + " ***\n";
    }
  }
  return RV;
}

void MachineFunctionPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<MachineModuleInfoWrapperPass>();
  AU.addPreserved<MachineModuleInfoWrapperPass>();

  // MachineFunctionPass preserves all LLVM IR passes, but there's no
  // high-level way to express this. Instead, just list a bunch of
  // passes explicitly. This does not include setPreservesCFG,
  // because CodeGen overloads that to mean preserving the MachineBasicBlock
  // CFG in addition to the LLVM IR CFG.
  AU.addPreserved<BasicAAWrapperPass>();
  AU.addPreserved<DominanceFrontierWrapperPass>();
  AU.addPreserved<DominatorTreeWrapperPass>();
  AU.addPreserved<AAResultsWrapperPass>();
  AU.addPreserved<GlobalsAAWrapperPass>();
  AU.addPreserved<IVUsersWrapperPass>();
  AU.addPreserved<LoopInfoWrapperPass>();
  AU.addPreserved<MemoryDependenceWrapperPass>();
  AU.addPreserved<ScalarEvolutionWrapperPass>();
  AU.addPreserved<SCEVAAWrapperPass>();

  FunctionPass::getAnalysisUsage(AU);
}
