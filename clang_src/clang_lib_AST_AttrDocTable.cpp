//===--- AttrDocTable.cpp - implements Attr::getDocumentation() -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file contains out-of-line methods for Attr classes.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_AST_Attr.h"
#include "llvm_include_llvm_ADT_StringRef.h"

#include "clang_lib_AST_AttrDocTable.inc"

static const llvm::StringRef AttrDoc[] = {
#define ATTR(NAME) AttrDoc_##NAME,
#include "clang_include_clang_Basic_AttrList.inc"
};

llvm::StringRef clang::Attr::getDocumentation(clang::attr::Kind K) {
  if (K < std::size(AttrDoc))
    return AttrDoc[K];
  return "";
}
