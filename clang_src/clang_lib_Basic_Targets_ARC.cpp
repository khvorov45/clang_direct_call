//===--- ARC.cpp - Implement ARC target feature support -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements ARC TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Basic_Targets_ARC.h"
#include "clang_include_clang_Basic_Builtins.h"
#include "clang_include_clang_Basic_MacroBuilder.h"
#include "clang_include_clang_Basic_TargetBuiltins.h"

using namespace clang;
using namespace clang::targets;

void ARCTargetInfo::getTargetDefines(const LangOptions &Opts,
                                     MacroBuilder &Builder) const {
  Builder.defineMacro("__arc__");
}
