//===- DebugSubsectionVisitor.cpp -------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_DebugInfo_CodeView_DebugSubsectionVisitor.h"

#include "llvm_include_llvm_DebugInfo_CodeView_CodeView.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugChecksumsSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugCrossExSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugCrossImpSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugFrameDataSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugInlineeLinesSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugLinesSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugStringTableSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugSubsectionRecord.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugSymbolRVASubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugSymbolsSubsection.h"
#include "llvm_include_llvm_DebugInfo_CodeView_DebugUnknownSubsection.h"
#include "llvm_include_llvm_Support_BinaryStreamReader.h"
#include "llvm_include_llvm_Support_SwapByteOrder.h"

using namespace llvm;
using namespace llvm::codeview;

Error llvm::codeview::visitDebugSubsection(
    const DebugSubsectionRecord &R, DebugSubsectionVisitor &V,
    const StringsAndChecksumsRef &State) {
  BinaryStreamReader Reader(R.getRecordData());
  switch (R.kind()) {
  case DebugSubsectionKind::Lines: {
    DebugLinesSubsectionRef Fragment;
    if (auto EC = Fragment.initialize(Reader))
      return EC;

    return V.visitLines(Fragment, State);
  }
  case DebugSubsectionKind::FileChecksums: {
    DebugChecksumsSubsectionRef Fragment;
    if (auto EC = Fragment.initialize(Reader))
      return EC;

    return V.visitFileChecksums(Fragment, State);
  }
  case DebugSubsectionKind::InlineeLines: {
    DebugInlineeLinesSubsectionRef Fragment;
    if (auto EC = Fragment.initialize(Reader))
      return EC;
    return V.visitInlineeLines(Fragment, State);
  }
  case DebugSubsectionKind::CrossScopeExports: {
    DebugCrossModuleExportsSubsectionRef Section;
    if (auto EC = Section.initialize(Reader))
      return EC;
    return V.visitCrossModuleExports(Section, State);
  }
  case DebugSubsectionKind::CrossScopeImports: {
    DebugCrossModuleImportsSubsectionRef Section;
    if (auto EC = Section.initialize(Reader))
      return EC;
    return V.visitCrossModuleImports(Section, State);
  }
  case DebugSubsectionKind::Symbols: {
    DebugSymbolsSubsectionRef Section;
    if (auto EC = Section.initialize(Reader))
      return EC;
    return V.visitSymbols(Section, State);
  }
  case DebugSubsectionKind::StringTable: {
    DebugStringTableSubsectionRef Section;
    if (auto EC = Section.initialize(Reader))
      return EC;
    return V.visitStringTable(Section, State);
  }
  case DebugSubsectionKind::FrameData: {
    DebugFrameDataSubsectionRef Section;
    if (auto EC = Section.initialize(Reader))
      return EC;
    return V.visitFrameData(Section, State);
  }
  case DebugSubsectionKind::CoffSymbolRVA: {
    DebugSymbolRVASubsectionRef Section;
    if (auto EC = Section.initialize(Reader))
      return EC;
    return V.visitCOFFSymbolRVAs(Section, State);
  }
  default: {
    DebugUnknownSubsectionRef Fragment(R.kind(), R.getRecordData());
    return V.visitUnknown(Fragment);
  }
  }
}
