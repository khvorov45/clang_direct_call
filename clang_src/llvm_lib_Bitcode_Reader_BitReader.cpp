//===-- BitReader.cpp -----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm-c_BitReader.h"
#include "llvm_include_llvm-c_Core.h"
#include "llvm_include_llvm_Bitcode_BitcodeReader.h"
#include "llvm_include_llvm_IR_LLVMContext.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_Support_MemoryBuffer.h"
#include <cstring>
#include <string>

using namespace llvm;

/* Builds a module from the bitcode in the specified memory buffer, returning a
   reference to the module via the OutModule parameter. Returns 0 on success.
   Optionally returns a human-readable error message via OutMessage. */
LLVMBool LLVMParseBitcode(LLVMMemoryBufferRef MemBuf, LLVMModuleRef *OutModule,
                          char **OutMessage) {
  return LLVMParseBitcodeInContext(LLVMGetGlobalContext(), MemBuf, OutModule,
                                   OutMessage);
}

LLVMBool LLVMParseBitcode2(LLVMMemoryBufferRef MemBuf,
                           LLVMModuleRef *OutModule) {
  return LLVMParseBitcodeInContext2(LLVMGetGlobalContext(), MemBuf, OutModule);
}

LLVMBool LLVMParseBitcodeInContext(LLVMContextRef ContextRef,
                                   LLVMMemoryBufferRef MemBuf,
                                   LLVMModuleRef *OutModule,
                                   char **OutMessage) {
  MemoryBufferRef Buf = unwrap(MemBuf)->getMemBufferRef();
  LLVMContext &Ctx = *unwrap(ContextRef);

  Expected<std::unique_ptr<Module>> ModuleOrErr = parseBitcodeFile(Buf, Ctx);
  if (Error Err = ModuleOrErr.takeError()) {
    std::string Message;
    handleAllErrors(std::move(Err), [&](ErrorInfoBase &EIB) {
      Message = EIB.message();
    });
    if (OutMessage)
      *OutMessage = strdup(Message.c_str());
    *OutModule = wrap((Module *)nullptr);
    return 1;
  }

  *OutModule = wrap(ModuleOrErr.get().release());
  return 0;
}

LLVMBool LLVMParseBitcodeInContext2(LLVMContextRef ContextRef,
                                    LLVMMemoryBufferRef MemBuf,
                                    LLVMModuleRef *OutModule) {
  MemoryBufferRef Buf = unwrap(MemBuf)->getMemBufferRef();
  LLVMContext &Ctx = *unwrap(ContextRef);

  ErrorOr<std::unique_ptr<Module>> ModuleOrErr =
      expectedToErrorOrAndEmitErrors(Ctx, parseBitcodeFile(Buf, Ctx));
  if (ModuleOrErr.getError()) {
    *OutModule = wrap((Module *)nullptr);
    return 1;
  }

  *OutModule = wrap(ModuleOrErr.get().release());
  return 0;
}

/* Reads a module from the specified path, returning via the OutModule parameter
   a module provider which performs lazy deserialization. Returns 0 on success.
   Optionally returns a human-readable error message via OutMessage. */
LLVMBool LLVMGetBitcodeModuleInContext(LLVMContextRef ContextRef,
                                       LLVMMemoryBufferRef MemBuf,
                                       LLVMModuleRef *OutM, char **OutMessage) {
  LLVMContext &Ctx = *unwrap(ContextRef);
  std::unique_ptr<MemoryBuffer> Owner(unwrap(MemBuf));
  Expected<std::unique_ptr<Module>> ModuleOrErr =
      getOwningLazyBitcodeModule(std::move(Owner), Ctx);
  // Release the buffer if we didn't take ownership of it since we never owned
  // it anyway.
  (void)Owner.release();

  if (Error Err = ModuleOrErr.takeError()) {
    std::string Message;
    handleAllErrors(std::move(Err), [&](ErrorInfoBase &EIB) {
      Message = EIB.message();
    });
    if (OutMessage)
      *OutMessage = strdup(Message.c_str());
    *OutM = wrap((Module *)nullptr);
    return 1;
  }

  *OutM = wrap(ModuleOrErr.get().release());

  return 0;
}

LLVMBool LLVMGetBitcodeModuleInContext2(LLVMContextRef ContextRef,
                                        LLVMMemoryBufferRef MemBuf,
                                        LLVMModuleRef *OutM) {
  LLVMContext &Ctx = *unwrap(ContextRef);
  std::unique_ptr<MemoryBuffer> Owner(unwrap(MemBuf));

  ErrorOr<std::unique_ptr<Module>> ModuleOrErr = expectedToErrorOrAndEmitErrors(
      Ctx, getOwningLazyBitcodeModule(std::move(Owner), Ctx));
  Owner.release();

  if (ModuleOrErr.getError()) {
    *OutM = wrap((Module *)nullptr);
    return 1;
  }

  *OutM = wrap(ModuleOrErr.get().release());
  return 0;
}

LLVMBool LLVMGetBitcodeModule(LLVMMemoryBufferRef MemBuf, LLVMModuleRef *OutM,
                              char **OutMessage) {
  return LLVMGetBitcodeModuleInContext(LLVMGetGlobalContext(), MemBuf, OutM,
                                       OutMessage);
}

LLVMBool LLVMGetBitcodeModule2(LLVMMemoryBufferRef MemBuf,
                               LLVMModuleRef *OutM) {
  return LLVMGetBitcodeModuleInContext2(LLVMGetGlobalContext(), MemBuf, OutM);
}
