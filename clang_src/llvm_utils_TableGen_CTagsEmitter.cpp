//===- CTagsEmitter.cpp - Generate ctags-compatible index ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This tablegen backend emits an index of definitions in ctags(1) format.
// A helper script, utils/TableGen/tdtags, provides an easier-to-use
// interface; run 'tdtags -H' for documentation.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Support_SourceMgr.h"
#include "llvm_include_llvm_Support_MemoryBuffer.h"
#include "llvm_include_llvm_TableGen_Error.h"
#include "llvm_include_llvm_TableGen_Record.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace llvm;

#define DEBUG_TYPE "ctags-emitter"

namespace {

class Tag {
private:
  StringRef Id;
  StringRef BufferIdentifier;
  unsigned Line;
public:
  Tag(StringRef Name, const SMLoc Location) : Id(Name) {
    const MemoryBuffer *CurMB =
        SrcMgr.getMemoryBuffer(SrcMgr.FindBufferContainingLoc(Location));
    BufferIdentifier = CurMB->getBufferIdentifier();
    auto LineAndColumn = SrcMgr.getLineAndColumn(Location);
    Line = LineAndColumn.first;
  }
  int operator<(const Tag &B) const {
    return std::make_tuple(Id, BufferIdentifier, Line) < std::make_tuple(B.Id, B.BufferIdentifier, B.Line);
  }
  void emit(raw_ostream &OS) const {
    OS << Id << "\t" << BufferIdentifier << "\t" << Line << "\n";
  }
};

class CTagsEmitter {
private:
  RecordKeeper &Records;
public:
  CTagsEmitter(RecordKeeper &R) : Records(R) {}

  void run(raw_ostream &OS);

private:
  static SMLoc locate(const Record *R);
};

} // End anonymous namespace.

SMLoc CTagsEmitter::locate(const Record *R) {
  ArrayRef<SMLoc> Locs = R->getLoc();
  return !Locs.empty() ? Locs.front() : SMLoc();
}

void CTagsEmitter::run(raw_ostream &OS) {
  const auto &Classes = Records.getClasses();
  const auto &Defs = Records.getDefs();
  std::vector<Tag> Tags;
  // Collect tags.
  Tags.reserve(Classes.size() + Defs.size());
  for (const auto &C : Classes) {
    Tags.push_back(Tag(C.first, locate(C.second.get())));
    for (SMLoc FwdLoc : C.second->getForwardDeclarationLocs())
      Tags.push_back(Tag(C.first, FwdLoc));
  }
  for (const auto &D : Defs)
    Tags.push_back(Tag(D.first, locate(D.second.get())));
  // Emit tags.
  llvm::sort(Tags);
  OS << "!_TAG_FILE_FORMAT\t1\t/original ctags format/\n";
  OS << "!_TAG_FILE_SORTED\t1\t/0=unsorted, 1=sorted, 2=foldcase/\n";
  for (const Tag &T : Tags)
    T.emit(OS);
}

namespace llvm {

void EmitCTags(RecordKeeper &RK, raw_ostream &OS) { CTagsEmitter(RK).run(OS); }

} // End llvm namespace.
