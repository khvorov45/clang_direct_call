//===- TargetSelect.h - Target Selection & Registration ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides utilities to make sure that certain classes of targets are
// linked into the main application executable, and initialize them as
// appropriate.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_TARGETSELECT_H
#define LLVM_SUPPORT_TARGETSELECT_H

#include "llvm_lib_Target_X86_TargetInfo_X86TargetInfo.h"
#include "llvm_include_llvm_MC_TargetRegistry.h"
#include "llvm_include_llvm_Config_llvm-config.h"

extern "C" {
  // Declare all of the target-initialization functions that are available.
#define LLVM_TARGET(TargetName) void LLVMInitialize##TargetName##TargetInfo();
#include "llvm_include_llvm_Config_Targets.def"

  // Declare all of the target-MC-initialization functions that are available.
#define LLVM_TARGET(TargetName) void LLVMInitialize##TargetName##TargetMC();
#include "llvm_include_llvm_Config_Targets.def"

  // Declare all of the available assembly printer initialization functions.
#define LLVM_ASM_PRINTER(TargetName) void LLVMInitialize##TargetName##AsmPrinter();
#include "llvm_include_llvm_Config_AsmPrinters.def"

  // Declare all of the available assembly parser initialization functions.
  void LLVMInitializeX86AsmParser();

  // Declare all of the available disassembler initialization functions.
#define LLVM_DISASSEMBLER(TargetName) \
  void LLVMInitialize##TargetName##Disassembler();
#include "llvm_include_llvm_Config_Disassemblers.def"

// Declare all of the available TargetMCA initialization functions.
#define LLVM_TARGETMCA(TargetName) void LLVMInitialize##TargetName##TargetMCA();
#include "llvm_include_llvm_Config_TargetMCAs.def"
}

namespace llvm {

  /// InitializeAllTargetMCs - The main program should call this function if it
  /// wants access to all available target MC that LLVM is configured to
  /// support, to make them available via the TargetRegistry.
  ///
  /// It is legal for a client to make multiple calls to this function.
  inline void InitializeAllTargetMCs() {
#define LLVM_TARGET(TargetName) LLVMInitialize##TargetName##TargetMC();
#include "llvm_include_llvm_Config_Targets.def"
  }

  /// InitializeAllAsmPrinters - The main program should call this function if
  /// it wants all asm printers that LLVM is configured to support, to make them
  /// available via the TargetRegistry.
  ///
  /// It is legal for a client to make multiple calls to this function.
  inline void InitializeAllAsmPrinters() {
#define LLVM_ASM_PRINTER(TargetName) LLVMInitialize##TargetName##AsmPrinter();
#include "llvm_include_llvm_Config_AsmPrinters.def"
  }

  /// InitializeAllAsmParsers - The main program should call this function if it
  /// wants all asm parsers that LLVM is configured to support, to make them
  /// available via the TargetRegistry.
  ///
  /// It is legal for a client to make multiple calls to this function.
  inline void InitializeAllAsmParsers() {
    LLVMInitializeX86AsmParser();
  }

  /// InitializeAllDisassemblers - The main program should call this function if
  /// it wants all disassemblers that LLVM is configured to support, to make
  /// them available via the TargetRegistry.
  ///
  /// It is legal for a client to make multiple calls to this function.
  inline void InitializeAllDisassemblers() {
#define LLVM_DISASSEMBLER(TargetName) LLVMInitialize##TargetName##Disassembler();
#include "llvm_include_llvm_Config_Disassemblers.def"
  }

  /// InitializeNativeTarget - The main program should call this function to
  /// initialize the native target corresponding to the host.  This is useful
  /// for JIT applications to ensure that the target gets linked in correctly.
  ///
  /// It is legal for a client to make multiple calls to this function.
  inline bool InitializeNativeTarget() {
  // If we have a native target, initialize it to ensure it is linked in.
#ifdef LLVM_NATIVE_TARGET
    LLVM_NATIVE_TARGETINFO();
    LLVM_NATIVE_TARGET();
    LLVM_NATIVE_TARGETMC();
    return false;
#else
    return true;
#endif
  }

  /// InitializeNativeTargetAsmPrinter - The main program should call
  /// this function to initialize the native target asm printer.
  inline bool InitializeNativeTargetAsmPrinter() {
  // If we have a native target, initialize the corresponding asm printer.
#ifdef LLVM_NATIVE_ASMPRINTER
    LLVM_NATIVE_ASMPRINTER();
    return false;
#else
    return true;
#endif
  }

  /// InitializeNativeTargetAsmParser - The main program should call
  /// this function to initialize the native target asm parser.
  inline bool InitializeNativeTargetAsmParser() {
  // If we have a native target, initialize the corresponding asm parser.
#ifdef LLVM_NATIVE_ASMPARSER
    LLVM_NATIVE_ASMPARSER();
    return false;
#else
    return true;
#endif
  }

  /// InitializeNativeTargetDisassembler - The main program should call
  /// this function to initialize the native target disassembler.
  inline bool InitializeNativeTargetDisassembler() {
  // If we have a native target, initialize the corresponding disassembler.
#ifdef LLVM_NATIVE_DISASSEMBLER
    LLVM_NATIVE_DISASSEMBLER();
    return false;
#else
    return true;
#endif
  }

  /// InitializeAllTargetMCAs - The main program should call
  /// this function to initialize the target CustomBehaviour and
  /// InstrPostProcess classes.
  inline void InitializeAllTargetMCAs() {
#define LLVM_TARGETMCA(TargetName) LLVMInitialize##TargetName##TargetMCA();
#include "llvm_include_llvm_Config_TargetMCAs.def"
  }
}

#endif
