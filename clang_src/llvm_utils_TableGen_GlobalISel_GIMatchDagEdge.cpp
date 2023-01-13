//===- GIMatchDagEdge.cpp - An edge describing a def/use lookup -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_utils_TableGen_GlobalISel_GIMatchDagEdge.h"
#include "llvm_utils_TableGen_GlobalISel_GIMatchDagInstr.h"
#include "llvm_utils_TableGen_GlobalISel_GIMatchDagOperands.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace llvm;

LLVM_DUMP_METHOD void GIMatchDagEdge::print(raw_ostream &OS) const {
  OS << getFromMI()->getName() << "[" << getFromMO()->getName() << "] --["
     << Name << "]--> " << getToMI()->getName() << "[" << getToMO()->getName()
     << "]";
}

bool GIMatchDagEdge::isDefToUse() const {
  // Def -> Def is invalid so we only need to check FromMO.
  return FromMO->isDef();
}

void GIMatchDagEdge::reverse() {
  std::swap(FromMI, ToMI);
  std::swap(FromMO, ToMO);
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void GIMatchDagEdge::dump() const { print(errs()); }
#endif // if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)

raw_ostream &llvm::operator<<(raw_ostream &OS, const GIMatchDagEdge &E) {
  E.print(OS);
  return OS;
}
