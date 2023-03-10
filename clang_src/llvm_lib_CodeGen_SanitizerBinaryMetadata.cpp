//===- SanitizerBinaryMetadata.cpp
//----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is a part of SanitizerBinaryMetadata.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Instrumentation_SanitizerBinaryMetadata.h"
#include "llvm_include_llvm_CodeGen_MachineFrameInfo.h"
#include "llvm_include_llvm_CodeGen_MachineFunction.h"
#include "llvm_include_llvm_CodeGen_MachineFunctionPass.h"
#include "llvm_include_llvm_CodeGen_Passes.h"
#include "llvm_include_llvm_IR_IRBuilder.h"
#include "llvm_include_llvm_IR_MDBuilder.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include <algorithm>

using namespace llvm;

class MachineSanitizerBinaryMetadata : public MachineFunctionPass {
public:
  static char ID;

  MachineSanitizerBinaryMetadata();
  bool runOnMachineFunction(MachineFunction &F) override;
};

INITIALIZE_PASS(MachineSanitizerBinaryMetadata, "machine-sanmd",
                "Machine Sanitizer Binary Metadata", false, false)

char MachineSanitizerBinaryMetadata::ID = 0;
char &llvm::MachineSanitizerBinaryMetadataID =
    MachineSanitizerBinaryMetadata::ID;

MachineSanitizerBinaryMetadata::MachineSanitizerBinaryMetadata()
    : MachineFunctionPass(ID) {
  initializeMachineSanitizerBinaryMetadataPass(
      *PassRegistry::getPassRegistry());
}

bool MachineSanitizerBinaryMetadata::runOnMachineFunction(MachineFunction &MF) {
  MDNode *MD = MF.getFunction().getMetadata(LLVMContext::MD_pcsections);
  if (!MD)
    return false;
  const auto &Section = *cast<MDString>(MD->getOperand(0));
  if (!Section.getString().equals(kSanitizerBinaryMetadataCoveredSection))
    return false;
  auto &AuxMDs = *cast<MDTuple>(MD->getOperand(1));
  // Assume it currently only has features.
  assert(AuxMDs.getNumOperands() == 1);
  auto *Features = cast<ConstantAsMetadata>(AuxMDs.getOperand(0))->getValue();
  if (!Features->getUniqueInteger()[kSanitizerBinaryMetadataUARBit])
    return false;
  // Calculate size of stack args for the function.
  int64_t Size = 0;
  uint64_t Align = 0;
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  for (int i = -1; i >= (int)-MFI.getNumFixedObjects(); --i) {
    Size = std::max(Size, MFI.getObjectOffset(i) + MFI.getObjectSize(i));
    Align = std::max(Align, MFI.getObjectAlign(i).value());
  }
  Size = (Size + Align - 1) & ~(Align - 1);
  auto &F = MF.getFunction();
  IRBuilder<> IRB(F.getContext());
  MDBuilder MDB(F.getContext());
  // Keep the features and append size of stack args to the metadata.
  F.setMetadata(LLVMContext::MD_pcsections,
                MDB.createPCSections(
                    {{Section.getString(), {Features, IRB.getInt32(Size)}}}));
  return false;
}
