//===- llvm/CodeGen/PseudoProbePrinter.cpp - Pseudo Probe Emission -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains support for writing pseudo probe info into asm files.
//
//===----------------------------------------------------------------------===//

#include "llvm_lib_CodeGen_AsmPrinter_PseudoProbePrinter.h"
#include "llvm_include_llvm_CodeGen_AsmPrinter.h"
#include "llvm_include_llvm_IR_DebugInfoMetadata.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_PseudoProbe.h"
#include "llvm_include_llvm_MC_MCPseudoProbe.h"
#include "llvm_include_llvm_MC_MCStreamer.h"

using namespace llvm;

PseudoProbeHandler::~PseudoProbeHandler() = default;

void PseudoProbeHandler::emitPseudoProbe(uint64_t Guid, uint64_t Index,
                                         uint64_t Type, uint64_t Attr,
                                         const DILocation *DebugLoc) {
  // Gather all the inlined-at nodes.
  // When it's done ReversedInlineStack looks like ([66, B], [88, A])
  // which means, Function A inlines function B at calliste with a probe id 88,
  // and B inlines C at probe 66 where C is represented by Guid.
  SmallVector<InlineSite, 8> ReversedInlineStack;
  auto *InlinedAt = DebugLoc ? DebugLoc->getInlinedAt() : nullptr;
  while (InlinedAt) {
    const DISubprogram *SP = InlinedAt->getScope()->getSubprogram();
    // Use linkage name for C++ if possible.
    auto Name = SP->getLinkageName();
    if (Name.empty())
      Name = SP->getName();
    // Use caching to avoid redundant md5 computation for build speed.
    uint64_t &CallerGuid = NameGuidMap[Name];
    if (!CallerGuid)
      CallerGuid = Function::getGUID(Name);
    uint64_t CallerProbeId = PseudoProbeDwarfDiscriminator::extractProbeIndex(
        InlinedAt->getDiscriminator());
    ReversedInlineStack.emplace_back(CallerGuid, CallerProbeId);
    InlinedAt = InlinedAt->getInlinedAt();
  }

  SmallVector<InlineSite, 8> InlineStack(llvm::reverse(ReversedInlineStack));
  Asm->OutStreamer->emitPseudoProbe(Guid, Index, Type, Attr, InlineStack,
                                    Asm->CurrentFnSym);
}
