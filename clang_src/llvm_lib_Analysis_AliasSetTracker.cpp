//===- AliasSetTracker.cpp - Alias Sets Tracker implementation-------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the AliasSetTracker and AliasSet classes.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Analysis_AliasSetTracker.h"
#include "llvm_include_llvm_Analysis_AliasAnalysis.h"
#include "llvm_include_llvm_Analysis_GuardUtils.h"
#include "llvm_include_llvm_Analysis_MemoryLocation.h"
#include "llvm_include_llvm_Config_llvm-config.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_InstIterator.h"
#include "llvm_include_llvm_IR_Instructions.h"
#include "llvm_include_llvm_IR_IntrinsicInst.h"
#include "llvm_include_llvm_IR_PassManager.h"
#include "llvm_include_llvm_IR_PatternMatch.h"
#include "llvm_include_llvm_IR_Value.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Support_AtomicOrdering.h"
#include "llvm_include_llvm_Support_CommandLine.h"
#include "llvm_include_llvm_Support_Compiler.h"
#include "llvm_include_llvm_Support_Debug.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

using namespace llvm;

static cl::opt<unsigned>
    SaturationThreshold("alias-set-saturation-threshold", cl::Hidden,
                        cl::init(250),
                        cl::desc("The maximum number of pointers may-alias "
                                 "sets may contain before degradation"));

/// mergeSetIn - Merge the specified alias set into this alias set.
void AliasSet::mergeSetIn(AliasSet &AS, AliasSetTracker &AST,
                          BatchAAResults &BatchAA) {
  assert(!AS.Forward && "Alias set is already forwarding!");
  assert(!Forward && "This set is a forwarding set!!");

  bool WasMustAlias = (Alias == SetMustAlias);
  // Update the alias and access types of this set...
  Access |= AS.Access;
  Alias  |= AS.Alias;

  if (Alias == SetMustAlias) {
    // Check that these two merged sets really are must aliases.  Since both
    // used to be must-alias sets, we can just check any pointer from each set
    // for aliasing.
    PointerRec *L = getSomePointer();
    PointerRec *R = AS.getSomePointer();

    // If the pointers are not a must-alias pair, this set becomes a may alias.
    if (!BatchAA.isMustAlias(
            MemoryLocation(L->getValue(), L->getSize(), L->getAAInfo()),
            MemoryLocation(R->getValue(), R->getSize(), R->getAAInfo())))
      Alias = SetMayAlias;
  }

  if (Alias == SetMayAlias) {
    if (WasMustAlias)
      AST.TotalMayAliasSetSize += size();
    if (AS.Alias == SetMustAlias)
      AST.TotalMayAliasSetSize += AS.size();
  }

  bool ASHadUnknownInsts = !AS.UnknownInsts.empty();
  if (UnknownInsts.empty()) {            // Merge call sites...
    if (ASHadUnknownInsts) {
      std::swap(UnknownInsts, AS.UnknownInsts);
      addRef();
    }
  } else if (ASHadUnknownInsts) {
    llvm::append_range(UnknownInsts, AS.UnknownInsts);
    AS.UnknownInsts.clear();
  }

  AS.Forward = this; // Forward across AS now...
  addRef();          // AS is now pointing to us...

  // Merge the list of constituent pointers...
  if (AS.PtrList) {
    SetSize += AS.size();
    AS.SetSize = 0;
    *PtrListEnd = AS.PtrList;
    AS.PtrList->setPrevInList(PtrListEnd);
    PtrListEnd = AS.PtrListEnd;

    AS.PtrList = nullptr;
    AS.PtrListEnd = &AS.PtrList;
    assert(*AS.PtrListEnd == nullptr && "End of list is not null?");
  }
  if (ASHadUnknownInsts)
    AS.dropRef(AST);
}

void AliasSetTracker::removeAliasSet(AliasSet *AS) {
  if (AliasSet *Fwd = AS->Forward) {
    Fwd->dropRef(*this);
    AS->Forward = nullptr;
  } else // Update TotalMayAliasSetSize only if not forwarding.
      if (AS->Alias == AliasSet::SetMayAlias)
        TotalMayAliasSetSize -= AS->size();

  AliasSets.erase(AS);
  // If we've removed the saturated alias set, set saturated marker back to
  // nullptr and ensure this tracker is empty.
  if (AS == AliasAnyAS) {
    AliasAnyAS = nullptr;
    assert(AliasSets.empty() && "Tracker not empty");
  }
}

