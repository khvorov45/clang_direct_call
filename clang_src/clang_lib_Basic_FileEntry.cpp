//===- FileEntry.cpp - File references --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
/// \file
/// Defines implementation for clang::FileEntry and clang::FileEntryRef.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_Basic_FileEntry.h"
#include "llvm_include_llvm_Support_MemoryBuffer.h"
#include "llvm_include_llvm_Support_VirtualFileSystem.h"

using namespace clang;

FileEntry::FileEntry() : UniqueID(0, 0) {}

FileEntry::~FileEntry() = default;

void FileEntry::closeFile() const { File.reset(); }
