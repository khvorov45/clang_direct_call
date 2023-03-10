//===--- ProfileList.h - ProfileList filter ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// User-provided filters include/exclude profile instrumentation in certain
// functions.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_CLANG_BASIC_PROFILELIST_H
#define LLVM_CLANG_BASIC_PROFILELIST_H

#include "clang_include_clang_Basic_CodeGenOptions.h"
#include "clang_include_clang_Basic_LLVM.h"
#include "clang_include_clang_Basic_SourceLocation.h"
#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_ADT_Optional.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include <memory>

namespace clang {

class ProfileSpecialCaseList;

class ProfileList {
public:
  /// Represents if an how something should be excluded from profiling.
  enum ExclusionType {
    /// Profiling is allowed.
    Allow,
    /// Profiling is skipped using the \p skipprofile attribute.
    Skip,
    /// Profiling is forbidden using the \p noprofile attribute.
    Forbid,
  };

private:
  std::unique_ptr<ProfileSpecialCaseList> SCL;
  const bool Empty;
  SourceManager &SM;
  llvm::Optional<ExclusionType> inSection(StringRef Section, StringRef Prefix,
                                          StringRef Query) const;

public:
  ProfileList(ArrayRef<std::string> Paths, SourceManager &SM);
  ~ProfileList();

  bool isEmpty() const { return Empty; }
  ExclusionType getDefault(CodeGenOptions::ProfileInstrKind Kind) const;

  llvm::Optional<ExclusionType>
  isFunctionExcluded(StringRef FunctionName,
                     CodeGenOptions::ProfileInstrKind Kind) const;
  llvm::Optional<ExclusionType>
  isLocationExcluded(SourceLocation Loc,
                     CodeGenOptions::ProfileInstrKind Kind) const;
  llvm::Optional<ExclusionType>
  isFileExcluded(StringRef FileName,
                 CodeGenOptions::ProfileInstrKind Kind) const;
};

} // namespace clang

#endif