void AliasSet::removeFromTracker(AliasSetTracker &AST) {
  assert(RefCount == 0 && "Cannot remove non-dead alias set from tracker!");
  AST.removeAliasSet(this);
}

void AliasSet::addPointer(AliasSetTracker &AST, PointerRec &Entry,
                          LocationSize Size, const AAMDNodes &AAInfo,
                          bool KnownMustAlias, bool SkipSizeUpdate) {
  assert(!Entry.hasAliasSet() && "Entry already in set!");

  // Check to see if we have to downgrade to _may_ alias.
  if (isMustAlias())
    if (PointerRec *P = getSomePointer()) {
      if (!KnownMustAlias) {
        BatchAAResults &AA = AST.getAliasAnalysis();
        AliasResult Result = AA.alias(
            MemoryLocation(P->getValue(), P->getSize(), P->getAAInfo()),
            MemoryLocation(Entry.getValue(), Size, AAInfo));
        if (Result != AliasResult::MustAlias) {
          Alias = SetMayAlias;
          AST.TotalMayAliasSetSize += size();
        }
        assert(Result != AliasResult::NoAlias && "Cannot be part of must set!");
      } else if (!SkipSizeUpdate)
        P->updateSizeAndAAInfo(Size, AAInfo);
    }

  Entry.setAliasSet(this);
  Entry.updateSizeAndAAInfo(Size, AAInfo);

  // Add it to the end of the list...
  ++SetSize;
  assert(*PtrListEnd == nullptr && "End of list is not null?");
  *PtrListEnd = &Entry;
  PtrListEnd = Entry.setPrevInList(PtrListEnd);
  assert(*PtrListEnd == nullptr && "End of list is not null?");
  // Entry points to alias set.
  addRef();

  if (Alias == SetMayAlias)
    AST.TotalMayAliasSetSize++;
}

void AliasSet::addUnknownInst(Instruction *I, BatchAAResults &AA) {
  if (UnknownInsts.empty())
    addRef();
  UnknownInsts.emplace_back(I);

  // Guards are marked as modifying memory for control flow modelling purposes,
  // but don't actually modify any specific memory location.
  using namespace PatternMatch;
  bool MayWriteMemory = I->mayWriteToMemory() && !isGuard(I) &&
    !(I->use_empty() && match(I, m_Intrinsic<Intrinsic::invariant_start>()));
  if (!MayWriteMemory) {
    Alias = SetMayAlias;
    Access |= RefAccess;
    return;
  }

  // FIXME: This should use mod/ref information to make this not suck so bad
  Alias = SetMayAlias;
  Access = ModRefAccess;
}

/// aliasesPointer - If the specified pointer "may" (or must) alias one of the
/// members in the set return the appropriate AliasResult. Otherwise return
/// NoAlias.
///
AliasResult AliasSet::aliasesPointer(const Value *Ptr, LocationSize Size,
                                     const AAMDNodes &AAInfo,
                                     BatchAAResults &AA) const {
  if (AliasAny)
    return AliasResult::MayAlias;

  if (Alias == SetMustAlias) {
    assert(UnknownInsts.empty() && "Illegal must alias set!");

    // If this is a set of MustAliases, only check to see if the pointer aliases
    // SOME value in the set.
    PointerRec *SomePtr = getSomePointer();
    assert(SomePtr && "Empty must-alias set??");
    return AA.alias(MemoryLocation(SomePtr->getValue(), SomePtr->getSize(),
                                   SomePtr->getAAInfo()),
                    MemoryLocation(Ptr, Size, AAInfo));
  }

  // If this is a may-alias set, we have to check all of the pointers in the set
  // to be sure it doesn't alias the set...
  for (iterator I = begin(), E = end(); I != E; ++I) {
    AliasResult AR =
        AA.alias(MemoryLocation(Ptr, Size, AAInfo),
                 MemoryLocation(I.getPointer(), I.getSize(), I.getAAInfo()));
    if (AR != AliasResult::NoAlias)
      return AR;
  }

  // Check the unknown instructions...
  if (!UnknownInsts.empty()) {
    for (Instruction *Inst : UnknownInsts)
      if (isModOrRefSet(
              AA.getModRefInfo(Inst, MemoryLocation(Ptr, Size, AAInfo))))
        return AliasResult::MayAlias;
  }

  return AliasResult::NoAlias;
}

