//===--- ZOS.cpp - z/OS ToolChain Implementations ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Driver_ToolChains_ZOS.h"
#include "clang_lib_Driver_ToolChains_CommonArgs.h"
#include "clang_include_clang_Driver_Compilation.h"
#include "clang_include_clang_Driver_Options.h"
#include "llvm_include_llvm_Option_ArgList.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace llvm::opt;
using namespace clang;

ZOS::ZOS(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
    : ToolChain(D, Triple, Args) {}

ZOS::~ZOS() {}

void ZOS::addClangTargetOptions(const ArgList &DriverArgs,
                                ArgStringList &CC1Args,
                                Action::OffloadKind DeviceOffloadKind) const {
  // Pass "-faligned-alloc-unavailable" only when the user hasn't manually
  // enabled or disabled aligned allocations.
  if (!DriverArgs.hasArgNoClaim(options::OPT_faligned_allocation,
                                options::OPT_fno_aligned_allocation))
    CC1Args.push_back("-faligned-alloc-unavailable");
}
