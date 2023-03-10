//===- DebugChecksumsSubsection.cpp ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_DebugInfo_CodeView_DebugChecksumsSubsection.h"
#include "llvm_include_llvm_ADT_ArrayRef.h"
#include "llvm_include_llvm_DebugInfo_CodeView_CodeView.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugStringTableSubsection.h"
#include "llvm_include_llvm_Support_BinaryStreamReader.h"
#include "llvm_include_llvm_Support_BinaryStreamWriter.h"
#include "llvm_include_llvm_Support_Endian.h"
#include "llvm_include_llvm_Support_Error.h"
#include "llvm_include_llvm_Support_MathExtras.h"
#include <cassert>
#include <cstdint>
#include <cstring>

using namespace llvm;
using namespace llvm::codeview;

struct FileChecksumEntryHeader {
  using ulittle32_t = support::ulittle32_t;

  ulittle32_t FileNameOffset; // Byte offset of filename in global string table.
  uint8_t ChecksumSize;       // Number of bytes of checksum.
  uint8_t ChecksumKind;       // FileChecksumKind
                              // Checksum bytes follow.
};

Error VarStreamArrayExtractor<FileChecksumEntry>::
operator()(BinaryStreamRef Stream, uint32_t &Len, FileChecksumEntry &Item) {
  BinaryStreamReader Reader(Stream);

  const FileChecksumEntryHeader *Header;
  if (auto EC = Reader.readObject(Header))
    return EC;

  Item.FileNameOffset = Header->FileNameOffset;
  Item.Kind = static_cast<FileChecksumKind>(Header->ChecksumKind);
  if (auto EC = Reader.readBytes(Item.Checksum, Header->ChecksumSize))
    return EC;

  Len = alignTo(Header->ChecksumSize + sizeof(FileChecksumEntryHeader), 4);
  return Error::success();
}

Error DebugChecksumsSubsectionRef::initialize(BinaryStreamReader Reader) {
  if (auto EC = Reader.readArray(Checksums, Reader.bytesRemaining()))
    return EC;

  return Error::success();
}

Error DebugChecksumsSubsectionRef::initialize(BinaryStreamRef Section) {
  BinaryStreamReader Reader(Section);
  return initialize(Reader);
}

DebugChecksumsSubsection::DebugChecksumsSubsection(
    DebugStringTableSubsection &Strings)
    : DebugSubsection(DebugSubsectionKind::FileChecksums), Strings(Strings) {}

void DebugChecksumsSubsection::addChecksum(StringRef FileName,
                                           FileChecksumKind Kind,
                                           ArrayRef<uint8_t> Bytes) {
  FileChecksumEntry Entry;
  if (!Bytes.empty()) {
    uint8_t *Copy = Storage.Allocate<uint8_t>(Bytes.size());
    ::memcpy(Copy, Bytes.data(), Bytes.size());
    Entry.Checksum = makeArrayRef(Copy, Bytes.size());
  }

  Entry.FileNameOffset = Strings.insert(FileName);
  Entry.Kind = Kind;
  Checksums.push_back(Entry);

  // This maps the offset of this string in the string table to the offset
  // of this checksum entry in the checksum buffer.
  OffsetMap[Entry.FileNameOffset] = SerializedSize;
  assert(SerializedSize % 4 == 0);

  uint32_t Len = alignTo(sizeof(FileChecksumEntryHeader) + Bytes.size(), 4);
  SerializedSize += Len;
}

uint32_t DebugChecksumsSubsection::calculateSerializedSize() const {
  return SerializedSize;
}

Error DebugChecksumsSubsection::commit(BinaryStreamWriter &Writer) const {
  for (const auto &FC : Checksums) {
    FileChecksumEntryHeader Header;
    Header.ChecksumKind = uint8_t(FC.Kind);
    Header.ChecksumSize = FC.Checksum.size();
    Header.FileNameOffset = FC.FileNameOffset;
    if (auto EC = Writer.writeObject(Header))
      return EC;
    if (auto EC = Writer.writeArray(makeArrayRef(FC.Checksum)))
      return EC;
    if (auto EC = Writer.padToAlignment(4))
      return EC;
  }
  return Error::success();
}

uint32_t DebugChecksumsSubsection::mapChecksumOffset(StringRef FileName) const {
  uint32_t Offset = Strings.getIdForString(FileName);
  auto Iter = OffsetMap.find(Offset);
  assert(Iter != OffsetMap.end());
  return Iter->second;
}