ModRefInfo AliasSet::aliasesUnknownInst(const Instruction *Inst,
                                        BatchAAResults &AA) const {

  if (AliasAny)
    return ModRefInfo::ModRef;

  if (!Inst->mayReadOrWriteMemory())
    return ModRefInfo::NoModRef;

  for (Instruction *UnknownInst : UnknownInsts) {
    const auto *C1 = dyn_cast<CallBase>(UnknownInst);
    const auto *C2 = dyn_cast<CallBase>(Inst);
    if (!C1 || !C2 || isModOrRefSet(AA.getModRefInfo(C1, C2)) ||
        isModOrRefSet(AA.getModRefInfo(C2, C1))) {
      // TODO: Could be more precise, but not really useful right now.
      return ModRefInfo::ModRef;
    }
  }

  ModRefInfo MR = ModRefInfo::NoModRef;
  for (iterator I = begin(), E = end(); I != E; ++I) {
    MR |= AA.getModRefInfo(
        Inst, MemoryLocation(I.getPointer(), I.getSize(), I.getAAInfo()));
    if (isModAndRefSet(MR))
      return MR;
  }

  return MR;
}

void AliasSetTracker::clear() {
  // Delete all the PointerRec entries.
  for (auto &I : PointerMap)
    I.second->eraseFromList();

  PointerMap.clear();

  // The alias sets should all be clear now.
  AliasSets.clear();
}

/// mergeAliasSetsForPointer - Given a pointer, merge all alias sets that may
/// alias the pointer. Return the unified set, or nullptr if no set that aliases
/// the pointer was found. MustAliasAll is updated to true/false if the pointer
/// is found to MustAlias all the sets it merged.
AliasSet *AliasSetTracker::mergeAliasSetsForPointer(const Value *Ptr,
                                                    LocationSize Size,
                                                    const AAMDNodes &AAInfo,
                                                    bool &MustAliasAll) {
  AliasSet *FoundSet = nullptr;
  MustAliasAll = true;
  for (AliasSet &AS : llvm::make_early_inc_range(*this)) {
    if (AS.Forward)
      continue;

    AliasResult AR = AS.aliasesPointer(Ptr, Size, AAInfo, AA);
    if (AR == AliasResult::NoAlias)
      continue;

    if (AR != AliasResult::MustAlias)
      MustAliasAll = false;

    if (!FoundSet) {
      // If this is the first alias set ptr can go into, remember it.
      FoundSet = &AS;
    } else {
      // Otherwise, we must merge the sets.
      FoundSet->mergeSetIn(AS, *this, AA);
    }
  }

  return FoundSet;
}

AliasSet *AliasSetTracker::findAliasSetForUnknownInst(Instruction *Inst) {
  AliasSet *FoundSet = nullptr;
  for (AliasSet &AS : llvm::make_early_inc_range(*this)) {
    if (AS.Forward || !isModOrRefSet(AS.aliasesUnknownInst(Inst, AA)))
      continue;
    if (!FoundSet) {
      // If this is the first alias set ptr can go into, remember it.
      FoundSet = &AS;
    } else {
      // Otherwise, we must merge the sets.
      FoundSet->mergeSetIn(AS, *this, AA);
    }
  }
  return FoundSet;
}

