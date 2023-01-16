#include "llvm_include_llvm_MC_TargetRegistry.h"

static llvm::Target* TheTarget = nullptr;

const llvm::Target*
llvm::TargetRegistry::getTarget(void) {
    return TheTarget;
}

void
llvm::TargetRegistry::setTarget(llvm::Target& T) {
    TheTarget = &T;
}
