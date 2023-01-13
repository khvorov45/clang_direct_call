//===- Formatters.h ---------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_FORMATTERS_H
#define LLVM_DEBUGINFO_CODEVIEW_FORMATTERS_H

#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_DebugInfo_CodeView_GUID.h"
#include "llvm_include_llvm_DebugInfo_CodeView_TypeIndex.h"
#include "llvm_include_llvm_Support_FormatAdapters.h"
#include "llvm_include_llvm_Support_FormatVariadic.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <cstdint>

namespace llvm {

namespace codeview {

struct GUID;

namespace detail {

class GuidAdapter final : public FormatAdapter<ArrayRef<uint8_t>> {
  ArrayRef<uint8_t> Guid;

public:
  explicit GuidAdapter(ArrayRef<uint8_t> Guid);
  explicit GuidAdapter(StringRef Guid);

  void format(raw_ostream &Stream, StringRef Style) override;
};

} // end namespace detail

inline detail::GuidAdapter fmt_guid(StringRef Item) {
  return detail::GuidAdapter(Item);
}

inline detail::GuidAdapter fmt_guid(ArrayRef<uint8_t> Item) {
  return detail::GuidAdapter(Item);
}

} // end namespace codeview

template <> struct format_provider<codeview::TypeIndex> {
public:
  static void format(const codeview::TypeIndex &V, raw_ostream &Stream,
                     StringRef Style) {
    if (V.isNoneType())
      Stream << "<no type>";
    else {
      Stream << formatv("{0:X+4}", V.getIndex());
      if (V.isSimple())
        Stream << " (" << codeview::TypeIndex::simpleTypeName(V) << ")";
    }
  }
};

template <> struct format_provider<codeview::GUID> {
  static void format(const codeview::GUID &V, llvm::raw_ostream &Stream,
                     StringRef Style) {
    Stream << V;
  }
};

} // end namespace llvm

#endif // LLVM_DEBUGINFO_CODEVIEW_FORMATTERS_H
