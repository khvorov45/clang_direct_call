//===- SymbolRecordMapping.h ------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_SYMBOLRECORDMAPPING_H
#define LLVM_DEBUGINFO_CODEVIEW_SYMBOLRECORDMAPPING_H

#include "llvm_include_llvm_DebugInfo_CodeView_CodeViewRecordIO.h"
#include "llvm_include_llvm_DebugInfo_CodeView_SymbolVisitorCallbacks.h"

namespace llvm {
class BinaryStreamReader;
class BinaryStreamWriter;

namespace codeview {
class SymbolRecordMapping : public SymbolVisitorCallbacks {
public:
  explicit SymbolRecordMapping(BinaryStreamReader &Reader,
                               CodeViewContainer Container)
      : IO(Reader), Container(Container) {}
  explicit SymbolRecordMapping(BinaryStreamWriter &Writer,
                               CodeViewContainer Container)
      : IO(Writer), Container(Container) {}

  Error visitSymbolBegin(CVSymbol &Record) override;
  Error visitSymbolEnd(CVSymbol &Record) override;

#define SYMBOL_RECORD(EnumName, EnumVal, Name)                                 \
  Error visitKnownRecord(CVSymbol &CVR, Name &Record) override;
#define SYMBOL_RECORD_ALIAS(EnumName, EnumVal, Name, AliasName)
#include "llvm_include_llvm_DebugInfo_CodeView_CodeViewSymbols.def"

private:
  std::optional<SymbolKind> Kind;

  CodeViewRecordIO IO;
  CodeViewContainer Container;
};
}
}

#endif
