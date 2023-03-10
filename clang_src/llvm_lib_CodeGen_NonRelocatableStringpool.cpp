//===-- NonRelocatableStringpool.cpp --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_CodeGen_NonRelocatableStringpool.h"
#include "llvm_include_llvm_ADT_STLExtras.h"

namespace llvm {

DwarfStringPoolEntryRef NonRelocatableStringpool::getEntry(StringRef S) {
  if (S.empty() && !Strings.empty())
    return EmptyString;

  if (Translator)
    S = Translator(S);
  auto I = Strings.insert({S, DwarfStringPoolEntry()});
  auto &Entry = I.first->second;
  if (I.second || !Entry.isIndexed()) {
    Entry.Index = NumEntries++;
    Entry.Offset = CurrentEndOffset;
    Entry.Symbol = nullptr;
    CurrentEndOffset += S.size() + 1;
  }
  return DwarfStringPoolEntryRef(*I.first);
}

StringRef NonRelocatableStringpool::internString(StringRef S) {
  DwarfStringPoolEntry Entry{nullptr, 0, DwarfStringPoolEntry::NotIndexed};

  if (Translator)
    S = Translator(S);

  auto InsertResult = Strings.insert({S, Entry});
  return InsertResult.first->getKey();
}

std::vector<DwarfStringPoolEntryRef>
NonRelocatableStringpool::getEntriesForEmission() const {
  std::vector<DwarfStringPoolEntryRef> Result;
  Result.reserve(Strings.size());
  for (const auto &E : Strings)
    if (E.getValue().isIndexed())
      Result.emplace_back(E);
  llvm::sort(Result, [](const DwarfStringPoolEntryRef A,
                        const DwarfStringPoolEntryRef B) {
    return A.getIndex() < B.getIndex();
  });
  return Result;
}

} // namespace llvm
