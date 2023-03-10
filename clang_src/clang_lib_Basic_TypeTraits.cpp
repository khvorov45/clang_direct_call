//===--- TypeTraits.cpp - Type Traits Support -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements the type traits support functions.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_Basic_TypeTraits.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include <cassert>
using namespace clang;

static constexpr const char *TypeTraitNames[] = {
#define TYPE_TRAIT_1(Spelling, Name, Key) #Name,
#include "clang_include_clang_Basic_TokenKinds.def"
#define TYPE_TRAIT_2(Spelling, Name, Key) #Name,
#include "clang_include_clang_Basic_TokenKinds.def"
#define TYPE_TRAIT_N(Spelling, Name, Key) #Name,
#include "clang_include_clang_Basic_TokenKinds.def"
};

static constexpr const char *TypeTraitSpellings[] = {
#define TYPE_TRAIT_1(Spelling, Name, Key) #Spelling,
#include "clang_include_clang_Basic_TokenKinds.def"
#define TYPE_TRAIT_2(Spelling, Name, Key) #Spelling,
#include "clang_include_clang_Basic_TokenKinds.def"
#define TYPE_TRAIT_N(Spelling, Name, Key) #Spelling,
#include "clang_include_clang_Basic_TokenKinds.def"
};

static constexpr const char *ArrayTypeTraitNames[] = {
#define ARRAY_TYPE_TRAIT(Spelling, Name, Key) #Name,
#include "clang_include_clang_Basic_TokenKinds.def"
};

static constexpr const char *ArrayTypeTraitSpellings[] = {
#define ARRAY_TYPE_TRAIT(Spelling, Name, Key) #Spelling,
#include "clang_include_clang_Basic_TokenKinds.def"
};

static constexpr const char *UnaryExprOrTypeTraitNames[] = {
#define UNARY_EXPR_OR_TYPE_TRAIT(Spelling, Name, Key) #Name,
#define CXX11_UNARY_EXPR_OR_TYPE_TRAIT(Spelling, Name, Key) #Name,
#include "clang_include_clang_Basic_TokenKinds.def"
};

static constexpr const char *UnaryExprOrTypeTraitSpellings[] = {
#define UNARY_EXPR_OR_TYPE_TRAIT(Spelling, Name, Key) #Spelling,
#define CXX11_UNARY_EXPR_OR_TYPE_TRAIT(Spelling, Name, Key) #Spelling,
#include "clang_include_clang_Basic_TokenKinds.def"
};

static constexpr const unsigned TypeTraitArities[] = {
#define TYPE_TRAIT_1(Spelling, Name, Key) 1,
#include "clang_include_clang_Basic_TokenKinds.def"
#define TYPE_TRAIT_2(Spelling, Name, Key) 2,
#include "clang_include_clang_Basic_TokenKinds.def"
#define TYPE_TRAIT_N(Spelling, Name, Key) 0,
#include "clang_include_clang_Basic_TokenKinds.def"
};

const char *clang::getTraitName(TypeTrait T) {
  assert(T <= TT_Last && "invalid enum value!");
  return TypeTraitNames[T];
}

const char *clang::getTraitName(ArrayTypeTrait T) {
  assert(T <= ATT_Last && "invalid enum value!");
  return ArrayTypeTraitNames[T];
}

const char *clang::getTraitName(UnaryExprOrTypeTrait T) {
  assert(T <= UETT_Last && "invalid enum value!");
  return UnaryExprOrTypeTraitNames[T];
}

const char *clang::getTraitSpelling(TypeTrait T) {
  assert(T <= TT_Last && "invalid enum value!");
  return TypeTraitSpellings[T];
}

const char *clang::getTraitSpelling(ArrayTypeTrait T) {
  assert(T <= ATT_Last && "invalid enum value!");
  return ArrayTypeTraitSpellings[T];
}

const char *clang::getTraitSpelling(UnaryExprOrTypeTrait T) {
  assert(T <= UETT_Last && "invalid enum value!");
  return UnaryExprOrTypeTraitSpellings[T];
}

unsigned clang::getTypeTraitArity(TypeTrait T) {
  assert(T <= TT_Last && "invalid enum value!");
  return TypeTraitArities[T];
}
