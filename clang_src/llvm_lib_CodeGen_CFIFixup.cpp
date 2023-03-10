//===------ CFIFixup.cpp - Insert CFI remember/restore instructions -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//

// This pass inserts the necessary  instructions to adjust for the inconsistency
// of the call-frame information caused by final machine basic block layout.
// The pass relies in constraints LLVM imposes on the placement of
// save/restore points (cf. ShrinkWrap):
// * there is a single basic block, containing the function prologue
// * possibly multiple epilogue blocks, where each epilogue block is
//   complete and self-contained, i.e. CSR restore instructions (and the
//   corresponding CFI instructions are not split across two or more blocks.
// * prologue and epilogue blocks are outside of any loops
// Thus, during execution, at the beginning and at the end of each basic block
// the function can be in one of two states:
//  - "has a call frame", if the function has executed the prologue, and
//    has not executed any epilogue
//  - "does not have a call frame", if the function has not executed the
//    prologue, or has executed an epilogue
// which can be computed by a single RPO traversal.

// In order to accommodate backends which do not generate unwind info in
// epilogues we compute an additional property "strong no call frame on entry",
// which is set for the entry point of the function and for every block
// reachable from the entry along a path that does not execute the prologue. If
// this property holds, it takes precedence over the "has a call frame"
// property.

// From the point of view of the unwind tables, the "has/does not have call
// frame" state at beginning of each block is determined by the state at the end
// of the previous block, in layout order. Where these states differ, we insert
// compensating CFI instructions, which come in two flavours:

//   - CFI instructions, which reset the unwind table state to the initial one.
//     This is done by a target specific hook and is expected to be trivial
//     to implement, for example it could be:
//       .cfi_def_cfa <sp>, 0
//       .cfi_same_value <rN>
//       .cfi_same_value <rN-1>
//       ...
//     where <rN> are the callee-saved registers.
//   - CFI instructions, which reset the unwind table state to the one
//     created by the function prologue. These are
//       .cfi_restore_state
//       .cfi_remember_state
//     In this case we also insert a `.cfi_remember_state` after the last CFI
//     instruction in the function prologue.
//
// Known limitations:
//  * the pass cannot handle an epilogue preceding the prologue in the basic
//    block layout
//  * the pass does not handle functions where SP is used as a frame pointer and
//    SP adjustments up and down are done in different basic blocks (TODO)
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_CodeGen_CFIFixup.h"

#include "llvm_include_llvm_ADT_PostOrderIterator.h"
#include "llvm_include_llvm_ADT_SmallBitVector.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_CodeGen_TargetFrameLowering.h"
#include "llvm_include_llvm_CodeGen_TargetInstrInfo.h"
#include "llvm_include_llvm_CodeGen_TargetSubtargetInfo.h"
#include "llvm_include_llvm_MC_MCAsmInfo.h"
#include "llvm_include_llvm_MC_MCDwarf.h"
#include "llvm_include_llvm_Target_TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "cfi-fixup"

char CFIFixup::ID = 0;

INITIALIZE_PASS(CFIFixup, "cfi-fixup",
                "Insert CFI remember/restore state instructions", false, false)
FunctionPass *llvm::createCFIFixup() { return new CFIFixup(); }

static bool isPrologueCFIInstruction(const MachineInstr &MI) {
  return MI.getOpcode() == TargetOpcode::CFI_INSTRUCTION &&
         MI.getFlag(MachineInstr::FrameSetup);
}

static bool containsPrologue(const MachineBasicBlock &MBB) {
  return llvm::any_of(MBB.instrs(), isPrologueCFIInstruction);
}

static bool containsEpilogue(const MachineBasicBlock &MBB) {
  return llvm::any_of(llvm::reverse(MBB), [](const auto &MI) {
    return MI.getOpcode() == TargetOpcode::CFI_INSTRUCTION &&
           MI.getFlag(MachineInstr::FrameDestroy);
  });
}