AliasSet &AliasSetTracker::getAliasSetFor(const MemoryLocation &MemLoc) {

  Value * const Pointer = const_cast<Value*>(MemLoc.Ptr);
  const LocationSize Size = MemLoc.Size;
  const AAMDNodes &AAInfo = MemLoc.AATags;

  AliasSet::PointerRec &Entry = getEntryFor(Pointer);

  if (AliasAnyAS) {
    // At this point, the AST is saturated, so we only have one active alias
    // set. That means we already know which alias set we want to return, and
    // just need to add the pointer to that set to keep the data structure
    // consistent.
    // This, of course, means that we will never need a merge here.
    if (Entry.hasAliasSet()) {
      Entry.updateSizeAndAAInfo(Size, AAInfo);
      assert(Entry.getAliasSet(*this) == AliasAnyAS &&
             "Entry in saturated AST must belong to only alias set");
    } else {
      AliasAnyAS->addPointer(*this, Entry, Size, AAInfo);
    }
    return *AliasAnyAS;
  }

  bool MustAliasAll = false;
  // Check to see if the pointer is already known.
  if (Entry.hasAliasSet()) {
    // If the size changed, we may need to merge several alias sets.
    // Note that we can *not* return the result of mergeAliasSetsForPointer
    // due to a quirk of alias analysis behavior. Since alias(undef, undef)
    // is NoAlias, mergeAliasSetsForPointer(undef, ...) will not find the
    // the right set for undef, even if it exists.
    if (Entry.updateSizeAndAAInfo(Size, AAInfo))
      mergeAliasSetsForPointer(Pointer, Size, AAInfo, MustAliasAll);
    // Return the set!
    return *Entry.getAliasSet(*this)->getForwardedTarget(*this);
  }

  if (AliasSet *AS =
          mergeAliasSetsForPointer(Pointer, Size, AAInfo, MustAliasAll)) {
    // Add it to the alias set it aliases.
    AS->addPointer(*this, Entry, Size, AAInfo, MustAliasAll);
    return *AS;
  }

  // Otherwise create a new alias set to hold the loaded pointer.
  AliasSets.push_back(new AliasSet());
  AliasSets.back().addPointer(*this, Entry, Size, AAInfo, true);
  return AliasSets.back();
}

void AliasSetTracker::add(Value *Ptr, LocationSize Size,
                          const AAMDNodes &AAInfo) {
  addPointer(MemoryLocation(Ptr, Size, AAInfo), AliasSet::NoAccess);
}

void AliasSetTracker::add(LoadInst *LI) {
  if (isStrongerThanMonotonic(LI->getOrdering()))
    return addUnknown(LI);
  addPointer(MemoryLocation::get(LI), AliasSet::RefAccess);
}

void AliasSetTracker::add(StoreInst *SI) {
  if (isStrongerThanMonotonic(SI->getOrdering()))
    return addUnknown(SI);
  addPointer(MemoryLocation::get(SI), AliasSet::ModAccess);
}

void AliasSetTracker::add(VAArgInst *VAAI) {
  addPointer(MemoryLocation::get(VAAI), AliasSet::ModRefAccess);
}

void AliasSetTracker::add(AnyMemSetInst *MSI) {
  addPointer(MemoryLocation::getForDest(MSI), AliasSet::ModAccess);
}

void AliasSetTracker::add(AnyMemTransferInst *MTI) {
  addPointer(MemoryLocation::getForDest(MTI), AliasSet::ModAccess);
  addPointer(MemoryLocation::getForSource(MTI), AliasSet::RefAccess);
}

void AliasSetTracker::addUnknown(Instruction *Inst) {
  if (isa<DbgInfoIntrinsic>(Inst))
    return; // Ignore DbgInfo Intrinsics.

  if (auto *II = dyn_cast<IntrinsicInst>(Inst)) {
    // These intrinsics will show up as affecting memory, but they are just
    // markers.
    switch (II->getIntrinsicID()) {
    default:
      break;
      // FIXME: Add lifetime/invariant intrinsics (See: PR30807).
    case Intrinsic::assume:
    case Intrinsic::experimental_noalias_scope_decl:
    case Intrinsic::sideeffect:
    case Intrinsic::pseudoprobe:
      return;
    }
  }
  if (!Inst->mayReadOrWriteMemory())
    return; // doesn't alias anything

  if (AliasSet *AS = findAliasSetForUnknownInst(Inst)) {
    AS->addUnknownInst(Inst, AA);
    return;
  }
  AliasSets.push_back(new AliasSet());
  AliasSets.back().addUnknownInst(Inst, AA);
}

