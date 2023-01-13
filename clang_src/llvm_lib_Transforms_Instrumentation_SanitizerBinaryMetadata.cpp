//===- SanitizerBinaryMetadata.cpp - binary analysis sanitizers metadata --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is a part of SanitizerBinaryMetadata.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Instrumentation_SanitizerBinaryMetadata.h"
#include "llvm_include_llvm_ADT_SetVector.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_ADT_Statistic.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_ADT_Triple.h"
#include "llvm_include_llvm_ADT_Twine.h"
#include "llvm_include_llvm_IR_Constant.h"
#include "llvm_include_llvm_IR_DerivedTypes.h"
#include "llvm_include_llvm_IR_Function.h"
#include "llvm_include_llvm_IR_GlobalValue.h"
#include "llvm_include_llvm_IR_GlobalVariable.h"
#include "llvm_include_llvm_IR_IRBuilder.h"
#include "llvm_include_llvm_IR_Instruction.h"
#include "llvm_include_llvm_IR_Instructions.h"
#include "llvm_include_llvm_IR_LLVMContext.h"
#include "llvm_include_llvm_IR_MDBuilder.h"
#include "llvm_include_llvm_IR_Metadata.h"
#include "llvm_include_llvm_IR_Module.h"
#include "llvm_include_llvm_IR_Type.h"
#include "llvm_include_llvm_IR_Value.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_Pass.h"
#include "llvm_include_llvm_Support_CommandLine.h"
#include "llvm_include_llvm_Support_Debug.h"
#include "llvm_include_llvm_Transforms_Instrumentation.h"
#include "llvm_include_llvm_Transforms_Utils_ModuleUtils.h"

#include <array>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "sanmd"

namespace {

//===--- Constants --------------------------------------------------------===//

constexpr uint32_t kVersionBase = 1;                // occupies lower 16 bits
constexpr uint32_t kVersionPtrSizeRel = (1u << 16); // offsets are pointer-sized
constexpr int kCtorDtorPriority = 2;

// Pairs of names of initialization callback functions and which section
// contains the relevant metadata.
class MetadataInfo {
public:
  const StringRef FunctionPrefix;
  const StringRef SectionSuffix;
  const uint32_t FeatureMask;

  static const MetadataInfo Covered;
  static const MetadataInfo Atomics;

private:
  // Forbid construction elsewhere.
  explicit constexpr MetadataInfo(StringRef FunctionPrefix,
                                  StringRef SectionSuffix, uint32_t Feature)
      : FunctionPrefix(FunctionPrefix), SectionSuffix(SectionSuffix),
        FeatureMask(Feature) {}
};
const MetadataInfo MetadataInfo::Covered{"__sanitizer_metadata_covered",
                                         kSanitizerBinaryMetadataCoveredSection,
                                         kSanitizerBinaryMetadataNone};
const MetadataInfo MetadataInfo::Atomics{"__sanitizer_metadata_atomics",
                                         kSanitizerBinaryMetadataAtomicsSection,
                                         kSanitizerBinaryMetadataAtomics};

// The only instances of MetadataInfo are the constants above, so a set of
// them may simply store pointers to them. To deterministically generate code,
// we need to use a set with stable iteration order, such as SetVector.
using MetadataInfoSet = SetVector<const MetadataInfo *>;

//===--- Command-line options ---------------------------------------------===//

cl::opt<bool> ClEmitCovered("sanitizer-metadata-covered",
                            cl::desc("Emit PCs for covered functions."),
                            cl::Hidden, cl::init(false));
cl::opt<bool> ClEmitAtomics("sanitizer-metadata-atomics",
                            cl::desc("Emit PCs for atomic operations."),
                            cl::Hidden, cl::init(false));
cl::opt<bool> ClEmitUAR("sanitizer-metadata-uar",
                        cl::desc("Emit PCs for start of functions that are "
                                 "subject for use-after-return checking"),
                        cl::Hidden, cl::init(false));

//===--- Statistics -------------------------------------------------------===//

STATISTIC(NumMetadataCovered, "Metadata attached to covered functions");
STATISTIC(NumMetadataAtomics, "Metadata attached to atomics");
STATISTIC(NumMetadataUAR, "Metadata attached to UAR functions");

//===----------------------------------------------------------------------===//

// Apply opt overrides.
SanitizerBinaryMetadataOptions &&
transformOptionsFromCl(SanitizerBinaryMetadataOptions &&Opts) {
  Opts.Covered |= ClEmitCovered;
  Opts.Atomics |= ClEmitAtomics;
  Opts.UAR |= ClEmitUAR;
  return std::move(Opts);
}

class SanitizerBinaryMetadata {
public:
  SanitizerBinaryMetadata(Module &M, SanitizerBinaryMetadataOptions Opts)
      : Mod(M), Options(transformOptionsFromCl(std::move(Opts))),
        TargetTriple(M.getTargetTriple()), IRB(M.getContext()) {
    // FIXME: Make it work with other formats.
    assert(TargetTriple.isOSBinFormatELF() && "ELF only");
  }

