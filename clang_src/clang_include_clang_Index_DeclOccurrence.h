//===- DeclOccurrence.h - An occurrence of a decl within a file -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_INDEX_DECLOCCURRENCE_H
#define LLVM_CLANG_INDEX_DECLOCCURRENCE_H

#include "clang_include_clang_AST_DeclBase.h"
#include "clang_include_clang_Basic_LLVM.h"
#include "clang_include_clang_Index_IndexSymbol.h"
#include "clang_include_clang_Lex_MacroInfo.h"
#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_ADT_PointerUnion.h"
#include "llvm_include_llvm_ADT_SmallVector.h"

namespace clang {
namespace index {

struct DeclOccurrence {
  SymbolRoleSet Roles;
  unsigned Offset;
  llvm::PointerUnion<const Decl *, const MacroInfo *> DeclOrMacro;
  const IdentifierInfo *MacroName = nullptr;
  SmallVector<SymbolRelation, 3> Relations;

  DeclOccurrence(SymbolRoleSet R, unsigned Offset, const Decl *D,
                 ArrayRef<SymbolRelation> Relations)
      : Roles(R), Offset(Offset), DeclOrMacro(D),
        Relations(Relations.begin(), Relations.end()) {}
  DeclOccurrence(SymbolRoleSet R, unsigned Offset, const IdentifierInfo *Name,
                 const MacroInfo *MI)
      : Roles(R), Offset(Offset), DeclOrMacro(MI), MacroName(Name) {}

  friend bool operator<(const DeclOccurrence &LHS, const DeclOccurrence &RHS) {
    return LHS.Offset < RHS.Offset;
  }
};

} // namespace index
} // namespace clang

#endif // LLVM_CLANG_INDEX_DECLOCCURRENCE_H
