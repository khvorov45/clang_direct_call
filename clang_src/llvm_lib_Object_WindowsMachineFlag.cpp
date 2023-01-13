//===- WindowsMachineFlag.cpp ---------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Functions for implementing the /machine: flag.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Object_WindowsMachineFlag.h"

#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_ADT_StringSwitch.h"
#include "llvm_include_llvm_BinaryFormat_COFF.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"

using namespace llvm;

// Returns /machine's value.
COFF::MachineTypes llvm::getMachineType(StringRef S) {
  return StringSwitch<COFF::MachineTypes>(S.lower())
      .Cases("x64", "amd64", COFF::IMAGE_FILE_MACHINE_AMD64)
      .Cases("x86", "i386", COFF::IMAGE_FILE_MACHINE_I386)
      .Case("arm", COFF::IMAGE_FILE_MACHINE_ARMNT)
      .Case("arm64", COFF::IMAGE_FILE_MACHINE_ARM64)
      .Case("arm64ec", COFF::IMAGE_FILE_MACHINE_ARM64EC)
      .Default(COFF::IMAGE_FILE_MACHINE_UNKNOWN);
}

StringRef llvm::machineToStr(COFF::MachineTypes MT) {
  switch (MT) {
  case COFF::IMAGE_FILE_MACHINE_ARMNT:
    return "arm";
  case COFF::IMAGE_FILE_MACHINE_ARM64:
    return "arm64";
  case COFF::IMAGE_FILE_MACHINE_ARM64EC:
    return "arm64ec";
  case COFF::IMAGE_FILE_MACHINE_AMD64:
    return "x64";
  case COFF::IMAGE_FILE_MACHINE_I386:
    return "x86";
  default:
    llvm_unreachable("unknown machine type");
  }
}
