//===-- llvm/CodeGen/AllocationOrder.cpp - Allocation Order ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements an allocation order for virtual registers.
//
// The preferred allocation order for a virtual register depends on allocation
// hints and target hooks. The AllocationOrder class encapsulates all of that.
//
//===----------------------------------------------------------------------===//

#include "llvm_lib_CodeGen_AllocationOrder.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineRegisterInfo.h"
#include "llvm_include_llvm_CodeGen_RegisterClassInfo.h"
#include "llvm_include_llvm_CodeGen_VirtRegMap.h"
#include "llvm_include_llvm_Support_Debug.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "regalloc"

// Compare VirtRegMap::getRegAllocPref().
AllocationOrder AllocationOrder::create(unsigned VirtReg, const VirtRegMap &VRM,
                                        const RegisterClassInfo &RegClassInfo,
                                        const LiveRegMatrix *Matrix) {
  const MachineFunction &MF = VRM.getMachineFunction();
  const TargetRegisterInfo *TRI = &VRM.getTargetRegInfo();
  auto Order = RegClassInfo.getOrder(MF.getRegInfo().getRegClass(VirtReg));
  SmallVector<MCPhysReg, 16> Hints;
  bool HardHints =
      TRI->getRegAllocationHints(VirtReg, Order, Hints, MF, &VRM, Matrix);

  LLVM_DEBUG({
    if (!Hints.empty()) {
      dbgs() << "hints:";
      for (unsigned I = 0, E = Hints.size(); I != E; ++I)
        dbgs() << ' ' << printReg(Hints[I], TRI);
      dbgs() << '\n';
    }
  });
#ifndef NDEBUG
  for (unsigned I = 0, E = Hints.size(); I != E; ++I)
    assert(is_contained(Order, Hints[I]) &&
           "Target hint is outside allocation order.");
#endif
  return AllocationOrder(std::move(Hints), Order, HardHints);
}
