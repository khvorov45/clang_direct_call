//===-- Decompressor.cpp --------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Object_Decompressor.h"
#include "llvm_include_llvm_BinaryFormat_ELF.h"
#include "llvm_include_llvm_Object_ObjectFile.h"
#include "llvm_include_llvm_Support_Compression.h"
#include "llvm_include_llvm_Support_DataExtractor.h"
#include "llvm_include_llvm_Support_Endian.h"

using namespace llvm;
using namespace llvm::support::endian;
using namespace object;

Expected<Decompressor> Decompressor::create(StringRef Name, StringRef Data,
                                            bool IsLE, bool Is64Bit) {
  Decompressor D(Data);
  if (Error Err = D.consumeCompressedHeader(Is64Bit, IsLE))
    return std::move(Err);
  return D;
}

Decompressor::Decompressor(StringRef Data)
    : SectionData(Data), DecompressedSize(0) {}

Error Decompressor::consumeCompressedHeader(bool Is64Bit, bool IsLittleEndian) {
  using namespace ELF;
  uint64_t HdrSize = Is64Bit ? sizeof(Elf64_Chdr) : sizeof(Elf32_Chdr);
  if (SectionData.size() < HdrSize)
    return createError("corrupted compressed section header");

  DataExtractor Extractor(SectionData, IsLittleEndian, 0);
  uint64_t Offset = 0;
  auto ChType = Extractor.getUnsigned(&Offset, Is64Bit ? sizeof(Elf64_Word)
                                                       : sizeof(Elf32_Word));
  switch (ChType) {
  case ELFCOMPRESS_ZLIB:
    CompressionType = DebugCompressionType::Zlib;
    break;
  case ELFCOMPRESS_ZSTD:
    CompressionType = DebugCompressionType::Zstd;
    break;
  default:
    return createError("unsupported compression type (" + Twine(ChType) + ")");
  }
  if (const char *Reason = llvm::compression::getReasonIfUnsupported(
          compression::formatFor(CompressionType)))
    return createError(Reason);

  // Skip Elf64_Chdr::ch_reserved field.
  if (Is64Bit)
    Offset += sizeof(Elf64_Word);

  DecompressedSize = Extractor.getUnsigned(
      &Offset, Is64Bit ? sizeof(Elf64_Xword) : sizeof(Elf32_Word));
  SectionData = SectionData.substr(HdrSize);
  return Error::success();
}

Error Decompressor::decompress(MutableArrayRef<uint8_t> Output) {
  return compression::decompress(CompressionType,
                                 arrayRefFromStringRef(SectionData),
                                 Output.data(), Output.size());
}
