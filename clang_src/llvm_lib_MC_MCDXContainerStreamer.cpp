//===- lib/MC/MCDXContainerStreamer.cpp - DXContainer Impl ----*- C++ -*---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the object streamer for DXContainer files.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCDXContainerStreamer.h"
#include "llvm_include_llvm_MC_MCAssembler.h"
#include "llvm_include_llvm_MC_TargetRegistry.h"

using namespace llvm;

void MCDXContainerStreamer::emitInstToData(const MCInst &,
                                           const MCSubtargetInfo &) {}

MCStreamer *llvm::createDXContainerStreamer(
    MCContext &Context, std::unique_ptr<MCAsmBackend> &&MAB,
    std::unique_ptr<MCObjectWriter> &&OW, std::unique_ptr<MCCodeEmitter> &&CE,
    bool RelaxAll) {
  auto *S = new MCDXContainerStreamer(Context, std::move(MAB), std::move(OW),
                                      std::move(CE));
  if (RelaxAll)
    S->getAssembler().setRelaxAll(true);
  return S;
}
