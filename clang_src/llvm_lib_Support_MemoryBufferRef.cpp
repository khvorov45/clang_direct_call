//===- MemoryBufferRef.cpp - Memory Buffer Reference ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements the MemoryBufferRef interface.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Support_MemoryBufferRef.h"
#include "llvm_include_llvm_Support_MemoryBuffer.h"

using namespace llvm;

MemoryBufferRef::MemoryBufferRef(const MemoryBuffer &Buffer)
    : Buffer(Buffer.getBuffer()), Identifier(Buffer.getBufferIdentifier()) {}
