//===--- DriverOptions.cpp - Driver Options Table -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_Driver_Options.h"
#include "llvm_include_llvm_ADT_STLExtras.h"
#include "llvm_include_llvm_Option_OptTable.h"
#include "llvm_include_llvm_Option_Option.h"
#include <cassert>

using namespace clang::driver;
using namespace clang::driver::options;
using namespace llvm::opt;

#define PREFIX(NAME, VALUE)                                                    \
  static constexpr llvm::StringLiteral NAME##_init[] = VALUE;                  \
  static constexpr llvm::ArrayRef<llvm::StringLiteral> NAME(                   \
      NAME##_init, std::size(NAME##_init) - 1);
#include "clang_include_clang_Driver_Options.inc"
#undef PREFIX

static constexpr OptTable::Info InfoTable[] = {
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, ALIASARGS, FLAGS, PARAM,  \
               HELPTEXT, METAVAR, VALUES)                                      \
  {PREFIX, NAME,  HELPTEXT,    METAVAR,     OPT_##ID,  Option::KIND##Class,    \
   PARAM,  FLAGS, OPT_##GROUP, OPT_##ALIAS, ALIASARGS, VALUES},
#include "clang_include_clang_Driver_Options.inc"
#undef OPTION
};

namespace {

class DriverOptTable : public OptTable {
public:
  DriverOptTable()
    : OptTable(InfoTable) {}
};

}

const llvm::opt::OptTable &clang::driver::getDriverOptTable() {
  static const DriverOptTable *Table = []() {
    auto Result = std::make_unique<DriverOptTable>();
    // Options.inc is included in DriverOptions.cpp, and calls OptTable's
    // addValues function.
    // Opt is a variable used in the code fragment in Options.inc.
    OptTable &Opt = *Result;
#define OPTTABLE_ARG_INIT
#include "clang_include_clang_Driver_Options.inc"
#undef OPTTABLE_ARG_INIT
    return Result.release();
  }();
  return *Table;
}
