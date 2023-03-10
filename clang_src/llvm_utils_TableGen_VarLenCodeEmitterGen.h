//===- VarLenCodeEmitterGen.h - CEG for variable-length insts ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declare the CodeEmitterGen component for variable-length
// instructions. See the .cpp file for more details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_UTILS_TABLEGEN_VARLENCODEEMITTERGEN_H
#define LLVM_UTILS_TABLEGEN_VARLENCODEEMITTERGEN_H

#include "llvm_include_llvm_TableGen_Record.h"

namespace llvm {

struct EncodingSegment {
  unsigned BitWidth;
  const Init *Value;
  StringRef CustomEncoder = "";
};

class VarLenInst {
  const RecordVal *TheDef;
  size_t NumBits;

  // Set if any of the segment is not fixed value.
  bool HasDynamicSegment;

  SmallVector<EncodingSegment, 4> Segments;

  void buildRec(const DagInit *DI);

  StringRef getCustomEncoderName(const Init *EI) const {
    if (const auto *DI = dyn_cast<DagInit>(EI)) {
      if (DI->getNumArgs() && isa<StringInit>(DI->getArg(0)))
        return cast<StringInit>(DI->getArg(0))->getValue();
    }
    return "";
  }

public:
  VarLenInst() : TheDef(nullptr), NumBits(0U), HasDynamicSegment(false) {}

  explicit VarLenInst(const DagInit *DI, const RecordVal *TheDef);

  /// Number of bits
  size_t size() const { return NumBits; }

  using const_iterator = decltype(Segments)::const_iterator;

  const_iterator begin() const { return Segments.begin(); }
  const_iterator end() const { return Segments.end(); }
  size_t getNumSegments() const { return Segments.size(); }

  bool isFixedValueOnly() const { return !HasDynamicSegment; }
};

void emitVarLenCodeEmitter(RecordKeeper &R, raw_ostream &OS);

} // end namespace llvm
#endif
