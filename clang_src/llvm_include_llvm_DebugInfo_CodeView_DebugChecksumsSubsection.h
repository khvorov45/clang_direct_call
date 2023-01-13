//===- DebugChecksumsSubsection.h -------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_CODEVIEW_DEBUGCHECKSUMSSUBSECTION_H
#define LLVM_DEBUGINFO_CODEVIEW_DEBUGCHECKSUMSSUBSECTION_H

#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_ADT_DenseMap.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_DebugInfo_CodeView_CodeView.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugSubsection.h"
#include "llvm_include_llvm_Support_Allocator.h"
#include "llvm_include_llvm_Support_BinaryStreamArray.h"
#include "llvm_include_llvm_Support_BinaryStreamRef.h"
#include "llvm_include_llvm_Support_Error.h"
#include <cstdint>
#include <vector>

namespace llvm {

class BinaryStreamReader;
class BinaryStreamWriter;

namespace codeview {

class DebugStringTableSubsection;

struct FileChecksumEntry {
  uint32_t FileNameOffset;    // Byte offset of filename in global stringtable.
  FileChecksumKind Kind;      // The type of checksum.
  ArrayRef<uint8_t> Checksum; // The bytes of the checksum.
};

} // end namespace codeview

template <> struct VarStreamArrayExtractor<codeview::FileChecksumEntry> {
public:
  using ContextType = void;

  Error operator()(BinaryStreamRef Stream, uint32_t &Len,
                   codeview::FileChecksumEntry &Item);
};

namespace codeview {

class DebugChecksumsSubsectionRef final : public DebugSubsectionRef {
  using FileChecksumArray = VarStreamArray<codeview::FileChecksumEntry>;
  using Iterator = FileChecksumArray::Iterator;

public:
  DebugChecksumsSubsectionRef()
      : DebugSubsectionRef(DebugSubsectionKind::FileChecksums) {}

  static bool classof(const DebugSubsectionRef *S) {
    return S->kind() == DebugSubsectionKind::FileChecksums;
  }

  bool valid() const { return Checksums.valid(); }

  Error initialize(BinaryStreamReader Reader);
  Error initialize(BinaryStreamRef Stream);

  Iterator begin() const { return Checksums.begin(); }
  Iterator end() const { return Checksums.end(); }

  const FileChecksumArray &getArray() const { return Checksums; }

private:
  FileChecksumArray Checksums;
};

class DebugChecksumsSubsection final : public DebugSubsection {
public:
  explicit DebugChecksumsSubsection(DebugStringTableSubsection &Strings);

  static bool classof(const DebugSubsection *S) {
    return S->kind() == DebugSubsectionKind::FileChecksums;
  }

  void addChecksum(StringRef FileName, FileChecksumKind Kind,
                   ArrayRef<uint8_t> Bytes);

  uint32_t calculateSerializedSize() const override;
  Error commit(BinaryStreamWriter &Writer) const override;
  uint32_t mapChecksumOffset(StringRef FileName) const;

private:
  DebugStringTableSubsection &Strings;

  DenseMap<uint32_t, uint32_t> OffsetMap;
  uint32_t SerializedSize = 0;
  BumpPtrAllocator Storage;
  std::vector<FileChecksumEntry> Checksums;
};

} // end namespace codeview

} // end namespace llvm

#endif // LLVM_DEBUGINFO_CODEVIEW_DEBUGCHECKSUMSSUBSECTION_H
