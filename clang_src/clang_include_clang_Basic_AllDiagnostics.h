//===--- AllDiagnostics.h - Aggregate Diagnostic headers --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Includes all the separate Diagnostic headers & some related helpers.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_ALLDIAGNOSTICS_H
#define LLVM_CLANG_BASIC_ALLDIAGNOSTICS_H

#include "clang_include_clang_Basic_DiagnosticAST.h"
#include "clang_include_clang_Basic_DiagnosticAnalysis.h"
#include "clang_include_clang_Basic_DiagnosticComment.h"
#include "clang_include_clang_Basic_DiagnosticCrossTU.h"
#include "clang_include_clang_Basic_DiagnosticDriver.h"
#include "clang_include_clang_Basic_DiagnosticFrontend.h"
#include "clang_include_clang_Basic_DiagnosticLex.h"
#include "clang_include_clang_Basic_DiagnosticParse.h"
#include "clang_include_clang_Basic_DiagnosticSema.h"
#include "clang_include_clang_Basic_DiagnosticSerialization.h"
#include "clang_include_clang_Basic_DiagnosticRefactoring.h"

namespace clang {
template <size_t SizeOfStr, typename FieldType>
class StringSizerHelper {
  static_assert(SizeOfStr <= FieldType(~0U), "Field too small!");
public:
  enum { Size = SizeOfStr };
};
} // end namespace clang

#define STR_SIZE(str, fieldTy) clang::StringSizerHelper<sizeof(str)-1, \
                                                        fieldTy>::Size

#endif
