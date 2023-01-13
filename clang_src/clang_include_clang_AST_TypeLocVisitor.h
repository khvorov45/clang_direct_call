//===--- TypeLocVisitor.h - Visitor for TypeLoc subclasses ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file defines the TypeLocVisitor interface.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_CLANG_AST_TYPELOCVISITOR_H
#define LLVM_CLANG_AST_TYPELOCVISITOR_H

#include "clang_include_clang_AST_TypeLoc.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"

namespace clang {

#define DISPATCH(CLASSNAME) \
  return static_cast<ImplClass*>(this)-> \
    Visit##CLASSNAME(TyLoc.castAs<CLASSNAME>())

template<typename ImplClass, typename RetTy=void>
class TypeLocVisitor {
public:
  RetTy Visit(TypeLoc TyLoc) {
    switch (TyLoc.getTypeLocClass()) {
#define ABSTRACT_TYPELOC(CLASS, PARENT)
#define TYPELOC(CLASS, PARENT) \
    case TypeLoc::CLASS: DISPATCH(CLASS##TypeLoc);
#include "clang_include_clang_AST_TypeLocNodes.def"
    }
    llvm_unreachable("unexpected type loc class!");
  }

  RetTy Visit(UnqualTypeLoc TyLoc) {
    switch (TyLoc.getTypeLocClass()) {
#define ABSTRACT_TYPELOC(CLASS, PARENT)
#define TYPELOC(CLASS, PARENT) \
    case TypeLoc::CLASS: DISPATCH(CLASS##TypeLoc);
#include "clang_include_clang_AST_TypeLocNodes.def"
    }
    llvm_unreachable("unexpected type loc class!");
  }

#define TYPELOC(CLASS, PARENT)      \
  RetTy Visit##CLASS##TypeLoc(CLASS##TypeLoc TyLoc) { \
    DISPATCH(PARENT);               \
  }
#include "clang_include_clang_AST_TypeLocNodes.def"

  RetTy VisitTypeLoc(TypeLoc TyLoc) { return RetTy(); }
};

#undef DISPATCH

}  // end namespace clang

#endif // LLVM_CLANG_AST_TYPELOCVISITOR_H
