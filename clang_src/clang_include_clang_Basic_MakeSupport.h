//===- MakeSupport.h - Make Utilities ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_MAKESUPPORT_H
#define LLVM_CLANG_BASIC_MAKESUPPORT_H

#include "clang_include_clang_Basic_LLVM.h"
#include "llvm_include_llvm_ADT_StringRef.h"

namespace clang {

/// Quote target names for inclusion in GNU Make dependency files.
/// Only the characters '$', '#', ' ', '\t' are quoted.
void quoteMakeTarget(StringRef Target, SmallVectorImpl<char> &Res);

} // namespace clang

#endif // LLVM_CLANG_BASIC_MAKESUPPORT_H
