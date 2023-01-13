//===- ExtractAPI/Serialization/SerializerBase.cpp --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements the APISerializer interface.
///
//===----------------------------------------------------------------------===//

#include "clang_include_clang_ExtractAPI_Serialization_SerializerBase.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace clang::extractapi;

void APISerializer::serialize(llvm::raw_ostream &os) {}
