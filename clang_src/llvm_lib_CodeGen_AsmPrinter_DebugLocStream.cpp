//===- DebugLocStream.cpp - DWARF debug_loc stream --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_lib_CodeGen_AsmPrinter_DebugLocStream.h"
#include "llvm_lib_CodeGen_AsmPrinter_DwarfDebug.h"
#include "llvm_include_llvm_CodeGen_AsmPrinter.h"

using namespace llvm;

bool DebugLocStream::finalizeList(AsmPrinter &Asm) {
  if (Lists.back().EntryOffset == Entries.size()) {
    // Empty list.  Delete it.
    Lists.pop_back();
    return false;
  }

  // Real list.  Generate a label for it.
  Lists.back().Label = Asm.createTempSymbol("debug_loc");
  return true;
}

void DebugLocStream::finalizeEntry() {
  if (Entries.back().ByteOffset != DWARFBytes.size())
    return;

  // The last entry was empty.  Delete it.
  Comments.erase(Comments.begin() + Entries.back().CommentOffset,
                 Comments.end());
  Entries.pop_back();

  assert(Lists.back().EntryOffset <= Entries.size() &&
         "Popped off more entries than are in the list");
}

DebugLocStream::ListBuilder::~ListBuilder() {
  if (!Locs.finalizeList(Asm))
    return;
  V.initializeDbgValue(&MI);
  V.setDebugLocListIndex(ListIndex);
  if (TagOffset)
    V.setDebugLocListTagOffset(*TagOffset);
}