  bool run();

private:
  // Return enabled feature mask of per-instruction metadata.
  uint32_t getEnabledPerInstructionFeature() const {
    uint32_t FeatureMask = 0;
    if (Options.Atomics)
      FeatureMask |= MetadataInfo::Atomics.FeatureMask;
    return FeatureMask;
  }

  uint32_t getVersion() const {
    uint32_t Version = kVersionBase;
    const auto CM = Mod.getCodeModel();
    if (CM.has_value() && (*CM == CodeModel::Medium || *CM == CodeModel::Large))
      Version |= kVersionPtrSizeRel;
    return Version;
  }

  void runOn(Function &F, MetadataInfoSet &MIS);

  // Determines which set of metadata to collect for this instruction.
  //
  // Returns true if covered metadata is required to unambiguously interpret
  // other metadata. For example, if we are interested in atomics metadata, any
  // function with memory operations (atomic or not) requires covered metadata
  // to determine if a memory operation is atomic or not in modules compiled
  // with SanitizerBinaryMetadata.
  bool runOn(Instruction &I, MetadataInfoSet &MIS, MDBuilder &MDB,
             uint32_t &FeatureMask);

  // Get start/end section marker pointer.
  GlobalVariable *getSectionMarker(const Twine &MarkerName, Type *Ty);

  // Returns the target-dependent section name.
  StringRef getSectionName(StringRef SectionSuffix);

  // Returns the section start marker name.
  Twine getSectionStart(StringRef SectionSuffix);

  // Returns the section end marker name.
  Twine getSectionEnd(StringRef SectionSuffix);

