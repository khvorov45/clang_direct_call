//===--- Context.cpp - Context for the constexpr VM -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang_lib_AST_Interp_Context.h"
#include "clang_lib_AST_Interp_ByteCodeEmitter.h"
#include "clang_lib_AST_Interp_ByteCodeExprGen.h"
#include "clang_lib_AST_Interp_ByteCodeStmtGen.h"
#include "clang_lib_AST_Interp_EvalEmitter.h"
#include "clang_lib_AST_Interp_Interp.h"
#include "clang_lib_AST_Interp_InterpFrame.h"
#include "clang_lib_AST_Interp_InterpStack.h"
#include "clang_lib_AST_Interp_PrimType.h"
#include "clang_lib_AST_Interp_Program.h"
#include "clang_include_clang_AST_Expr.h"
#include "clang_include_clang_Basic_TargetInfo.h"

using namespace clang;
using namespace clang::interp;

Context::Context(ASTContext &Ctx) : Ctx(Ctx), P(new Program(*this)) {}

Context::~Context() {}

bool Context::isPotentialConstantExpr(State &Parent, const FunctionDecl *FD) {
  assert(Stk.empty());
  Function *Func = P->getFunction(FD);
  if (!Func || !Func->hasBody()) {
    if (auto R = ByteCodeStmtGen<ByteCodeEmitter>(*this, *P).compileFunc(FD)) {
      Func = *R;
    } else {
      handleAllErrors(R.takeError(), [&Parent](ByteCodeGenError &Err) {
        Parent.FFDiag(Err.getRange().getBegin(),
                      diag::err_experimental_clang_interp_failed)
            << Err.getRange();
      });
      return false;
    }
  }

  return Func->isConstexpr();
}

bool Context::evaluateAsRValue(State &Parent, const Expr *E, APValue &Result) {
  assert(Stk.empty());
  ByteCodeExprGen<EvalEmitter> C(*this, *P, Parent, Stk, Result);
  if (Check(Parent, C.interpretExpr(E))) {
    assert(Stk.empty());
    return true;
  }

  Stk.clear();
  return false;
}

bool Context::evaluateAsInitializer(State &Parent, const VarDecl *VD,
                                    APValue &Result) {
  assert(Stk.empty());
  ByteCodeExprGen<EvalEmitter> C(*this, *P, Parent, Stk, Result);
  if (Check(Parent, C.interpretDecl(VD))) {
    assert(Stk.empty());
    return true;
  }

  Stk.clear();
  return false;
}

const LangOptions &Context::getLangOpts() const { return Ctx.getLangOpts(); }

std::optional<PrimType> Context::classify(QualType T) const {
  if (T->isReferenceType() || T->isPointerType()) {
    return PT_Ptr;
  }

  if (T->isBooleanType())
    return PT_Bool;

  if (T->isSignedIntegerOrEnumerationType()) {
    switch (Ctx.getIntWidth(T)) {
    case 64:
      return PT_Sint64;
    case 32:
      return PT_Sint32;
    case 16:
      return PT_Sint16;
    case 8:
      return PT_Sint8;
    default:
      return {};
    }
  }

  if (T->isUnsignedIntegerOrEnumerationType()) {
    switch (Ctx.getIntWidth(T)) {
    case 64:
      return PT_Uint64;
    case 32:
      return PT_Uint32;
    case 16:
      return PT_Uint16;
    case 8:
      return PT_Uint8;
    default:
      return {};
    }
  }

  if (T->isNullPtrType())
    return PT_Ptr;

  if (auto *AT = dyn_cast<AtomicType>(T))
    return classify(AT->getValueType());

  return {};
}

unsigned Context::getCharBit() const {
  return Ctx.getTargetInfo().getCharWidth();
}

bool Context::Run(State &Parent, Function *Func, APValue &Result) {
  InterpState State(Parent, *P, Stk, *this);
  State.Current = new InterpFrame(State, Func, nullptr, {}, {});
  if (Interpret(State, Result))
    return true;
  Stk.clear();
  return false;
}

bool Context::Check(State &Parent, llvm::Expected<bool> &&Flag) {
  if (Flag)
    return *Flag;
  handleAllErrors(Flag.takeError(), [&Parent](ByteCodeGenError &Err) {
    Parent.FFDiag(Err.getRange().getBegin(),
                  diag::err_experimental_clang_interp_failed)
        << Err.getRange();
  });
  return false;
}
