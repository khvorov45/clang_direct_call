//===- RegisterUsageInfo.cpp - Register Usage Information Storage ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// This pass is required to take advantage of the interprocedural register
/// allocation infrastructure.
///
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_CodeGen_RegisterUsageInfo.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_CodeGen_MachineOperand.h"
#include "llvm_include_llvm_CodeGen_TargetRegisterInfo.h"
#include "llvm_include_llvm_CodeGen_TargetSubtargetInfo.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Support_CommandLine.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include "llvm_include_llvm_Target_TargetMachine.h"
#include <cstdint>
#include <utility>
#include <vector>

using namespace llvm;

static cl::opt<bool> DumpRegUsage(
    "print-regusage", cl::init(false), cl::Hidden,
    cl::desc("print register usage details collected for analysis."));

INITIALIZE_PASS(PhysicalRegisterUsageInfo, "reg-usage-info",
                "Register Usage Information Storage", false, true)

char PhysicalRegisterUsageInfo::ID = 0;

void PhysicalRegisterUsageInfo::setTargetMachine(const LLVMTargetMachine &TM) {
  this->TM = &TM;
}

bool PhysicalRegisterUsageInfo::doInitialization(Module &M) {
  RegMasks.grow(M.size());
  return false;
}

bool PhysicalRegisterUsageInfo::doFinalization(Module &M) {
  if (DumpRegUsage)
    print(errs());

  RegMasks.shrink_and_clear();
  return false;
}

void PhysicalRegisterUsageInfo::storeUpdateRegUsageInfo(
    const Function &FP, ArrayRef<uint32_t> RegMask) {
  RegMasks[&FP] = RegMask;
}

ArrayRef<uint32_t>
PhysicalRegisterUsageInfo::getRegUsageInfo(const Function &FP) {
  auto It = RegMasks.find(&FP);
  if (It != RegMasks.end())
    return makeArrayRef<uint32_t>(It->second);
  return ArrayRef<uint32_t>();
}

void PhysicalRegisterUsageInfo::print(raw_ostream &OS, const Module *M) const {
  using FuncPtrRegMaskPair = std::pair<const Function *, std::vector<uint32_t>>;

  SmallVector<const FuncPtrRegMaskPair *, 64> FPRMPairVector;

  // Create a vector of pointer to RegMasks entries
  for (const auto &RegMask : RegMasks)
    FPRMPairVector.push_back(&RegMask);

  // sort the vector to print analysis in alphabatic order of function name.
  llvm::sort(
      FPRMPairVector,
      [](const FuncPtrRegMaskPair *A, const FuncPtrRegMaskPair *B) -> bool {
        return A->first->getName() < B->first->getName();
      });

  for (const FuncPtrRegMaskPair *FPRMPair : FPRMPairVector) {
    OS << FPRMPair->first->getName() << " "
       << "Clobbered Registers: ";
    const TargetRegisterInfo *TRI
        = TM->getSubtarget<TargetSubtargetInfo>(*(FPRMPair->first))
          .getRegisterInfo();

    for (unsigned PReg = 1, PRegE = TRI->getNumRegs(); PReg < PRegE; ++PReg) {
      if (MachineOperand::clobbersPhysReg(&(FPRMPair->second[0]), PReg))
        OS << printReg(PReg, TRI) << " ";
    }
    OS << "\n";
  }
}