  Module &Mod;
  const SanitizerBinaryMetadataOptions Options;
  const Triple TargetTriple;
  IRBuilder<> IRB;
};

bool SanitizerBinaryMetadata::run() {
  MetadataInfoSet MIS;

  for (Function &F : Mod)
    runOn(F, MIS);

  if (MIS.empty())
    return false;

  //
  // Setup constructors and call all initialization functions for requested
  // metadata features.
  //

  auto *Int8PtrTy = IRB.getInt8PtrTy();
  auto *Int8PtrPtrTy = PointerType::getUnqual(Int8PtrTy);
  auto *Int32Ty = IRB.getInt32Ty();
  const std::array<Type *, 3> InitTypes = {Int32Ty, Int8PtrPtrTy, Int8PtrPtrTy};
  auto *Version = ConstantInt::get(Int32Ty, getVersion());

  for (const MetadataInfo *MI : MIS) {
    const std::array<Value *, InitTypes.size()> InitArgs = {
        Version,
        getSectionMarker(getSectionStart(MI->SectionSuffix), Int8PtrTy),
        getSectionMarker(getSectionEnd(MI->SectionSuffix), Int8PtrTy),
    };
    Function *Ctor =
        createSanitizerCtorAndInitFunctions(
            Mod, (MI->FunctionPrefix + ".module_ctor").str(),
            (MI->FunctionPrefix + "_add").str(), InitTypes, InitArgs)
            .first;
    Function *Dtor =
        createSanitizerCtorAndInitFunctions(
            Mod, (MI->FunctionPrefix + ".module_dtor").str(),
            (MI->FunctionPrefix + "_del").str(), InitTypes, InitArgs)
            .first;
    Constant *CtorData = nullptr;
    Constant *DtorData = nullptr;
    if (TargetTriple.supportsCOMDAT()) {
      // Use COMDAT to deduplicate constructor/destructor function.
      Ctor->setComdat(Mod.getOrInsertComdat(Ctor->getName()));
      Dtor->setComdat(Mod.getOrInsertComdat(Dtor->getName()));
      CtorData = Ctor;
      DtorData = Dtor;
    }
    appendToGlobalCtors(Mod, Ctor, kCtorDtorPriority, CtorData);
    appendToGlobalDtors(Mod, Dtor, kCtorDtorPriority, DtorData);
  }

  return true;
}

void SanitizerBinaryMetadata::runOn(Function &F, MetadataInfoSet &MIS) {
  if (F.empty())
    return;
  if (F.hasFnAttribute(Attribute::DisableSanitizerInstrumentation))
    return;
  // Don't touch available_externally functions, their actual body is elsewhere.
  if (F.getLinkage() == GlobalValue::AvailableExternallyLinkage)
    return;

  MDBuilder MDB(F.getContext());

  // The metadata features enabled for this function, stored along covered
  // metadata (if enabled).
  uint32_t FeatureMask = getEnabledPerInstructionFeature();
  // Don't emit unnecessary covered metadata for all functions to save space.
  bool RequiresCovered = false;
  // We can only understand if we need to set UAR feature after looking
  // at the instructions. So we need to check instructions even if FeatureMask
  // is empty.
  if (FeatureMask || Options.UAR) {
    for (BasicBlock &BB : F)
      for (Instruction &I : BB)
        RequiresCovered |= runOn(I, MIS, MDB, FeatureMask);
  }

  if (F.isVarArg())
    FeatureMask &= ~kSanitizerBinaryMetadataUAR;
  if (FeatureMask & kSanitizerBinaryMetadataUAR) {
    RequiresCovered = true;
    NumMetadataUAR++;
  }

  // Covered metadata is always emitted if explicitly requested, otherwise only
  // if some other metadata requires it to unambiguously interpret it for
  // modules compiled with SanitizerBinaryMetadata.
  if (Options.Covered || (FeatureMask && RequiresCovered)) {
    NumMetadataCovered++;
    const auto *MI = &MetadataInfo::Covered;
    MIS.insert(MI);
    const StringRef Section = getSectionName(MI->SectionSuffix);
    // The feature mask will be placed after the size (32 bit) of the function,
    // so in total one covered entry will use `sizeof(void*) + 4 + 4`.
    Constant *CFM = IRB.getInt32(FeatureMask);
    F.setMetadata(LLVMContext::MD_pcsections,
                  MDB.createPCSections({{Section, {CFM}}}));
  }
}

bool hasUseAfterReturnUnsafeUses(Value &V) {
  for (User *U : V.users()) {
    if (auto *I = dyn_cast<Instruction>(U)) {
      if (I->isLifetimeStartOrEnd() || I->isDroppable())
        continue;
      if (isa<LoadInst>(U))
        continue;
      if (auto *SI = dyn_cast<StoreInst>(U)) {
        // If storing TO the alloca, then the address isn't taken.
        if (SI->getOperand(1) == &V)
          continue;
      }
      if (auto *GEPI = dyn_cast<GetElementPtrInst>(U)) {
        if (!hasUseAfterReturnUnsafeUses(*GEPI))
          continue;
      } else if (auto *BCI = dyn_cast<BitCastInst>(U)) {
        if (!hasUseAfterReturnUnsafeUses(*BCI))
          continue;
      }
    }
    return true;
  }
  return false;
}

bool useAfterReturnUnsafe(Instruction &I) {
  if (isa<AllocaInst>(I))
    return hasUseAfterReturnUnsafeUses(I);
  // Tail-called functions are not necessary intercepted
  // at runtime because there is no call instruction.
  // So conservatively mark the caller as requiring checking.
  else if (auto *CI = dyn_cast<CallInst>(&I))
    return CI->isTailCall();
  return false;
}

bool SanitizerBinaryMetadata::runOn(Instruction &I, MetadataInfoSet &MIS,
                                    MDBuilder &MDB, uint32_t &FeatureMask) {
  SmallVector<const MetadataInfo *, 1> InstMetadata;
  bool RequiresCovered = false;

  if (Options.UAR && !(FeatureMask & kSanitizerBinaryMetadataUAR)) {
    if (useAfterReturnUnsafe(I))
      FeatureMask |= kSanitizerBinaryMetadataUAR;
  }

  if (Options.Atomics && I.mayReadOrWriteMemory()) {
    auto SSID = getAtomicSyncScopeID(&I);
    if (SSID.has_value() && *SSID != SyncScope::SingleThread) {
      NumMetadataAtomics++;
      InstMetadata.push_back(&MetadataInfo::Atomics);
    }
    RequiresCovered = true;
  }

  // Attach MD_pcsections to instruction.
  if (!InstMetadata.empty()) {
    MIS.insert(InstMetadata.begin(), InstMetadata.end());
    SmallVector<MDBuilder::PCSection, 1> Sections;
    for (const auto &MI : InstMetadata)
      Sections.push_back({getSectionName(MI->SectionSuffix), {}});
    I.setMetadata(LLVMContext::MD_pcsections, MDB.createPCSections(Sections));
  }

  return RequiresCovered;
}

GlobalVariable *
SanitizerBinaryMetadata::getSectionMarker(const Twine &MarkerName, Type *Ty) {
  // Use ExternalWeak so that if all sections are discarded due to section
  // garbage collection, the linker will not report undefined symbol errors.
  auto *Marker = new GlobalVariable(Mod, Ty, /*isConstant=*/false,
                                    GlobalVariable::ExternalWeakLinkage,
                                    /*Initializer=*/nullptr, MarkerName);
  Marker->setVisibility(GlobalValue::HiddenVisibility);
  return Marker;
}

StringRef SanitizerBinaryMetadata::getSectionName(StringRef SectionSuffix) {
  // FIXME: Other TargetTriple (req. string pool)
  return SectionSuffix;
}

Twine SanitizerBinaryMetadata::getSectionStart(StringRef SectionSuffix) {
  return "__start_" + SectionSuffix;
}

Twine SanitizerBinaryMetadata::getSectionEnd(StringRef SectionSuffix) {
  return "__stop_" + SectionSuffix;
}

} // namespace

SanitizerBinaryMetadataPass::SanitizerBinaryMetadataPass(
    SanitizerBinaryMetadataOptions Opts)
    : Options(std::move(Opts)) {}

PreservedAnalyses
SanitizerBinaryMetadataPass::run(Module &M, AnalysisManager<Module> &AM) {
  SanitizerBinaryMetadata Pass(M, Options);
  if (Pass.run())
    return PreservedAnalyses::none();
  return PreservedAnalyses::all();
}
