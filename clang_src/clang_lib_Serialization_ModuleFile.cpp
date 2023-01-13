//===- ModuleFile.cpp - Module description --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements the ModuleFile class, which describes a module that
//  has been loaded from an AST file.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_Serialization_ModuleFile.h"
#include "clang_lib_Serialization_ASTReaderInternals.h"
#include "clang_include_clang_Serialization_ContinuousRangeMap.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_Support_Compiler.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace clang;
using namespace serialization;
using namespace reader;

ModuleFile::~ModuleFile() {
  delete static_cast<ASTIdentifierLookupTable *>(IdentifierLookupTable);
  delete static_cast<HeaderFileInfoLookupTable *>(HeaderFileInfoTable);
  delete static_cast<ASTSelectorLookupTable *>(SelectorLookupTable);
}

template<typename Key, typename Offset, unsigned InitialCapacity>
static void
dumpLocalRemap(StringRef Name,
               const ContinuousRangeMap<Key, Offset, InitialCapacity> &Map) {
  if (Map.begin() == Map.end())
    return;

  using MapType = ContinuousRangeMap<Key, Offset, InitialCapacity>;

  llvm::errs() << "  " << Name << ":\n";
  for (typename MapType::const_iterator I = Map.begin(), IEnd = Map.end();
       I != IEnd; ++I) {
    llvm::errs() << "    " << I->first << " -> " << I->second << "\n";
  }
}

LLVM_DUMP_METHOD void ModuleFile::dump() {
  llvm::errs() << "\nModule: " << FileName << "\n";
  if (!Imports.empty()) {
    llvm::errs() << "  Imports: ";
    for (unsigned I = 0, N = Imports.size(); I != N; ++I) {
      if (I)
        llvm::errs() << ", ";
      llvm::errs() << Imports[I]->FileName;
    }
    llvm::errs() << "\n";
  }

  // Remapping tables.
  llvm::errs() << "  Base source location offset: " << SLocEntryBaseOffset
               << '\n';
  dumpLocalRemap("Source location offset local -> global map", SLocRemap);

  llvm::errs() << "  Base identifier ID: " << BaseIdentifierID << '\n'
               << "  Number of identifiers: " << LocalNumIdentifiers << '\n';
  dumpLocalRemap("Identifier ID local -> global map", IdentifierRemap);

  llvm::errs() << "  Base macro ID: " << BaseMacroID << '\n'
               << "  Number of macros: " << LocalNumMacros << '\n';
  dumpLocalRemap("Macro ID local -> global map", MacroRemap);

  llvm::errs() << "  Base submodule ID: " << BaseSubmoduleID << '\n'
               << "  Number of submodules: " << LocalNumSubmodules << '\n';
  dumpLocalRemap("Submodule ID local -> global map", SubmoduleRemap);

  llvm::errs() << "  Base selector ID: " << BaseSelectorID << '\n'
               << "  Number of selectors: " << LocalNumSelectors << '\n';
  dumpLocalRemap("Selector ID local -> global map", SelectorRemap);

  llvm::errs() << "  Base preprocessed entity ID: " << BasePreprocessedEntityID
               << '\n'
               << "  Number of preprocessed entities: "
               << NumPreprocessedEntities << '\n';
  dumpLocalRemap("Preprocessed entity ID local -> global map",
                 PreprocessedEntityRemap);

  llvm::errs() << "  Base type index: " << BaseTypeIndex << '\n'
               << "  Number of types: " << LocalNumTypes << '\n';
  dumpLocalRemap("Type index local -> global map", TypeRemap);

  llvm::errs() << "  Base decl ID: " << BaseDeclID << '\n'
               << "  Number of decls: " << LocalNumDecls << '\n';
  dumpLocalRemap("Decl ID local -> global map", DeclRemap);
}
