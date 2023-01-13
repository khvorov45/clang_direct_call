//===-- COM.cpp - Implement COM utility classes -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements utility classes related to COM.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Support_COM.h"

#include "llvm_include_llvm_Config_llvm-config.h"

// Include the platform-specific parts of this class.
#ifdef LLVM_ON_UNIX
#include "llvm_lib_Support_Unix_COM.inc"
#elif defined(_WIN32)
#include "llvm_lib_Support_Windows_COM.inc"
#endif
