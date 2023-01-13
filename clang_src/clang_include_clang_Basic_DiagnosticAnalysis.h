//===--- DiagnosticAnalysis.h - Diagnostics for libanalysis -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_DIAGNOSTICANALYSIS_H
#define LLVM_CLANG_BASIC_DIAGNOSTICANALYSIS_H

#include "clang_include_clang_Basic_Diagnostic.h"

namespace clang {
namespace diag {
enum {
#define DIAG(ENUM, FLAGS, DEFAULT_MAPPING, DESC, GROUP, SFINAE, NOWERROR,      \
             SHOWINSYSHEADER, SHOWINSYSMACRO, DEFERRABLE, CATEGORY)            \
  ENUM,
#define ANALYSISSTART
#include "clang_include_clang_Basic_DiagnosticAnalysisKinds.inc"
#undef DIAG
  NUM_BUILTIN_ANALYSIS_DIAGNOSTICS
};
} // end namespace diag
} // end namespace clang

#endif // LLVM_CLANG_BASIC_DIAGNOSTICANALYSIS_H