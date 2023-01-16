//===--- TargetRegistry.cpp - Target registration -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_MC_TargetRegistry.h"
#include "llvm_include_llvm_ADT_STLExtras.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
#include <cassert>
#include <vector>
using namespace llvm;

// Clients are responsible for avoid race conditions in registration.
static Target* TheTarget = nullptr;

iterator_range<TargetRegistry::iterator>
TargetRegistry::targets() {
    return make_range(iterator(TheTarget), iterator());
}

const Target*
TargetRegistry::getTarget(void) {
    return TheTarget;
}

void
TargetRegistry::AddTarget(Target& T) {
    T.Next = TheTarget;
    TheTarget = &T;
}

static int
TargetArraySortFn(const std::pair<StringRef, const Target*>* LHS, const std::pair<StringRef, const Target*>* RHS) {
    return LHS->first.compare(RHS->first);
}

void
TargetRegistry::printRegisteredTargetsForVersion(raw_ostream& OS) {
    std::vector<std::pair<StringRef, const Target*>> Targets;
    size_t                                           Width = 0;
    for (const auto& T : TargetRegistry::targets()) {
        Targets.push_back(std::make_pair(T.getName(), &T));
        Width = std::max(Width, Targets.back().first.size());
    }
    array_pod_sort(Targets.begin(), Targets.end(), TargetArraySortFn);

    OS << "\n";
    OS << "  Registered Targets:\n";
    for (const auto& Target : Targets) {
        OS << "    " << Target.first;
        OS.indent(Width - Target.first.size())
            << " - " << Target.second->getShortDescription() << '\n';
    }
    if (Targets.empty())
        OS << "    (none)\n";
}
