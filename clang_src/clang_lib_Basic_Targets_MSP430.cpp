//===--- MSP430.cpp - Implement MSP430 target feature support -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements MSP430 TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Basic_Targets_MSP430.h"
#include "clang_include_clang_Basic_MacroBuilder.h"

using namespace clang;
using namespace clang::targets;

const char *const MSP430TargetInfo::GCCRegNames[] = {
    "r0", "r1", "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
};

ArrayRef<const char *> MSP430TargetInfo::getGCCRegNames() const {
  return llvm::makeArrayRef(GCCRegNames);
}

void MSP430TargetInfo::getTargetDefines(const LangOptions &Opts,
                                        MacroBuilder &Builder) const {
  Builder.defineMacro("MSP430");
  Builder.defineMacro("__MSP430__");
  Builder.defineMacro("__ELF__");
  // FIXME: defines for different 'flavours' of MCU
}
