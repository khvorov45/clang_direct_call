//===- DiagnosticOptions.cpp - C Language Family Diagnostic Handling ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements the DiagnosticOptions related interfaces.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_Basic_DiagnosticOptions.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <type_traits>

namespace clang {

raw_ostream &operator<<(raw_ostream &Out, DiagnosticLevelMask M) {
  using UT = std::underlying_type_t<DiagnosticLevelMask>;
  return Out << static_cast<UT>(M);
}

} // namespace clang
