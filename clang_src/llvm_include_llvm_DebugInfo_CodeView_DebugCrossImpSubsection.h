//===- DebugCrossImpSubsection.h --------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_DEBUGCROSSIMPSUBSECTION_H
#define LLVM_DEBUGINFO_CODEVIEW_DEBUGCROSSIMPSUBSECTION_H

#include "llvm_include_llvm_ADT_StringMap.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_DebugInfo_CodeView_CodeView.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugSubsection.h"
#include "llvm_include_llvm_Support_BinaryStreamArray.h"
#include "llvm_include_llvm_Support_BinaryStreamRef.h"
#include "llvm_include_llvm_Support_Endian.h"
#include "llvm_include_llvm_Support_Error.h"
#include <cstdint>
#include <vector>

namespace llvm {
class BinaryStreamReader;
class BinaryStreamWriter;

namespace codeview {

struct CrossModuleImportItem {
  const CrossModuleImport *Header = nullptr;
  FixedStreamArray<support::ulittle32_t> Imports;
};

} // end namespace codeview

template <> struct VarStreamArrayExtractor<codeview::CrossModuleImportItem> {
public:
  using ContextType = void;

  Error operator()(BinaryStreamRef Stream, uint32_t &Len,
                   codeview::CrossModuleImportItem &Item);
};

namespace codeview {

class DebugStringTableSubsection;

class DebugCrossModuleImportsSubsectionRef final : public DebugSubsectionRef {
  using ReferenceArray = VarStreamArray<CrossModuleImportItem>;
  using Iterator = ReferenceArray::Iterator;

public:
  DebugCrossModuleImportsSubsectionRef()
      : DebugSubsectionRef(DebugSubsectionKind::CrossScopeImports) {}

  static bool classof(const DebugSubsectionRef *S) {
    return S->kind() == DebugSubsectionKind::CrossScopeImports;
  }

  Error initialize(BinaryStreamReader Reader);
  Error initialize(BinaryStreamRef Stream);

  Iterator begin() const { return References.begin(); }
  Iterator end() const { return References.end(); }

private:
  ReferenceArray References;
};

class DebugCrossModuleImportsSubsection final : public DebugSubsection {
public:
  explicit DebugCrossModuleImportsSubsection(
      DebugStringTableSubsection &Strings)
      : DebugSubsection(DebugSubsectionKind::CrossScopeImports),
        Strings(Strings) {}

  static bool classof(const DebugSubsection *S) {
    return S->kind() == DebugSubsectionKind::CrossScopeImports;
  }

  void addImport(StringRef Module, uint32_t ImportId);

  uint32_t calculateSerializedSize() const override;
  Error commit(BinaryStreamWriter &Writer) const override;

private:
  DebugStringTableSubsection &Strings;
  StringMap<std::vector<support::ulittle32_t>> Mappings;
};

} // end namespace codeview

} // end namespace llvm

#endif // LLVM_DEBUGINFO_CODEVIEW_DEBUGCROSSIMPSUBSECTION_H
