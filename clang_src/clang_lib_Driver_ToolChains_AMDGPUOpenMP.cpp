//===- AMDGPUOpenMP.cpp - AMDGPUOpenMP ToolChain Implementation -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Driver_ToolChains_AMDGPUOpenMP.h"
#include "clang_lib_Driver_ToolChains_AMDGPU.h"
#include "clang_lib_Driver_ToolChains_CommonArgs.h"
#include "clang_lib_Driver_ToolChains_ROCm.h"
#include "clang_include_clang_Basic_DiagnosticDriver.h"
#include "clang_include_clang_Driver_Compilation.h"
#include "clang_include_clang_Driver_Driver.h"
#include "clang_include_clang_Driver_DriverDiagnostic.h"
#include "clang_include_clang_Driver_InputInfo.h"
#include "clang_include_clang_Driver_Options.h"
#include "clang_include_clang_Driver_Tool.h"
#include "llvm_include_llvm_ADT_STLExtras.h"
#include "llvm_include_llvm_Support_FileSystem.h"
#include "llvm_include_llvm_Support_FormatAdapters.h"
#include "llvm_include_llvm_Support_FormatVariadic.h"
#include "llvm_include_llvm_Support_Path.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

namespace {

static bool checkSystemForAMDGPU(const ArgList &Args, const AMDGPUToolChain &TC,
                                 std::string &GPUArch) {
  if (auto Err = TC.getSystemGPUArch(Args, GPUArch)) {
    std::string ErrMsg =
        llvm::formatv("{0}", llvm::fmt_consume(std::move(Err)));
    TC.getDriver().Diag(diag::err_drv_undetermined_amdgpu_arch) << ErrMsg;
    return false;
  }

  return true;
}
} // namespace

AMDGPUOpenMPToolChain::AMDGPUOpenMPToolChain(const Driver &D,
                                             const llvm::Triple &Triple,
                                             const ToolChain &HostTC,
                                             const ArgList &Args)
    : ROCMToolChain(D, Triple, Args), HostTC(HostTC) {
  // Lookup binaries into the driver directory, this is used to
  // discover the clang-offload-bundler executable.
  getProgramPaths().push_back(getDriver().Dir);
}

void AMDGPUOpenMPToolChain::addClangTargetOptions(
    const llvm::opt::ArgList &DriverArgs, llvm::opt::ArgStringList &CC1Args,
    Action::OffloadKind DeviceOffloadingKind) const {
  HostTC.addClangTargetOptions(DriverArgs, CC1Args, DeviceOffloadingKind);

  std::string GPUArch = DriverArgs.getLastArgValue(options::OPT_march_EQ).str();
  if (GPUArch.empty()) {
    if (!checkSystemForAMDGPU(DriverArgs, *this, GPUArch))
      return;
  }

  assert(DeviceOffloadingKind == Action::OFK_OpenMP &&
         "Only OpenMP offloading kinds are supported.");

  CC1Args.push_back("-target-cpu");
  CC1Args.push_back(DriverArgs.MakeArgStringRef(GPUArch));
  CC1Args.push_back("-fcuda-is-device");

  if (DriverArgs.hasArg(options::OPT_nogpulib))
    return;

  for (auto BCFile : getDeviceLibs(DriverArgs)) {
    CC1Args.push_back(BCFile.ShouldInternalize ? "-mlink-builtin-bitcode"
                                               : "-mlink-bitcode-file");
    CC1Args.push_back(DriverArgs.MakeArgString(BCFile.Path));
  }

  // Link the bitcode library late if we're using device LTO.
  if (getDriver().isUsingLTO(/* IsOffload */ true))
    return;

  addOpenMPDeviceRTL(getDriver(), DriverArgs, CC1Args, GPUArch, getTriple());
}

llvm::opt::DerivedArgList *AMDGPUOpenMPToolChain::TranslateArgs(
    const llvm::opt::DerivedArgList &Args, StringRef BoundArch,
    Action::OffloadKind DeviceOffloadKind) const {
  DerivedArgList *DAL =
      HostTC.TranslateArgs(Args, BoundArch, DeviceOffloadKind);
  if (!DAL)
    DAL = new DerivedArgList(Args.getBaseArgs());

  const OptTable &Opts = getDriver().getOpts();

  if (DeviceOffloadKind == Action::OFK_OpenMP) {
    for (Arg *A : Args)
      if (!llvm::is_contained(*DAL, A))
        DAL->append(A);

    if (!DAL->hasArg(options::OPT_march_EQ)) {
      std::string Arch = BoundArch.str();
      if (BoundArch.empty())
        checkSystemForAMDGPU(Args, *this, Arch);
      DAL->AddJoinedArg(nullptr, Opts.getOption(options::OPT_march_EQ), Arch);
    }

    return DAL;
  }

  for (Arg *A : Args) {
    DAL->append(A);
  }

  if (!BoundArch.empty()) {
    DAL->eraseArg(options::OPT_march_EQ);
    DAL->AddJoinedArg(nullptr, Opts.getOption(options::OPT_march_EQ),
                      BoundArch);
  }

  return DAL;
}

void AMDGPUOpenMPToolChain::addClangWarningOptions(
    ArgStringList &CC1Args) const {
  HostTC.addClangWarningOptions(CC1Args);
}

ToolChain::CXXStdlibType
AMDGPUOpenMPToolChain::GetCXXStdlibType(const ArgList &Args) const {
  return HostTC.GetCXXStdlibType(Args);
}

void AMDGPUOpenMPToolChain::AddClangSystemIncludeArgs(
    const ArgList &DriverArgs, ArgStringList &CC1Args) const {
  HostTC.AddClangSystemIncludeArgs(DriverArgs, CC1Args);
}

void AMDGPUOpenMPToolChain::AddIAMCUIncludeArgs(const ArgList &Args,
                                                ArgStringList &CC1Args) const {
  HostTC.AddIAMCUIncludeArgs(Args, CC1Args);
}

SanitizerMask AMDGPUOpenMPToolChain::getSupportedSanitizers() const {
  // The AMDGPUOpenMPToolChain only supports sanitizers in the sense that it
  // allows sanitizer arguments on the command line if they are supported by the
  // host toolchain. The AMDGPUOpenMPToolChain will actually ignore any command
  // line arguments for any of these "supported" sanitizers. That means that no
  // sanitization of device code is actually supported at this time.
  //
  // This behavior is necessary because the host and device toolchains
  // invocations often share the command line, so the device toolchain must
  // tolerate flags meant only for the host toolchain.
  return HostTC.getSupportedSanitizers();
}

VersionTuple
AMDGPUOpenMPToolChain::computeMSVCVersion(const Driver *D,
                                          const ArgList &Args) const {
  return HostTC.computeMSVCVersion(D, Args);
}

llvm::SmallVector<ToolChain::BitCodeLibraryInfo, 12>
AMDGPUOpenMPToolChain::getDeviceLibs(const llvm::opt::ArgList &Args) const {
  if (Args.hasArg(options::OPT_nogpulib))
    return {};

  if (!RocmInstallation.hasDeviceLibrary()) {
    getDriver().Diag(diag::err_drv_no_rocm_device_lib) << 0;
    return {};
  }

  StringRef GpuArch = getProcessorFromTargetID(
      getTriple(), Args.getLastArgValue(options::OPT_march_EQ));

  SmallVector<BitCodeLibraryInfo, 12> BCLibs;
  for (auto BCLib : getCommonDeviceLibNames(Args, GpuArch.str(),
                                            /*IsOpenMP=*/true))
    BCLibs.emplace_back(BCLib);

  return BCLibs;
}
