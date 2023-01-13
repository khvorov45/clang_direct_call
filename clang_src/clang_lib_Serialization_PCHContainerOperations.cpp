//=== Serialization/PCHContainerOperations.cpp - PCH Containers -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file defines PCHContainerOperations and RawPCHContainerOperation.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_Serialization_PCHContainerOperations.h"
#include "clang_include_clang_AST_ASTConsumer.h"
#include "clang_include_clang_Lex_ModuleLoader.h"
#include "llvm_include_llvm_Bitstream_BitstreamReader.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <utility>

using namespace clang;

PCHContainerWriter::~PCHContainerWriter() {}
PCHContainerReader::~PCHContainerReader() {}

namespace {

/// A PCHContainerGenerator that writes out the PCH to a flat file.
class RawPCHContainerGenerator : public ASTConsumer {
  std::shared_ptr<PCHBuffer> Buffer;
  std::unique_ptr<raw_pwrite_stream> OS;

public:
  RawPCHContainerGenerator(std::unique_ptr<llvm::raw_pwrite_stream> OS,
                           std::shared_ptr<PCHBuffer> Buffer)
      : Buffer(std::move(Buffer)), OS(std::move(OS)) {}

  ~RawPCHContainerGenerator() override = default;

  void HandleTranslationUnit(ASTContext &Ctx) override {
    if (Buffer->IsComplete) {
      // Make sure it hits disk now.
      *OS << Buffer->Data;
      OS->flush();
    }
    // Free the space of the temporary buffer.
    llvm::SmallVector<char, 0> Empty;
    Buffer->Data = std::move(Empty);
  }
};

} // anonymous namespace

std::unique_ptr<ASTConsumer> RawPCHContainerWriter::CreatePCHContainerGenerator(
    CompilerInstance &CI, const std::string &MainFileName,
    const std::string &OutputFileName, std::unique_ptr<llvm::raw_pwrite_stream> OS,
    std::shared_ptr<PCHBuffer> Buffer) const {
  return std::make_unique<RawPCHContainerGenerator>(std::move(OS), Buffer);
}

StringRef
RawPCHContainerReader::ExtractPCH(llvm::MemoryBufferRef Buffer) const {
  return Buffer.getBuffer();
}

PCHContainerOperations::PCHContainerOperations() {
  registerWriter(std::make_unique<RawPCHContainerWriter>());
  registerReader(std::make_unique<RawPCHContainerReader>());
}
