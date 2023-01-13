//===- ValueSymbolTable.cpp - Implement the ValueSymbolTable class --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the ValueSymbolTable class for the IR library.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_IR_ValueSymbolTable.h"
#include "llvm_include_llvm_ADT_SmallString.h"
#include "llvm_include_llvm_ADT_Triple.h"
#include "llvm_include_llvm_Config_llvm-config.h"
#include "llvm_include_llvm_IR_GlobalValue.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_IR_Type.h"
#include "llvm_include_llvm_IR_Value.h"
#include "llvm_include_llvm_Support_Casting.h"
#include "llvm_include_llvm_Support_Compiler.h"
#include "llvm_include_llvm_Support_Debug.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <cassert>
#include <utility>

using namespace llvm;

#define DEBUG_TYPE "valuesymtab"

// Class destructor
ValueSymbolTable::~ValueSymbolTable() {
#ifndef NDEBUG // Only do this in -g mode...
  for (const auto &VI : vmap)
    dbgs() << "Value still in symbol table! Type = '"
           << *VI.getValue()->getType() << "' Name = '" << VI.getKeyData()
           << "'\n";
  assert(vmap.empty() && "Values remain in symbol table!");
#endif
}

ValueName *ValueSymbolTable::makeUniqueName(Value *V,
                                            SmallString<256> &UniqueName) {
  unsigned BaseSize = UniqueName.size();
  while (true) {
    // Trim any suffix off and append the next number.
    UniqueName.resize(BaseSize);
    raw_svector_ostream S(UniqueName);
    if (auto *GV = dyn_cast<GlobalValue>(V)) {
      // A dot is appended to mark it as clone during ABI demangling so that
      // for example "_Z1fv" and "_Z1fv.1" both demangle to "f()", the second
      // one being a clone.
      // On NVPTX we cannot use a dot because PTX only allows [A-Za-z0-9_$] for
      // identifiers. This breaks ABI demangling but at least ptxas accepts and
      // compiles the program.
      const Module *M = GV->getParent();
      if (!(M && Triple(M->getTargetTriple()).isNVPTX()))
        S << ".";
    }
    S << ++LastUnique;

    // Try insert the vmap entry with this suffix.
    auto IterBool = vmap.insert(std::make_pair(UniqueName.str(), V));
    if (IterBool.second)
      return &*IterBool.first;
  }
}

// Insert a value into the symbol table with the specified name...
//
void ValueSymbolTable::reinsertValue(Value *V) {
  assert(V->hasName() && "Can't insert nameless Value into symbol table");

  // Try inserting the name, assuming it won't conflict.
  if (vmap.insert(V->getValueName())) {
    // LLVM_DEBUG(dbgs() << " Inserted value: " << V->getValueName() << ": " <<
    // *V << "\n");
    return;
  }

  // Otherwise, there is a naming conflict.  Rename this value.
  SmallString<256> UniqueName(V->getName().begin(), V->getName().end());

  // The name is too already used, just free it so we can allocate a new name.
  MallocAllocator Allocator;
  V->getValueName()->Destroy(Allocator);

  ValueName *VN = makeUniqueName(V, UniqueName);
  V->setValueName(VN);
}

void ValueSymbolTable::removeValueName(ValueName *V) {
  // LLVM_DEBUG(dbgs() << " Removing Value: " << V->getKeyData() << "\n");
  // Remove the value from the symbol table.
  vmap.remove(V);
}

/// createValueName - This method attempts to create a value name and insert
/// it into the symbol table with the specified name.  If it conflicts, it
/// auto-renames the name and returns that instead.
ValueName *ValueSymbolTable::createValueName(StringRef Name, Value *V) {
  if (MaxNameSize > -1 && Name.size() > (unsigned)MaxNameSize)
    Name = Name.substr(0, std::max(1u, (unsigned)MaxNameSize));

  // In the common case, the name is not already in the symbol table.
  auto IterBool = vmap.insert(std::make_pair(Name, V));
  if (IterBool.second) {
    // LLVM_DEBUG(dbgs() << " Inserted value: " << Entry.getKeyData() << ": "
    //           << *V << "\n");
    return &*IterBool.first;
  }

  // Otherwise, there is a naming conflict.  Rename this value.
  SmallString<256> UniqueName(Name.begin(), Name.end());
  return makeUniqueName(V, UniqueName);
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
// dump - print out the symbol table
//
LLVM_DUMP_METHOD void ValueSymbolTable::dump() const {
  // dbgs() << "ValueSymbolTable:\n";
  for (const auto &I : *this) {
    // dbgs() << "  '" << I->getKeyData() << "' = ";
    I.getValue()->dump();
    // dbgs() << "\n";
  }
}
#endif
