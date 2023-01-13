//===- clang/Basic/DirectoryEntry.h - Directory references ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Defines interfaces for clang::DirectoryEntry and clang::DirectoryEntryRef.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_DIRECTORYENTRY_H
#define LLVM_CLANG_BASIC_DIRECTORYENTRY_H

#include "clang_include_clang_Basic_CustomizableOptional.h"
#include "clang_include_clang_Basic_LLVM.h"
#include "llvm_include_llvm_ADT_DenseMapInfo.h"
#include "llvm_include_llvm_ADT_Hashing.h"
#include "llvm_include_llvm_ADT_STLExtras.h"
#include "llvm_include_llvm_ADT_StringMap.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_Support_ErrorOr.h"

#include <utility>

namespace clang {
namespace FileMgr {

template <class RefTy> class MapEntryOptionalStorage;

} // end namespace FileMgr

/// Cached information about one directory (either on disk or in
/// the virtual file system).
class DirectoryEntry {
  DirectoryEntry() = default;
  DirectoryEntry(const DirectoryEntry &) = delete;
  DirectoryEntry &operator=(const DirectoryEntry &) = delete;
  friend class FileManager;
  friend class FileEntryTestHelper;

  // FIXME: We should not be storing a directory entry name here.
  StringRef Name; // Name of the directory.

public:
  StringRef getName() const { return Name; }
};

/// A reference to a \c DirectoryEntry  that includes the name of the directory
/// as it was accessed by the FileManager's client.
class DirectoryEntryRef {
public:
  const DirectoryEntry &getDirEntry() const { return *ME->getValue(); }

  StringRef getName() const { return ME->getKey(); }

  /// Hash code is based on the DirectoryEntry, not the specific named
  /// reference.
  friend llvm::hash_code hash_value(DirectoryEntryRef Ref) {
    return llvm::hash_value(&Ref.getDirEntry());
  }

  using MapEntry = llvm::StringMapEntry<llvm::ErrorOr<DirectoryEntry &>>;

  const MapEntry &getMapEntry() const { return *ME; }

  /// Check if RHS referenced the file in exactly the same way.
  bool isSameRef(DirectoryEntryRef RHS) const { return ME == RHS.ME; }

  DirectoryEntryRef() = delete;
  DirectoryEntryRef(const MapEntry &ME) : ME(&ME) {}

  /// Allow DirectoryEntryRef to degrade into 'const DirectoryEntry*' to
  /// facilitate incremental adoption.
  ///
  /// The goal is to avoid code churn due to dances like the following:
  /// \code
  /// // Old code.
  /// lvalue = rvalue;
  ///
  /// // Temporary code from an incremental patch.
  /// lvalue = &rvalue.getDirectoryEntry();
  ///
  /// // Final code.
  /// lvalue = rvalue;
  /// \endcode
  ///
  /// FIXME: Once DirectoryEntryRef is "everywhere" and DirectoryEntry::getName
  /// has been deleted, delete this implicit conversion.
  operator const DirectoryEntry *() const { return &getDirEntry(); }

private:
  friend class FileMgr::MapEntryOptionalStorage<DirectoryEntryRef>;
  struct optional_none_tag {};

  // Private constructor for use by OptionalStorage.
  DirectoryEntryRef(optional_none_tag) : ME(nullptr) {}
  bool hasOptionalValue() const { return ME; }

  friend struct llvm::DenseMapInfo<DirectoryEntryRef>;
  struct dense_map_empty_tag {};
  struct dense_map_tombstone_tag {};

  // Private constructors for use by DenseMapInfo.
  DirectoryEntryRef(dense_map_empty_tag)
      : ME(llvm::DenseMapInfo<const MapEntry *>::getEmptyKey()) {}
  DirectoryEntryRef(dense_map_tombstone_tag)
      : ME(llvm::DenseMapInfo<const MapEntry *>::getTombstoneKey()) {}
  bool isSpecialDenseMapKey() const {
    return isSameRef(DirectoryEntryRef(dense_map_empty_tag())) ||
           isSameRef(DirectoryEntryRef(dense_map_tombstone_tag()));
  }

