//===- MCAsmInfoELF.cpp - ELF asm properties ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines target asm properties related what form asm statements
// should take in general on ELF-based targets
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCAsmInfoELF.h"
#include "llvm_include_llvm_BinaryFormat_ELF.h"
#include "llvm_include_llvm_MC_MCContext.h"
#include "llvm_include_llvm_MC_MCSectionELF.h"

using namespace llvm;

void MCAsmInfoELF::anchor() {}

MCSection *MCAsmInfoELF::getNonexecutableStackSection(MCContext &Ctx) const {
  return Ctx.getELFSection(".note.GNU-stack", ELF::SHT_PROGBITS, 0);
}

MCAsmInfoELF::MCAsmInfoELF() {
  HasIdentDirective = true;
  WeakRefDirective = "\t.weak\t";
  PrivateGlobalPrefix = ".L";
  PrivateLabelPrefix = ".L";
}
