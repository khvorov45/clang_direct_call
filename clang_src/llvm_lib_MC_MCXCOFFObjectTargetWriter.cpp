//===- MCXCOFFObjectTargetWriter.cpp - XCOFF Target Writer Subclass -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCXCOFFObjectWriter.h"

using namespace llvm;

MCXCOFFObjectTargetWriter::MCXCOFFObjectTargetWriter(bool Is64Bit)
    : Is64Bit(Is64Bit) {}

MCXCOFFObjectTargetWriter::~MCXCOFFObjectTargetWriter() = default;
