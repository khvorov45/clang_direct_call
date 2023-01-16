#include "llvm_include_llvm_MC_TargetRegistry.h"

// Clients are responsible for avoid race conditions in registration.
static llvm::Target* TheTarget = nullptr;

const llvm::Target*
llvm::TargetRegistry::getTarget(void) {
    return TheTarget;
}

void
llvm::TargetRegistry::AddTarget(llvm::Target& T) {
    T.Next = TheTarget;
    TheTarget = &T;
}
