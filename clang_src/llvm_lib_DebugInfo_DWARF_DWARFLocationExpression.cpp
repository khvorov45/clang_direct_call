//===- DWARFLocationExpression.cpp ----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_DebugInfo_DWARF_DWARFLocationExpression.h"
#include "llvm_include_llvm_ADT_iterator_range.h"
#include "llvm_include_llvm_Support_FormatVariadic.h"

using namespace llvm;

raw_ostream &llvm::operator<<(raw_ostream &OS,
                              const DWARFLocationExpression &Loc) {
  return OS << Loc.Range << ": "
            << formatv("{0}", make_range(Loc.Expr.begin(), Loc.Expr.end()));
}
