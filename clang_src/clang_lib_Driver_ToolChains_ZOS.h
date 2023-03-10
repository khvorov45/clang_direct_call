//===--- ZOS.h - z/OS ToolChain Implementations -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ZOS_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ZOS_H

#include "clang_include_clang_Driver_Tool.h"
#include "clang_include_clang_Driver_ToolChain.h"

namespace clang {
namespace driver {
namespace toolchains {

class LLVM_LIBRARY_VISIBILITY ZOS : public ToolChain {
public:
  ZOS(const Driver &D, const llvm::Triple &Triple,
      const llvm::opt::ArgList &Args);
  ~ZOS() override;

  bool isPICDefault() const override { return false; }
  bool isPIEDefault(const llvm::opt::ArgList &Args) const override {
    return false;
  }
  bool isPICDefaultForced() const override { return false; }

  bool IsIntegratedAssemblerDefault() const override { return true; }

  unsigned GetDefaultDwarfVersion() const override { return 4; }

  void addClangTargetOptions(
      const llvm::opt::ArgList &DriverArgs, llvm::opt::ArgStringList &CC1Args,
      Action::OffloadKind DeviceOffloadingKind) const override;
};

} // end namespace toolchains
} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_ZOS_H
