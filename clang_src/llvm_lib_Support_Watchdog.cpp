//===---- Watchdog.cpp - Implement Watchdog ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements the Watchdog class.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Support_Watchdog.h"
#include "llvm_include_llvm_Config_llvm-config.h"

// Include the platform-specific parts of this class.
#ifdef LLVM_ON_UNIX
#include "llvm_lib_Support_Unix_Watchdog.inc"
#endif
#ifdef _WIN32
#include "llvm_lib_Support_Windows_Watchdog.inc"
#endif
