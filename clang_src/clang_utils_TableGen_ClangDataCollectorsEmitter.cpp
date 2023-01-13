#include "clang_utils_TableGen_TableGenBackends.h"
#include "llvm_include_llvm_TableGen_Record.h"
#include "llvm_include_llvm_TableGen_TableGenBackend.h"

using namespace llvm;

void clang::EmitClangDataCollectors(RecordKeeper &RK, raw_ostream &OS) {
  const auto &Defs = RK.getClasses();
  for (const auto &Entry : Defs) {
    Record &R = *Entry.second;
    OS << "DEF_ADD_DATA(" << R.getName() << ", {\n";
    auto Code = R.getValue("Code")->getValue();
    OS << Code->getAsUnquotedString() << "}\n)";
    OS << "\n";
  }
  OS << "#undef DEF_ADD_DATA\n";
}
