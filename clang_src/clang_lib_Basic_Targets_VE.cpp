//===--- VE.cpp - Implement VE target feature support ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements VE TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Basic_Targets_VE.h"
#include "clang_include_clang_Basic_Builtins.h"
#include "clang_include_clang_Basic_MacroBuilder.h"
#include "clang_include_clang_Basic_TargetBuiltins.h"

using namespace clang;
using namespace clang::targets;

static constexpr Builtin::Info BuiltinInfo[] = {
#define BUILTIN(ID, TYPE, ATTRS)                                               \
  {#ID, TYPE, ATTRS, nullptr, ALL_LANGUAGES, nullptr},
#include "clang_include_clang_Basic_BuiltinsVE.def"
};

void VETargetInfo::getTargetDefines(const LangOptions &Opts,
                                    MacroBuilder &Builder) const {
  Builder.defineMacro("_LP64", "1");
  Builder.defineMacro("unix", "1");
  Builder.defineMacro("__unix__", "1");
  Builder.defineMacro("__linux__", "1");
  Builder.defineMacro("__ve", "1");
  Builder.defineMacro("__ve__", "1");
  Builder.defineMacro("__STDC_HOSTED__", "1");
  Builder.defineMacro("__STDC__", "1");
  Builder.defineMacro("__NEC__", "1");
  // FIXME: define __FAST_MATH__ 1 if -ffast-math is enabled
  // FIXME: define __OPTIMIZE__ n if -On is enabled
  // FIXME: define __VECTOR__ n 1 if automatic vectorization is enabled
}

ArrayRef<Builtin::Info> VETargetInfo::getTargetBuiltins() const {
  return llvm::makeArrayRef(BuiltinInfo,
                            clang::VE::LastTSBuiltin - Builtin::FirstTSBuiltin);
}
