//===-- BitWriter.cpp -----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm-c_BitWriter.h"
#include "llvm_include_llvm_Bitcode_BitcodeWriter.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_Support_FileSystem.h"
#include "llvm_include_llvm_Support_MemoryBuffer.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
using namespace llvm;


/*===-- Operations on modules ---------------------------------------------===*/

int LLVMWriteBitcodeToFile(LLVMModuleRef M, const char *Path) {
  std::error_code EC;
  raw_fd_ostream OS(Path, EC, sys::fs::OF_None);

  if (EC)
    return -1;

  WriteBitcodeToFile(*unwrap(M), OS);
  return 0;
}

int LLVMWriteBitcodeToFD(LLVMModuleRef M, int FD, int ShouldClose,
                         int Unbuffered) {
  raw_fd_ostream OS(FD, ShouldClose, Unbuffered);

  WriteBitcodeToFile(*unwrap(M), OS);
  return 0;
}

int LLVMWriteBitcodeToFileHandle(LLVMModuleRef M, int FileHandle) {
  return LLVMWriteBitcodeToFD(M, FileHandle, true, false);
}

LLVMMemoryBufferRef LLVMWriteBitcodeToMemoryBuffer(LLVMModuleRef M) {
  std::string Data;
  raw_string_ostream OS(Data);

  WriteBitcodeToFile(*unwrap(M), OS);
  return wrap(MemoryBuffer::getMemBufferCopy(OS.str()).release());
}