bool CFIFixup::runOnMachineFunction(MachineFunction &MF) {
  const TargetFrameLowering &TFL = *MF.getSubtarget().getFrameLowering();
  if (!TFL.enableCFIFixup(MF))
    return false;

  const unsigned NumBlocks = MF.getNumBlockIDs();
  if (NumBlocks < 2)
    return false;

  struct BlockFlags {
    bool Reachable : 1;
    bool StrongNoFrameOnEntry : 1;
    bool HasFrameOnEntry : 1;
    bool HasFrameOnExit : 1;
  };
  SmallVector<BlockFlags, 32> BlockInfo(NumBlocks, {false, false, false, false});
  BlockInfo[0].Reachable = true;
  BlockInfo[0].StrongNoFrameOnEntry = true;

  // Compute the presence/absence of frame at each basic block.
  MachineBasicBlock *PrologueBlock = nullptr;
  ReversePostOrderTraversal<MachineBasicBlock *> RPOT(&*MF.begin());
  for (MachineBasicBlock *MBB : RPOT) {
    BlockFlags &Info = BlockInfo[MBB->getNumber()];

    // Set to true if the current block contains the prologue or the epilogue,
    // respectively.
    bool HasPrologue = false;
    bool HasEpilogue = false;

    if (!PrologueBlock && !Info.HasFrameOnEntry && containsPrologue(*MBB)) {
      PrologueBlock = MBB;
      HasPrologue = true;
    }

    if (Info.HasFrameOnEntry || HasPrologue)
      HasEpilogue = containsEpilogue(*MBB);

    // If the function has a call frame at the entry of the current block or the
    // current block contains the prologue, then the function has a call frame
    // at the exit of the block, unless the block contains the epilogue.
    Info.HasFrameOnExit = (Info.HasFrameOnEntry || HasPrologue) && !HasEpilogue;

    // Set the successors' state on entry.
    for (MachineBasicBlock *Succ : MBB->successors()) {
      BlockFlags &SuccInfo = BlockInfo[Succ->getNumber()];
      SuccInfo.Reachable = true;
      SuccInfo.StrongNoFrameOnEntry |=
          Info.StrongNoFrameOnEntry && !HasPrologue;
      SuccInfo.HasFrameOnEntry = Info.HasFrameOnExit;
    }
  }

  if (!PrologueBlock)
    return false;

  // Walk the blocks of the function in "physical" order.
  // Every block inherits the frame state (as recorded in the unwind tables)
  // of the previous block. If the intended frame state is different, insert
  // compensating CFI instructions.
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  bool Change = false;
  // `InsertPt` always points to the point in a preceding block where we have to
  // insert a `.cfi_remember_state`, in the case that the current block needs a
  // `.cfi_restore_state`.
  MachineBasicBlock *InsertMBB = PrologueBlock;
  MachineBasicBlock::iterator InsertPt = PrologueBlock->begin();
  for (MachineInstr &MI : *PrologueBlock)
    if (isPrologueCFIInstruction(MI))
      InsertPt = std::next(MI.getIterator());

  assert(InsertPt != PrologueBlock->begin() &&
         "Inconsistent notion of \"prologue block\"");

  // No point starting before the prologue block.
  // TODO: the unwind tables will still be incorrect if an epilogue physically
  // preceeds the prologue.
  MachineFunction::iterator CurrBB = std::next(PrologueBlock->getIterator());
  bool HasFrame = BlockInfo[PrologueBlock->getNumber()].HasFrameOnExit;
  while (CurrBB != MF.end()) {
    const BlockFlags &Info = BlockInfo[CurrBB->getNumber()];
    if (!Info.Reachable) {
      ++CurrBB;
      continue;
    }

#ifndef NDEBUG
    if (!Info.StrongNoFrameOnEntry) {
      for (auto *Pred : CurrBB->predecessors()) {
        BlockFlags &PredInfo = BlockInfo[Pred->getNumber()];
        assert((!PredInfo.Reachable ||
                Info.HasFrameOnEntry == PredInfo.HasFrameOnExit) &&
               "Inconsistent call frame state");
      }
    }
#endif
    if (!Info.StrongNoFrameOnEntry && Info.HasFrameOnEntry && !HasFrame) {
      // Reset to the "after prologue" state.

      // Insert a `.cfi_remember_state` into the last block known to have a
      // stack frame.
      unsigned CFIIndex =
          MF.addFrameInst(MCCFIInstruction::createRememberState(nullptr));
      BuildMI(*InsertMBB, InsertPt, DebugLoc(),
              TII.get(TargetOpcode::CFI_INSTRUCTION))
          .addCFIIndex(CFIIndex);
      // Insert a `.cfi_restore_state` at the beginning of the current block.
      CFIIndex = MF.addFrameInst(MCCFIInstruction::createRestoreState(nullptr));
      InsertPt = BuildMI(*CurrBB, CurrBB->begin(), DebugLoc(),
                         TII.get(TargetOpcode::CFI_INSTRUCTION))
                     .addCFIIndex(CFIIndex);
      ++InsertPt;
      InsertMBB = &*CurrBB;
      Change = true;
    } else if ((Info.StrongNoFrameOnEntry || !Info.HasFrameOnEntry) &&
               HasFrame) {
      // Reset to the state upon function entry.
      TFL.resetCFIToInitialState(*CurrBB);
      Change = true;
    }

    HasFrame = Info.HasFrameOnExit;
    ++CurrBB;
  }

  return Change;
}
