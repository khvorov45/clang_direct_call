//===-- ModuleDebugInfoPrinter.cpp - Prints module debug info metadata ----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass decodes the debug info metadata in a module and prints in a
// (sufficiently-prepared-) human-readable form.
//
// For example, run this pass from opt along with the -analyze option, and
// it'll print to standard output.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Analysis_ModuleDebugInfoPrinter.h"
#include "llvm_include_llvm_Analysis_Passes.h"
#include "llvm_include_llvm_BinaryFormat_Dwarf.h"
#include "llvm_include_llvm_IR_DebugInfo.h"
#include "llvm_include_llvm_IR_PassManager.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
using namespace llvm;

namespace {
class ModuleDebugInfoLegacyPrinter : public ModulePass {
  DebugInfoFinder Finder;

public:
  static char ID; // Pass identification, replacement for typeid
  ModuleDebugInfoLegacyPrinter() : ModulePass(ID) {
    initializeModuleDebugInfoLegacyPrinterPass(
        *PassRegistry::getPassRegistry());
  }

  bool runOnModule(Module &M) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
  void print(raw_ostream &O, const Module *M) const override;
};
}

char ModuleDebugInfoLegacyPrinter::ID = 0;
INITIALIZE_PASS(ModuleDebugInfoLegacyPrinter, "module-debuginfo",
                "Decodes module-level debug info", false, true)

ModulePass *llvm::createModuleDebugInfoPrinterPass() {
  return new ModuleDebugInfoLegacyPrinter();
}

bool ModuleDebugInfoLegacyPrinter::runOnModule(Module &M) {
  Finder.processModule(M);
  return false;
}

static void printFile(raw_ostream &O, StringRef Filename, StringRef Directory,
                      unsigned Line = 0) {
  if (Filename.empty())
    return;

  O << " from ";
  if (!Directory.empty())
    O << Directory << "/";
  O << Filename;
  if (Line)
    O << ":" << Line;
}

static void printModuleDebugInfo(raw_ostream &O, const Module *M,
                                 const DebugInfoFinder &Finder) {
  // Printing the nodes directly isn't particularly helpful (since they
  // reference other nodes that won't be printed, particularly for the
  // filenames), so just print a few useful things.
  for (DICompileUnit *CU : Finder.compile_units()) {
    O << "Compile unit: ";
    auto Lang = dwarf::LanguageString(CU->getSourceLanguage());
    if (!Lang.empty())
      O << Lang;
    else
      O << "unknown-language(" << CU->getSourceLanguage() << ")";
    printFile(O, CU->getFilename(), CU->getDirectory());
    O << '\n';
  }

  for (DISubprogram *S : Finder.subprograms()) {
    O << "Subprogram: " << S->getName();
    printFile(O, S->getFilename(), S->getDirectory(), S->getLine());
    if (!S->getLinkageName().empty())
      O << " ('" << S->getLinkageName() << "')";
    O << '\n';
  }

  for (auto *GVU : Finder.global_variables()) {
    const auto *GV = GVU->getVariable();
    O << "Global variable: " << GV->getName();
    printFile(O, GV->getFilename(), GV->getDirectory(), GV->getLine());
    if (!GV->getLinkageName().empty())
      O << " ('" << GV->getLinkageName() << "')";
    O << '\n';
  }

  for (const DIType *T : Finder.types()) {
    O << "Type:";
    if (!T->getName().empty())
      O << ' ' << T->getName();
    printFile(O, T->getFilename(), T->getDirectory(), T->getLine());
    if (auto *BT = dyn_cast<DIBasicType>(T)) {
      O << " ";
      auto Encoding = dwarf::AttributeEncodingString(BT->getEncoding());
      if (!Encoding.empty())
        O << Encoding;
      else
        O << "unknown-encoding(" << BT->getEncoding() << ')';
    } else {
      O << ' ';
      auto Tag = dwarf::TagString(T->getTag());
      if (!Tag.empty())
        O << Tag;
      else
        O << "unknown-tag(" << T->getTag() << ")";
    }
    if (auto *CT = dyn_cast<DICompositeType>(T)) {
      if (auto *S = CT->getRawIdentifier())
        O << " (identifier: '" << S->getString() << "')";
    }
    O << '\n';
  }
}

void ModuleDebugInfoLegacyPrinter::print(raw_ostream &O,
                                         const Module *M) const {
  printModuleDebugInfo(O, M, Finder);
}

ModuleDebugInfoPrinterPass::ModuleDebugInfoPrinterPass(raw_ostream &OS)
    : OS(OS) {}

PreservedAnalyses ModuleDebugInfoPrinterPass::run(Module &M,
                                                  ModuleAnalysisManager &AM) {
  Finder.processModule(M);
  printModuleDebugInfo(OS, &M, Finder);
  return PreservedAnalyses::all();
}