void AliasSetTracker::add(Instruction *I) {
  // Dispatch to one of the other add methods.
  if (LoadInst *LI = dyn_cast<LoadInst>(I))
    return add(LI);
  if (StoreInst *SI = dyn_cast<StoreInst>(I))
    return add(SI);
  if (VAArgInst *VAAI = dyn_cast<VAArgInst>(I))
    return add(VAAI);
  if (AnyMemSetInst *MSI = dyn_cast<AnyMemSetInst>(I))
    return add(MSI);
  if (AnyMemTransferInst *MTI = dyn_cast<AnyMemTransferInst>(I))
    return add(MTI);

  // Handle all calls with known mod/ref sets genericall
  if (auto *Call = dyn_cast<CallBase>(I))
    if (Call->onlyAccessesArgMemory()) {
      auto getAccessFromModRef = [](ModRefInfo MRI) {
        if (isRefSet(MRI) && isModSet(MRI))
          return AliasSet::ModRefAccess;
        else if (isModSet(MRI))
          return AliasSet::ModAccess;
        else if (isRefSet(MRI))
          return AliasSet::RefAccess;
        else
          return AliasSet::NoAccess;
      };

      ModRefInfo CallMask = AA.getMemoryEffects(Call).getModRef();

      // Some intrinsics are marked as modifying memory for control flow
      // modelling purposes, but don't actually modify any specific memory
      // location.
      using namespace PatternMatch;
      if (Call->use_empty() &&
          match(Call, m_Intrinsic<Intrinsic::invariant_start>()))
        CallMask &= ModRefInfo::Ref;

      for (auto IdxArgPair : enumerate(Call->args())) {
        int ArgIdx = IdxArgPair.index();
        const Value *Arg = IdxArgPair.value();
        if (!Arg->getType()->isPointerTy())
          continue;
        MemoryLocation ArgLoc =
            MemoryLocation::getForArgument(Call, ArgIdx, nullptr);
        ModRefInfo ArgMask = AA.getArgModRefInfo(Call, ArgIdx);
        ArgMask &= CallMask;
        if (!isNoModRef(ArgMask))
          addPointer(ArgLoc, getAccessFromModRef(ArgMask));
      }
      return;
    }

  return addUnknown(I);
}

void AliasSetTracker::add(BasicBlock &BB) {
  for (auto &I : BB)
    add(&I);
}

void AliasSetTracker::add(const AliasSetTracker &AST) {
  assert(&AA == &AST.AA &&
         "Merging AliasSetTracker objects with different Alias Analyses!");

  // Loop over all of the alias sets in AST, adding the pointers contained
  // therein into the current alias sets.  This can cause alias sets to be
  // merged together in the current AST.
  for (const AliasSet &AS : AST) {
    if (AS.Forward)
      continue; // Ignore forwarding alias sets

    // If there are any call sites in the alias set, add them to this AST.
    for (Instruction *Inst : AS.UnknownInsts)
      add(Inst);

    // Loop over all of the pointers in this alias set.
    for (AliasSet::iterator ASI = AS.begin(), E = AS.end(); ASI != E; ++ASI)
      addPointer(
          MemoryLocation(ASI.getPointer(), ASI.getSize(), ASI.getAAInfo()),
          (AliasSet::AccessLattice)AS.Access);
  }
}

