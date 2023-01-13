//===--- Attributes.h - Attributes header -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_ATTRIBUTES_H
#define LLVM_CLANG_BASIC_ATTRIBUTES_H

#include "clang_include_clang_Basic_AttributeCommonInfo.h"
#include "clang_include_clang_Basic_LangOptions.h"
#include "clang_include_clang_Basic_TargetInfo.h"

namespace clang {

class IdentifierInfo;

/// Return the version number associated with the attribute if we
/// recognize and implement the attribute specified by the given information.
int hasAttribute(AttributeCommonInfo::Syntax Syntax,
                 const IdentifierInfo *Scope, const IdentifierInfo *Attr,
                 const TargetInfo &Target, const LangOptions &LangOpts);

} // end namespace clang

#endif // LLVM_CLANG_BASIC_ATTRIBUTES_H
