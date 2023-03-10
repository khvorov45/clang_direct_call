//===- PackedVersion.cpp --------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the Mach-O packed version.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_TextAPI_PackedVersion.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_ADT_StringExtras.h"
#include "llvm_include_llvm_Support_Format.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

namespace llvm {
namespace MachO {

bool PackedVersion::parse32(StringRef Str) {
  Version = 0;

  if (Str.empty())
    return false;

  SmallVector<StringRef, 3> Parts;
  SplitString(Str, Parts, ".");

  if (Parts.size() > 3)
    return false;

  unsigned long long Num;
  if (getAsUnsignedInteger(Parts[0], 10, Num))
    return false;

  if (Num > UINT16_MAX)
    return false;

  Version = Num << 16;

  for (unsigned i = 1, ShiftNum = 8; i < Parts.size(); ++i, ShiftNum -= 8) {
    if (getAsUnsignedInteger(Parts[i], 10, Num))
      return false;

    if (Num > UINT8_MAX)
      return false;

    Version |= (Num << ShiftNum);
  }

  return true;
}

std::pair<bool, bool> PackedVersion::parse64(StringRef Str) {
  bool Truncated = false;
  Version = 0;

  if (Str.empty())
    return std::make_pair(false, Truncated);

  SmallVector<StringRef, 5> Parts;
  SplitString(Str, Parts, ".");

  if (Parts.size() > 5)
    return std::make_pair(false, Truncated);

  unsigned long long Num;
  if (getAsUnsignedInteger(Parts[0], 10, Num))
    return std::make_pair(false, Truncated);

  if (Num > 0xFFFFFFULL)
    return std::make_pair(false, Truncated);

  if (Num > 0xFFFFULL) {
    Num = 0xFFFFULL;
    Truncated = true;
  }
  Version = Num << 16;

  for (unsigned i = 1, ShiftNum = 8; i < Parts.size() && i < 3;
       ++i, ShiftNum -= 8) {
    if (getAsUnsignedInteger(Parts[i], 10, Num))
      return std::make_pair(false, Truncated);

    if (Num > 0x3FFULL)
      return std::make_pair(false, Truncated);

    if (Num > 0xFFULL) {
      Num = 0xFFULL;
      Truncated = true;
    }
    Version |= (Num << ShiftNum);
  }

  if (Parts.size() > 3)
    Truncated = true;

  return std::make_pair(true, Truncated);
}

void PackedVersion::print(raw_ostream &OS) const {
  OS << format("%d", getMajor());
  if (getMinor() || getSubminor())
    OS << format(".%d", getMinor());
  if (getSubminor())
    OS << format(".%d", getSubminor());
}

} // end namespace MachO.
} // end namespace llvm.
