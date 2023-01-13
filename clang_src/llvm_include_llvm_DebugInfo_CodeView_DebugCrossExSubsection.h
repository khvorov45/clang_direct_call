//===- DebugCrossExSubsection.h ---------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_DEBUGCROSSEXSUBSECTION_H
#define LLVM_DEBUGINFO_CODEVIEW_DEBUGCROSSEXSUBSECTION_H

#include "llvm_include_llvm_DebugInfo_CodeView_CodeView.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugSubsection.h"
#include "llvm_include_llvm_Support_BinaryStreamArray.h"
#include "llvm_include_llvm_Support_BinaryStreamRef.h"
#include "llvm_include_llvm_Support_Error.h"
#include <cstdint>
#include <map>

namespace llvm {
class BinaryStreamReader;
class BinaryStreamWriter;
namespace codeview {

class DebugCrossModuleExportsSubsectionRef final : public DebugSubsectionRef {
  using ReferenceArray = FixedStreamArray<CrossModuleExport>;
  using Iterator = ReferenceArray::Iterator;

public:
  DebugCrossModuleExportsSubsectionRef()
      : DebugSubsectionRef(DebugSubsectionKind::CrossScopeExports) {}

  static bool classof(const DebugSubsectionRef *S) {
    return S->kind() == DebugSubsectionKind::CrossScopeExports;
  }

  Error initialize(BinaryStreamReader Reader);
  Error initialize(BinaryStreamRef Stream);

  Iterator begin() const { return References.begin(); }
  Iterator end() const { return References.end(); }

private:
  FixedStreamArray<CrossModuleExport> References;
};

class DebugCrossModuleExportsSubsection final : public DebugSubsection {
public:
  DebugCrossModuleExportsSubsection()
      : DebugSubsection(DebugSubsectionKind::CrossScopeExports) {}

  static bool classof(const DebugSubsection *S) {
    return S->kind() == DebugSubsectionKind::CrossScopeExports;
  }

  void addMapping(uint32_t Local, uint32_t Global);

  uint32_t calculateSerializedSize() const override;
  Error commit(BinaryStreamWriter &Writer) const override;

private:
  std::map<uint32_t, uint32_t> Mappings;
};

} // end namespace codeview
} // end namespace llvm

#endif // LLVM_DEBUGINFO_CODEVIEW_DEBUGCROSSEXSUBSECTION_H
