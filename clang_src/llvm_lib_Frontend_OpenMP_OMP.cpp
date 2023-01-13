//===- OMP.cpp ------ Collection of helpers for OpenMP --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Frontend_OpenMP_OMP.h.inc"

#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_ADT_StringSwitch.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"

using namespace llvm;
using namespace omp;

#define GEN_DIRECTIVES_IMPL
#include "llvm_include_llvm_Frontend_OpenMP_OMP.inc"
