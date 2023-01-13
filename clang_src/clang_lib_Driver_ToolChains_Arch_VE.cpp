//===--- VE.cpp - Tools Implementations -------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Driver_ToolChains_Arch_VE.h"
#include "clang_include_clang_Driver_Driver.h"
#include "clang_include_clang_Driver_DriverDiagnostic.h"
#include "clang_include_clang_Driver_Options.h"
#include "llvm_include_llvm_Option_ArgList.h"

using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

void ve::getVETargetFeatures(const Driver &D, const ArgList &Args,
                             std::vector<StringRef> &Features) {}
