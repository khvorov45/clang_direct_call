//===- MC/MCAsmInfoXCOFF.cpp - XCOFF asm properties ------------ *- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_MCAsmInfoXCOFF.h"
#include "llvm_include_llvm_ADT_StringExtras.h"
#include "llvm_include_llvm_Support_CommandLine.h"

using namespace llvm;

namespace llvm {
extern cl::opt<cl::boolOrDefault> UseLEB128Directives;
}

void MCAsmInfoXCOFF::anchor() {}

MCAsmInfoXCOFF::MCAsmInfoXCOFF() {
  IsLittleEndian = false;
  HasVisibilityOnlyWithLinkage = true;
  HasBasenameOnlyForFileDirective = false;
  HasFourStringsDotFile = true;

  // For XCOFF, string constant consists of any number of characters enclosed in
  // "" (double quotation marks).
  HasPairedDoubleQuoteStringConstants = true;

  PrivateGlobalPrefix = "L..";
  PrivateLabelPrefix = "L..";
  SupportsQuotedNames = false;
  UseDotAlignForAlignment = true;
  UsesDwarfFileAndLocDirectives = false;
  DwarfSectionSizeRequired = false;
  if (UseLEB128Directives == cl::BOU_UNSET)
    HasLEB128Directives = false;
  ZeroDirective = "\t.space\t";
  ZeroDirectiveSupportsNonZeroValue = false;
  AsciiDirective = nullptr; // not supported
  AscizDirective = nullptr; // not supported
  ByteListDirective = "\t.byte\t";
  PlainStringDirective = "\t.string\t";
  CharacterLiteralSyntax = ACLS_SingleQuotePrefix;

  // Use .vbyte for data definition to avoid directives that apply an implicit
  // alignment.
  Data16bitsDirective = "\t.vbyte\t2, ";
  Data32bitsDirective = "\t.vbyte\t4, ";

  COMMDirectiveAlignmentIsInBytes = false;
  LCOMMDirectiveAlignmentType = LCOMM::Log2Alignment;
  HasDotTypeDotSizeDirective = false;
  UseIntegratedAssembler = false;
  ParseInlineAsmUsingAsmParser = true;
  NeedsFunctionDescriptors = true;

  ExceptionsType = ExceptionHandling::AIX;
}

bool MCAsmInfoXCOFF::isAcceptableChar(char C) const {
  // QualName is allowed for a MCSymbolXCOFF, and
  // QualName contains '[' and ']'.
  if (C == '[' || C == ']')
    return true;

  // For AIX assembler, symbols may consist of numeric digits,
  // underscores, periods, uppercase or lowercase letters, or
  // any combination of these.
  return isAlnum(C) || C == '_' || C == '.';
}
