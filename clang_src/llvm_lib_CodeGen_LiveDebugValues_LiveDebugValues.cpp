//===- LiveDebugValues.cpp - Tracking Debug Value MIs ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_lib_CodeGen_LiveDebugValues_LiveDebugValues.h"

#include "llvm_include_llvm_ADT_Triple.h"
#include "llvm_include_llvm_CodeGen_MachineDominators.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_CodeGen_TargetPassConfig.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_PassRegistry.h"
#include "llvm_include_llvm_Support_CommandLine.h"

/// \file LiveDebugValues.cpp
///
/// The LiveDebugValues pass extends the range of variable locations
/// (specified by DBG_VALUE instructions) from single blocks to successors
/// and any other code locations where the variable location is valid.
/// There are currently two implementations: the "VarLoc" implementation
/// explicitly tracks the location of a variable, while the "InstrRef"
/// implementation tracks the values defined by instructions through locations.
///
/// This file implements neither; it merely registers the pass, allows the
/// user to pick which implementation will be used to propagate variable
/// locations.

#define DEBUG_TYPE "livedebugvalues"

using namespace llvm;

static cl::opt<bool>
    ForceInstrRefLDV("force-instr-ref-livedebugvalues", cl::Hidden,
                     cl::desc("Use instruction-ref based LiveDebugValues with "
                              "normal DBG_VALUE inputs"),
                     cl::init(false));

static cl::opt<cl::boolOrDefault> ValueTrackingVariableLocations(
    "experimental-debug-variable-locations",
    cl::desc("Use experimental new value-tracking variable locations"));

// Options to prevent pathological compile-time behavior. If InputBBLimit and
// InputDbgValueLimit are both exceeded, range extension is disabled.
static cl::opt<unsigned> InputBBLimit(
    "livedebugvalues-input-bb-limit",
    cl::desc("Maximum input basic blocks before DBG_VALUE limit applies"),
    cl::init(10000), cl::Hidden);
static cl::opt<unsigned> InputDbgValueLimit(
    "livedebugvalues-input-dbg-value-limit",
    cl::desc(
        "Maximum input DBG_VALUE insts supported by debug range extension"),
    cl::init(50000), cl::Hidden);

namespace {
/// Generic LiveDebugValues pass. Calls through to VarLocBasedLDV or
/// InstrRefBasedLDV to perform location propagation, via the LDVImpl
/// base class.
class LiveDebugValues : public MachineFunctionPass {
public:
  static char ID;

  LiveDebugValues();
  ~LiveDebugValues() = default;

  /// Calculate the liveness information for the given machine function.
  bool runOnMachineFunction(MachineFunction &MF) override;

  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::NoVRegs);
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

private:
  std::unique_ptr<LDVImpl> InstrRefImpl;
  std::unique_ptr<LDVImpl> VarLocImpl;
  TargetPassConfig *TPC;
  MachineDominatorTree MDT;
};
} // namespace

char LiveDebugValues::ID = 0;

char &llvm::LiveDebugValuesID = LiveDebugValues::ID;

INITIALIZE_PASS(LiveDebugValues, DEBUG_TYPE, "Live DEBUG_VALUE analysis", false,
                false)

/// Default construct and initialize the pass.
LiveDebugValues::LiveDebugValues() : MachineFunctionPass(ID) {
  initializeLiveDebugValuesPass(*PassRegistry::getPassRegistry());
  InstrRefImpl =
      std::unique_ptr<LDVImpl>(llvm::makeInstrRefBasedLiveDebugValues());
  VarLocImpl = std::unique_ptr<LDVImpl>(llvm::makeVarLocBasedLiveDebugValues());
}

bool LiveDebugValues::runOnMachineFunction(MachineFunction &MF) {
  bool InstrRefBased = MF.useDebugInstrRef();
  // Allow the user to force selection of InstrRef LDV.
  InstrRefBased |= ForceInstrRefLDV;

  TPC = getAnalysisIfAvailable<TargetPassConfig>();
  LDVImpl *TheImpl = &*VarLocImpl;

  MachineDominatorTree *DomTree = nullptr;
  if (InstrRefBased) {
    DomTree = &MDT;
    MDT.calculate(MF);
    TheImpl = &*InstrRefImpl;
  }

  return TheImpl->ExtendRanges(MF, DomTree, TPC, InputBBLimit,
                               InputDbgValueLimit);
}

bool llvm::debuginfoShouldUseDebugInstrRef(const Triple &T) {
  // Enable by default on x86_64, disable if explicitly turned off on cmdline.
  if (T.getArch() == llvm::Triple::x86_64 &&
      ValueTrackingVariableLocations != cl::boolOrDefault::BOU_FALSE)
    return true;

  // Enable if explicitly requested on command line.
  return ValueTrackingVariableLocations == cl::boolOrDefault::BOU_TRUE;
}