  const MapEntry *ME;
};

using OptionalDirectoryEntryRef = CustomizableOptional<DirectoryEntryRef>;

namespace FileMgr {

/// Customized storage for refs derived from map entires in FileManager, using
/// the private optional_none_tag to keep it to the size of a single pointer.
template <class RefTy> class MapEntryOptionalStorage {
  using optional_none_tag = typename RefTy::optional_none_tag;
  RefTy MaybeRef;

public:
  MapEntryOptionalStorage() : MaybeRef(optional_none_tag()) {}

  template <class... ArgTypes>
  explicit MapEntryOptionalStorage(std::in_place_t, ArgTypes &&...Args)
      : MaybeRef(std::forward<ArgTypes>(Args)...) {}

  void reset() { MaybeRef = optional_none_tag(); }

  bool has_value() const { return MaybeRef.hasOptionalValue(); }

  RefTy &value() & {
    assert(has_value());
    return MaybeRef;
  }
  RefTy const &value() const & {
    assert(has_value());
    return MaybeRef;
  }
  RefTy &&value() && {
    assert(has_value());
    return std::move(MaybeRef);
  }

  template <class... Args> void emplace(Args &&...args) {
    MaybeRef = RefTy(std::forward<Args>(args)...);
  }

  MapEntryOptionalStorage &operator=(RefTy Ref) {
    MaybeRef = Ref;
    return *this;
  }
};

} // end namespace FileMgr

namespace optional_detail {

/// Customize OptionalStorage<DirectoryEntryRef> to use DirectoryEntryRef and
/// its optional_none_tag to keep it the size of a single pointer.
template <>
class OptionalStorage<clang::DirectoryEntryRef>
    : public clang::FileMgr::MapEntryOptionalStorage<clang::DirectoryEntryRef> {
  using StorageImpl =
      clang::FileMgr::MapEntryOptionalStorage<clang::DirectoryEntryRef>;

public:
  OptionalStorage() = default;

  template <class... ArgTypes>
  explicit OptionalStorage(std::in_place_t, ArgTypes &&...Args)
      : StorageImpl(std::in_place_t{}, std::forward<ArgTypes>(Args)...) {}

  OptionalStorage &operator=(clang::DirectoryEntryRef Ref) {
    StorageImpl::operator=(Ref);
    return *this;
  }
};

static_assert(sizeof(OptionalDirectoryEntryRef) == sizeof(DirectoryEntryRef),
              "OptionalDirectoryEntryRef must avoid size overhead");

static_assert(std::is_trivially_copyable<OptionalDirectoryEntryRef>::value,
              "OptionalDirectoryEntryRef should be trivially copyable");

} // end namespace optional_detail
} // namespace clang

namespace llvm {
/// Specialisation of DenseMapInfo for DirectoryEntryRef.
template <> struct DenseMapInfo<clang::DirectoryEntryRef> {
  static inline clang::DirectoryEntryRef getEmptyKey() {
    return clang::DirectoryEntryRef(
        clang::DirectoryEntryRef::dense_map_empty_tag());
  }

  static inline clang::DirectoryEntryRef getTombstoneKey() {
    return clang::DirectoryEntryRef(
        clang::DirectoryEntryRef::dense_map_tombstone_tag());
  }

  static unsigned getHashValue(clang::DirectoryEntryRef Val) {
    return hash_value(Val);
  }

  static bool isEqual(clang::DirectoryEntryRef LHS,
                      clang::DirectoryEntryRef RHS) {
    // Catch the easy cases: both empty, both tombstone, or the same ref.
    if (LHS.isSameRef(RHS))
      return true;

    // Confirm LHS and RHS are valid.
    if (LHS.isSpecialDenseMapKey() || RHS.isSpecialDenseMapKey())
      return false;

    // It's safe to use operator==.
    return LHS == RHS;
  }
};

} // end namespace llvm

namespace clang {

/// Wrapper around OptionalDirectoryEntryRef that degrades to 'const
/// DirectoryEntry*', facilitating incremental patches to propagate
/// DirectoryEntryRef.
///
/// This class can be used as return value or field where it's convenient for
/// an OptionalDirectoryEntryRef to degrade to a 'const DirectoryEntry*'. The
/// purpose is to avoid code churn due to dances like the following:
/// \code
/// // Old code.
/// lvalue = rvalue;
///
/// // Temporary code from an incremental patch.
/// OptionalDirectoryEntryRef MaybeF = rvalue;
/// lvalue = MaybeF ? &MaybeF.getDirectoryEntry() : nullptr;
///
/// // Final code.
/// lvalue = rvalue;
/// \endcode
///
/// FIXME: Once DirectoryEntryRef is "everywhere" and DirectoryEntry::LastRef
/// and DirectoryEntry::getName have been deleted, delete this class and
/// replace instances with OptionalDirectoryEntryRef.
class OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr
    : public OptionalDirectoryEntryRef {
public:
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr() = default;
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr(
      OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &&) = default;
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr(
      const OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &) = default;
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &
  operator=(OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &&) = default;
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &
  operator=(const OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &) = default;

  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr(std::nullopt_t) {}
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr(DirectoryEntryRef Ref)
      : OptionalDirectoryEntryRef(Ref) {}
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr(
      OptionalDirectoryEntryRef MaybeRef)
      : OptionalDirectoryEntryRef(MaybeRef) {}

  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &
  operator=(std::nullopt_t) {
    OptionalDirectoryEntryRef::operator=(std::nullopt);
    return *this;
  }
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &operator=(DirectoryEntryRef Ref) {
    OptionalDirectoryEntryRef::operator=(Ref);
    return *this;
  }
  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr &
  operator=(OptionalDirectoryEntryRef MaybeRef) {
    OptionalDirectoryEntryRef::operator=(MaybeRef);
    return *this;
  }

  /// Degrade to 'const DirectoryEntry *' to allow  DirectoryEntry::LastRef and
  /// DirectoryEntry::getName have been deleted, delete this class and replace
  /// instances with OptionalDirectoryEntryRef
  operator const DirectoryEntry *() const {
    return has_value() ? &(*this)->getDirEntry() : nullptr;
  }
};

static_assert(std::is_trivially_copyable<
                  OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr>::value,
              "OptionalDirectoryEntryRefDegradesToDirectoryEntryPtr should be "
              "trivially copyable");

} // end namespace clang

#endif // LLVM_CLANG_BASIC_DIRECTORYENTRY_H
