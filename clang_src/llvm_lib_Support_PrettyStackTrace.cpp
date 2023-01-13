//===- PrettyStackTrace.cpp - Pretty Crash Handling -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines some helpful functions for dealing with the possibility of
// Unix signals occurring while your program is running.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Support_PrettyStackTrace.h"
#include "llvm_include_llvm-c_ErrorHandling.h"
#include "llvm_include_llvm_Config_config.h"
#include "llvm_include_llvm_Support_Compiler.h"
#include "llvm_include_llvm_Support_SaveAndRestore.h"
#include "llvm_include_llvm_Support_Signals.h"
#include "llvm_include_llvm_Support_Watchdog.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

#ifdef __APPLE__
#include "llvm_include_llvm_ADT_SmallString.h"
#endif

#include <atomic>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <tuple>

#ifdef HAVE_CRASHREPORTERCLIENT_H
#include <CrashReporterClient.h>
#endif

using namespace llvm;

PrettyStackTraceEntry::PrettyStackTraceEntry() {
#if ENABLE_BACKTRACES
  // Handle SIGINFO first, because we haven't finished constructing yet.
  printForSigInfoIfNeeded();
  // Link ourselves.
  NextEntry = PrettyStackTraceHead;
  PrettyStackTraceHead = this;
#endif
}

PrettyStackTraceEntry::~PrettyStackTraceEntry() {
#if ENABLE_BACKTRACES
  assert(PrettyStackTraceHead == this &&
         "Pretty stack trace entry destruction is out of order");
  PrettyStackTraceHead = NextEntry;
  // Handle SIGINFO first, because we already started destructing.
  printForSigInfoIfNeeded();
#endif
}

void PrettyStackTraceString::print(raw_ostream &OS) const { OS << Str << "\n"; }

PrettyStackTraceFormat::PrettyStackTraceFormat(const char *Format, ...) {
  va_list AP;
  va_start(AP, Format);
  const int SizeOrError = vsnprintf(nullptr, 0, Format, AP);
  va_end(AP);
  if (SizeOrError < 0) {
    return;
  }

  const int Size = SizeOrError + 1; // '\0'
  Str.resize(Size);
  va_start(AP, Format);
  vsnprintf(Str.data(), Size, Format, AP);
  va_end(AP);
}

void PrettyStackTraceFormat::print(raw_ostream &OS) const { OS << Str << "\n"; }

void PrettyStackTraceProgram::print(raw_ostream &OS) const {
  OS << "Program arguments: ";
  // Print the argument list.
  for (int I = 0; I < ArgC; ++I) {
    const bool HaveSpace = ::strchr(ArgV[I], ' ');
    if (I)
      OS << ' ';
    if (HaveSpace)
      OS << '"';
    OS.write_escaped(ArgV[I]);
    if (HaveSpace)
      OS << '"';
  }
  OS << '\n';
}

#if ENABLE_BACKTRACES
static bool RegisterCrashPrinter() {
  sys::AddSignalHandler(CrashHandler, nullptr);
  return false;
}
#endif

void llvm::EnablePrettyStackTrace() {
#if ENABLE_BACKTRACES
  // The first time this is called, we register the crash printer.
  static bool HandlerRegistered = RegisterCrashPrinter();
  (void)HandlerRegistered;
#endif
}

void llvm::EnablePrettyStackTraceOnSigInfoForThisThread(bool ShouldEnable) {
#if ENABLE_BACKTRACES
  if (!ShouldEnable) {
    ThreadLocalSigInfoGenerationCounter = 0;
    return;
  }

  // The first time this is called, we register the SIGINFO handler.
  static bool HandlerRegistered = []{
    sys::SetInfoSignalFunction([]{
      GlobalSigInfoGenerationCounter.fetch_add(1, std::memory_order_relaxed);
    });
    return false;
  }();
  (void)HandlerRegistered;

  // Next, enable it for the current thread.
  ThreadLocalSigInfoGenerationCounter =
      GlobalSigInfoGenerationCounter.load(std::memory_order_relaxed);
#endif
}

const void *llvm::SavePrettyStackState() {
#if ENABLE_BACKTRACES
  return PrettyStackTraceHead;
#else
  return nullptr;
#endif
}

void llvm::RestorePrettyStackState(const void *Top) {
#if ENABLE_BACKTRACES
  PrettyStackTraceHead =
      static_cast<PrettyStackTraceEntry *>(const_cast<void *>(Top));
#endif
}

void LLVMEnablePrettyStackTrace() {
  EnablePrettyStackTrace();
}
