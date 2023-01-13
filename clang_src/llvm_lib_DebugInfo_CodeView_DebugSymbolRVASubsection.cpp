//===- DebugSymbolRVASubsection.cpp ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_DebugInfo_CodeView_DebugSymbolRVASubsection.h"
#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_DebugInfo_CodeView_CodeView.h"
#include "llvm_include_llvm_Support_BinaryStreamReader.h"
#include "llvm_include_llvm_Support_BinaryStreamWriter.h"
#include <cstdint>

using namespace llvm;
using namespace llvm::codeview;

DebugSymbolRVASubsectionRef::DebugSymbolRVASubsectionRef()
    : DebugSubsectionRef(DebugSubsectionKind::CoffSymbolRVA) {}

Error DebugSymbolRVASubsectionRef::initialize(BinaryStreamReader &Reader) {
  return Reader.readArray(RVAs, Reader.bytesRemaining() / sizeof(uint32_t));
}

DebugSymbolRVASubsection::DebugSymbolRVASubsection()
    : DebugSubsection(DebugSubsectionKind::CoffSymbolRVA) {}

Error DebugSymbolRVASubsection::commit(BinaryStreamWriter &Writer) const {
  return Writer.writeArray(makeArrayRef(RVAs));
}

uint32_t DebugSymbolRVASubsection::calculateSerializedSize() const {
  return RVAs.size() * sizeof(uint32_t);
}