AliasSet &AliasSetTracker::mergeAllAliasSets() {
  assert(!AliasAnyAS && (TotalMayAliasSetSize > SaturationThreshold) &&
         "Full merge should happen once, when the saturation threshold is "
         "reached");

  // Collect all alias sets, so that we can drop references with impunity
  // without worrying about iterator invalidation.
  std::vector<AliasSet *> ASVector;
  ASVector.reserve(SaturationThreshold);
  for (AliasSet &AS : *this)
    ASVector.push_back(&AS);

  // Copy all instructions and pointers into a new set, and forward all other
  // sets to it.
  AliasSets.push_back(new AliasSet());
  AliasAnyAS = &AliasSets.back();
  AliasAnyAS->Alias = AliasSet::SetMayAlias;
  AliasAnyAS->Access = AliasSet::ModRefAccess;
  AliasAnyAS->AliasAny = true;

  for (auto *Cur : ASVector) {
    // If Cur was already forwarding, just forward to the new AS instead.
    AliasSet *FwdTo = Cur->Forward;
    if (FwdTo) {
      Cur->Forward = AliasAnyAS;
      AliasAnyAS->addRef();
      FwdTo->dropRef(*this);
      continue;
    }

    // Otherwise, perform the actual merge.
    AliasAnyAS->mergeSetIn(*Cur, *this, AA);
  }

  return *AliasAnyAS;
}

AliasSet &AliasSetTracker::addPointer(MemoryLocation Loc,
                                      AliasSet::AccessLattice E) {
  AliasSet &AS = getAliasSetFor(Loc);
  AS.Access |= E;

  if (!AliasAnyAS && (TotalMayAliasSetSize > SaturationThreshold)) {
    // The AST is now saturated. From here on, we conservatively consider all
    // pointers to alias each-other.
    return mergeAllAliasSets();
  }

  return AS;
}

//===----------------------------------------------------------------------===//
//               AliasSet/AliasSetTracker Printing Support
//===----------------------------------------------------------------------===//

void AliasSet::print(raw_ostream &OS) const {
  OS << "  AliasSet[" << (const void*)this << ", " << RefCount << "] ";
  OS << (Alias == SetMustAlias ? "must" : "may") << " alias, ";
  switch (Access) {
  case NoAccess:     OS << "No access "; break;
  case RefAccess:    OS << "Ref       "; break;
  case ModAccess:    OS << "Mod       "; break;
  case ModRefAccess: OS << "Mod/Ref   "; break;
  default: llvm_unreachable("Bad value for Access!");
  }
  if (Forward)
    OS << " forwarding to " << (void*)Forward;

  if (!empty()) {
    OS << "Pointers: ";
    for (iterator I = begin(), E = end(); I != E; ++I) {
      if (I != begin()) OS << ", ";
      I.getPointer()->printAsOperand(OS << "(");
      if (I.getSize() == LocationSize::afterPointer())
        OS << ", unknown after)";
      else if (I.getSize() == LocationSize::beforeOrAfterPointer())
        OS << ", unknown before-or-after)";
      else
        OS << ", " << I.getSize() << ")";
    }
  }
  if (!UnknownInsts.empty()) {
    ListSeparator LS;
    OS << "\n    " << UnknownInsts.size() << " Unknown instructions: ";
    for (Instruction *I : UnknownInsts) {
      OS << LS;
      if (I->hasName())
        I->printAsOperand(OS);
      else
        I->print(OS);
    }
  }
  OS << "\n";
}

void AliasSetTracker::print(raw_ostream &OS) const {
  OS << "Alias Set Tracker: " << AliasSets.size();
  if (AliasAnyAS)
    OS << " (Saturated)";
  OS << " alias sets for " << PointerMap.size() << " pointer values.\n";
  for (const AliasSet &AS : *this)
    AS.print(OS);
  OS << "\n";
}

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
LLVM_DUMP_METHOD void AliasSet::dump() const { print(dbgs()); }
LLVM_DUMP_METHOD void AliasSetTracker::dump() const { print(dbgs()); }
#endif

//===----------------------------------------------------------------------===//
//                            AliasSetPrinter Pass
//===----------------------------------------------------------------------===//

AliasSetsPrinterPass::AliasSetsPrinterPass(raw_ostream &OS) : OS(OS) {}

PreservedAnalyses AliasSetsPrinterPass::run(Function &F,
                                            FunctionAnalysisManager &AM) {
  auto &AA = AM.getResult<AAManager>(F);
  BatchAAResults BatchAA(AA);
  AliasSetTracker Tracker(BatchAA);
  OS << "Alias sets for function '" << F.getName() << "':\n";
  for (Instruction &I : instructions(F))
    Tracker.add(&I);
  Tracker.print(OS);
  return PreservedAnalyses::all();
}