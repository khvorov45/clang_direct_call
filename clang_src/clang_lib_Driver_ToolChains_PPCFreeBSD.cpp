//===-- PPCFreeBSD.cpp - PowerPC ToolChain Implementations ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Driver_ToolChains_PPCFreeBSD.h"
#include "clang_include_clang_Driver_Driver.h"
#include "clang_include_clang_Driver_Options.h"
#include "llvm_include_llvm_Support_Path.h"

using namespace clang::driver::toolchains;
using namespace llvm::opt;

void PPCFreeBSDToolChain::AddClangSystemIncludeArgs(
    const ArgList &DriverArgs, ArgStringList &CC1Args) const {
  if (!DriverArgs.hasArg(clang::driver::options::OPT_nostdinc) &&
      !DriverArgs.hasArg(options::OPT_nobuiltininc)) {
    const Driver &D = getDriver();
    SmallString<128> P(D.ResourceDir);
    llvm::sys::path::append(P, "include", "ppc_wrappers");
    addSystemInclude(DriverArgs, CC1Args, P);
  }

  FreeBSD::AddClangSystemIncludeArgs(DriverArgs, CC1Args);
}
