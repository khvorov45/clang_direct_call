//===--- BPF.cpp - Implement BPF target feature support -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements BPF TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "clang_lib_Basic_Targets_BPF.h"
#include "clang_lib_Basic_Targets.h"
#include "clang_include_clang_Basic_MacroBuilder.h"
#include "clang_include_clang_Basic_TargetBuiltins.h"
#include "llvm_include_llvm_ADT_StringRef.h"

using namespace clang;
using namespace clang::targets;

static constexpr Builtin::Info BuiltinInfo[] = {
#define BUILTIN(ID, TYPE, ATTRS)                                               \
  {#ID, TYPE, ATTRS, nullptr, ALL_LANGUAGES, nullptr},
#include "clang_include_clang_Basic_BuiltinsBPF.def"
};

void BPFTargetInfo::getTargetDefines(const LangOptions &Opts,
                                     MacroBuilder &Builder) const {
  Builder.defineMacro("__bpf__");
  Builder.defineMacro("__BPF__");
}

static constexpr llvm::StringLiteral ValidCPUNames[] = {"generic", "v1", "v2",
                                                        "v3", "probe"};

bool BPFTargetInfo::isValidCPUName(StringRef Name) const {
  return llvm::is_contained(ValidCPUNames, Name);
}

void BPFTargetInfo::fillValidCPUList(SmallVectorImpl<StringRef> &Values) const {
  Values.append(std::begin(ValidCPUNames), std::end(ValidCPUNames));
}

ArrayRef<Builtin::Info> BPFTargetInfo::getTargetBuiltins() const {
  return llvm::makeArrayRef(BuiltinInfo, clang::BPF::LastTSBuiltin -
                                             Builtin::FirstTSBuiltin);
}

bool BPFTargetInfo::handleTargetFeatures(std::vector<std::string> &Features,
                                         DiagnosticsEngine &Diags) {
  for (const auto &Feature : Features) {
    if (Feature == "+alu32") {
      HasAlu32 = true;
    }
  }

  return true;
}
