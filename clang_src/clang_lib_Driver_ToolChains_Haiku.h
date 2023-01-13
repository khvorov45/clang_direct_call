//===--- Haiku.h - Haiku ToolChain Implementations --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_HAIKU_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_HAIKU_H

#include "clang_lib_Driver_ToolChains_Gnu.h"
#include "clang_include_clang_Driver_Driver.h"
#include "clang_include_clang_Driver_ToolChain.h"

namespace clang {
namespace driver {
namespace toolchains {

class LLVM_LIBRARY_VISIBILITY Haiku : public Generic_ELF {
public:
  Haiku(const Driver &D, const llvm::Triple &Triple,
          const llvm::opt::ArgList &Args);

  bool isPIEDefault(const llvm::opt::ArgList &Args) const override {
    return getTriple().getArch() == llvm::Triple::x86_64;
  }

  void addLibCxxIncludePaths(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;
  void addLibStdCxxIncludePaths(
      const llvm::opt::ArgList &DriverArgs,
      llvm::opt::ArgStringList &CC1Args) const override;
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_HAIKU_H
