//===-- ModuleFileExtension.cpp - Module File Extensions ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "clang_include_clang_Serialization_ModuleFileExtension.h"
#include "llvm_include_llvm_ADT_Hashing.h"
using namespace clang;

char ModuleFileExtension::ID = 0;

ModuleFileExtension::~ModuleFileExtension() {}

void ModuleFileExtension::hashExtension(ExtensionHashBuilder &HBuilder) const {}

ModuleFileExtensionWriter::~ModuleFileExtensionWriter() {}

ModuleFileExtensionReader::~ModuleFileExtensionReader() {}
