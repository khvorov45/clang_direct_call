//===--- Frame.cpp - Call frame for the VM and AST Walker -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang_lib_AST_Interp_Frame.h"

using namespace clang;
using namespace clang::interp;

Frame::~Frame() {}
