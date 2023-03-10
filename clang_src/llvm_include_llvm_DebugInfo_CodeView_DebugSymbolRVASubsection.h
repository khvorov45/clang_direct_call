//===- DebugSymbolRVASubsection.h -------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_DEBUGSYMBOLRVASUBSECTION_H
#define LLVM_DEBUGINFO_CODEVIEW_DEBUGSYMBOLRVASUBSECTION_H

#include "llvm_include_llvm_DebugInfo_CodeView_CodeView.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugSubsection.h"
#include "llvm_include_llvm_Support_BinaryStreamArray.h"
#include "llvm_include_llvm_Support_Endian.h"
#include "llvm_include_llvm_Support_Error.h"
#include <cstdint>
#include <vector>

namespace llvm {

class BinaryStreamReader;

namespace codeview {

class DebugSymbolRVASubsectionRef final : public DebugSubsectionRef {
public:
  using ArrayType = FixedStreamArray<support::ulittle32_t>;

  DebugSymbolRVASubsectionRef();

  static bool classof(const DebugSubsectionRef *S) {
    return S->kind() == DebugSubsectionKind::CoffSymbolRVA;
  }

  ArrayType::Iterator begin() const { return RVAs.begin(); }
  ArrayType::Iterator end() const { return RVAs.end(); }

  Error initialize(BinaryStreamReader &Reader);

private:
  ArrayType RVAs;
};

class DebugSymbolRVASubsection final : public DebugSubsection {
public:
  DebugSymbolRVASubsection();

  static bool classof(const DebugSubsection *S) {
    return S->kind() == DebugSubsectionKind::CoffSymbolRVA;
  }

  Error commit(BinaryStreamWriter &Writer) const override;
  uint32_t calculateSerializedSize() const override;

  void addRVA(uint32_t RVA) { RVAs.push_back(support::ulittle32_t(RVA)); }

private:
  std::vector<support::ulittle32_t> RVAs;
};

} // end namespace codeview

} // end namespace llvm

#endif // LLVM_DEBUGINFO_CODEVIEW_DEBUGSYMBOLRVASUBSECTION_H
