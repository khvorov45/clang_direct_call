//==- X86ReturnThunks.cpp - Replace rets with thunks or inline thunks --=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
///
/// Pass that replaces ret instructions with a jmp to __x86_return_thunk.
///
/// This corresponds to -mfunction-return=thunk-extern or
/// __attribute__((function_return("thunk-extern").
///
/// This pass is a minimal implementation necessary to help mitigate
/// RetBleed for the Linux kernel.
///
/// Should support for thunk or thunk-inline be necessary in the future, then
/// this pass should be combined with x86-retpoline-thunks which already has
/// machinery to emit thunks. Until then, YAGNI.
///
/// This pass is very similar to x86-lvi-ret.
///
//===----------------------------------------------------------------------===//

#include "llvm_lib_Target_X86_X86.h"
#include "llvm_lib_Target_X86_X86InstrInfo.h"
#include "llvm_lib_Target_X86_X86Subtarget.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_ADT_Triple.h"
#include "llvm_include_llvm_CodeGen_MachineBasicBlock.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"
#include "llvm_include_llvm_CodeGen_MachineInstr.h"
#include "llvm_include_llvm_CodeGen_MachineInstrBuilder.h"
#include "llvm_include_llvm_CodeGen_MachineModuleInfo.h"
#include "llvm_include_llvm_MC_MCInstrDesc.h"
#include "llvm_include_llvm_Support_Debug.h"

using namespace llvm;

#define PASS_KEY "x86-return-thunks"
#define DEBUG_TYPE PASS_KEY

struct X86ReturnThunks final : public MachineFunctionPass {
  static char ID;
  X86ReturnThunks() : MachineFunctionPass(ID) {}
  StringRef getPassName() const override { return "X86 Return Thunks"; }
  bool runOnMachineFunction(MachineFunction &MF) override;
};

char X86ReturnThunks::ID = 0;

bool X86ReturnThunks::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG(dbgs() << getPassName() << "\n");

  bool Modified = false;

  if (!MF.getFunction().hasFnAttribute(llvm::Attribute::FnRetThunkExtern))
    return Modified;

  StringRef ThunkName = "__x86_return_thunk";
  if (MF.getFunction().getName() == ThunkName)
    return Modified;

  const auto &ST = MF.getSubtarget<X86Subtarget>();
  const bool Is64Bit = ST.getTargetTriple().getArch() == Triple::x86_64;
  const unsigned RetOpc = Is64Bit ? X86::RET64 : X86::RET32;
  SmallVector<MachineInstr *, 16> Rets;

  for (MachineBasicBlock &MBB : MF)
    for (MachineInstr &Term : MBB.terminators())
      if (Term.getOpcode() == RetOpc)
        Rets.push_back(&Term);

  bool IndCS =
      MF.getMMI().getModule()->getModuleFlag("indirect_branch_cs_prefix");
  const MCInstrDesc &CS = ST.getInstrInfo()->get(X86::CS_PREFIX);
  const MCInstrDesc &JMP = ST.getInstrInfo()->get(X86::TAILJMPd);

  for (MachineInstr *Ret : Rets) {
    if (IndCS)
      BuildMI(Ret->getParent(), Ret->getDebugLoc(), CS);
    BuildMI(Ret->getParent(), Ret->getDebugLoc(), JMP)
        .addExternalSymbol(ThunkName.data());
    Ret->eraseFromParent();
    Modified = true;
  }

  return Modified;
}

INITIALIZE_PASS(X86ReturnThunks, PASS_KEY, "X86 Return Thunks", false, false)

FunctionPass *llvm::createX86ReturnThunksPass() {
  return new X86ReturnThunks();
}
