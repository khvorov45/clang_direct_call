//===- lib/MC/MCTargetOptions.cpp - MC Target Options ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCTargetOptions.h"
#include "llvm_include_llvm_ADT_StringRef.h"

using namespace llvm;

MCTargetOptions::MCTargetOptions()
    : MCRelaxAll(false), MCNoExecStack(false), MCFatalWarnings(false),
      MCNoWarn(false), MCNoDeprecatedWarn(false), MCNoTypeCheck(false),
      MCSaveTempLabels(false), MCIncrementalLinkerCompatible(false),
      ShowMCEncoding(false), ShowMCInst(false), AsmVerbose(false),
      PreserveAsmComments(true), Dwarf64(false),
      EmitDwarfUnwind(EmitDwarfUnwindType::Default),
      MCUseDwarfDirectory(DefaultDwarfDirectory) {}

StringRef MCTargetOptions::getABIName() const {
  return ABIName;
}

StringRef MCTargetOptions::getAssemblyLanguage() const {
  return AssemblyLanguage;
}
