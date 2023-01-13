//===--- Stack.h - Utilities for dealing with stack space -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Defines utilities for dealing with stack allocation and stack space.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_STACK_H
#define LLVM_CLANG_BASIC_STACK_H

#include <cstddef>

#include "llvm_include_llvm_ADT_STLExtras.h"
#include "llvm_include_llvm_Support_Compiler.h"

namespace clang {
  /// Run a given function on a stack with "sufficient" space. If stack space
  /// is insufficient, calls Diag to emit a diagnostic before calling Fn.
  inline void runWithSufficientStackSpace(llvm::function_ref<void()> Diag,
                                          llvm::function_ref<void()> Fn) { 
      Fn();
  }
} // end namespace clang

#endif // LLVM_CLANG_BASIC_STACK_H
