//===-- Process.cpp - Implement OS Process Concept --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements the operating system Process concept.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Support_Process.h"
#include "llvm_include_llvm_ADT_STLExtras.h"
#include "llvm_include_llvm_ADT_StringExtras.h"
#include "llvm_include_llvm_Config_config.h"
#include "llvm_include_llvm_Config_llvm-config.h"
#include "llvm_include_llvm_Support_CrashRecoveryContext.h"
#include "llvm_include_llvm_Support_FileSystem.h"
#include "llvm_include_llvm_Support_Path.h"
#include "llvm_include_llvm_Support_Program.h"

#include <optional>
#include <stdlib.h> // for _Exit

using namespace llvm;
using namespace sys;

//===----------------------------------------------------------------------===//
//=== WARNING: Implementation here must contain only TRULY operating system
//===          independent code.
//===----------------------------------------------------------------------===//

std::optional<std::string>
Process::FindInEnvPath(StringRef EnvName, StringRef FileName, char Separator) {
  return FindInEnvPath(EnvName, FileName, {}, Separator);
}

std::optional<std::string>
Process::FindInEnvPath(StringRef EnvName, StringRef FileName,
                       ArrayRef<std::string> IgnoreList, char Separator) {
  assert(!path::is_absolute(FileName));
  std::optional<std::string> FoundPath;
  std::optional<std::string> OptPath = Process::GetEnv(EnvName);
  if (!OptPath)
    return FoundPath;

  const char EnvPathSeparatorStr[] = {Separator, '\0'};
  SmallVector<StringRef, 8> Dirs;
  SplitString(*OptPath, Dirs, EnvPathSeparatorStr);

  for (StringRef Dir : Dirs) {
    if (Dir.empty())
      continue;

    if (any_of(IgnoreList, [&](StringRef S) { return fs::equivalent(S, Dir); }))
      continue;

    SmallString<128> FilePath(Dir);
    path::append(FilePath, FileName);
    if (fs::exists(Twine(FilePath))) {
      FoundPath = std::string(FilePath.str());
      break;
    }
  }

  return FoundPath;
}


#define COLOR(FGBG, CODE, BOLD) "\033[0;" BOLD FGBG CODE "m"

#define ALLCOLORS(FGBG,BOLD) {\
    COLOR(FGBG, "0", BOLD),\
    COLOR(FGBG, "1", BOLD),\
    COLOR(FGBG, "2", BOLD),\
    COLOR(FGBG, "3", BOLD),\
    COLOR(FGBG, "4", BOLD),\
    COLOR(FGBG, "5", BOLD),\
    COLOR(FGBG, "6", BOLD),\
    COLOR(FGBG, "7", BOLD)\
  }

static const char colorcodes[2][2][8][10] = {
 { ALLCOLORS("3",""), ALLCOLORS("3","1;") },
 { ALLCOLORS("4",""), ALLCOLORS("4","1;") }
};

// A CMake option controls wheter we emit core dumps by default. An application
// may disable core dumps by calling Process::PreventCoreFiles().
static bool coreFilesPrevented = !LLVM_ENABLE_CRASH_DUMPS;

bool Process::AreCoreFilesPrevented() { return coreFilesPrevented; }

[[noreturn]] void Process::Exit(int RetCode, bool NoCleanup) {
  if (CrashRecoveryContext *CRC = CrashRecoveryContext::GetCurrent())
    CRC->HandleExit(RetCode);

  if (NoCleanup)
    ExitNoCleanup(RetCode);
  else
    ::exit(RetCode);
}

// Include the platform-specific parts of this class.
#ifdef LLVM_ON_UNIX
#include "llvm_lib_Support_Unix_Process.inc"
#endif
#ifdef _WIN32
#include "llvm_lib_Support_Windows_Process.inc"
#endif
