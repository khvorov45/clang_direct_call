//===- RecordName.h ------------------------------------------- *- C++ --*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_RECORDNAME_H
#define LLVM_DEBUGINFO_CODEVIEW_RECORDNAME_H

#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_DebugInfo_CodeView_CVRecord.h"
#include <string>

namespace llvm {
namespace codeview {
class TypeCollection;
class TypeIndex;
std::string computeTypeName(TypeCollection &Types, TypeIndex Index);
StringRef getSymbolName(CVSymbol Sym);
} // namespace codeview
} // namespace llvm

#endif
