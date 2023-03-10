//===-- Vectorize.cpp -----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements common infrastructure for libLLVMVectorizeOpts.a, which
// implements several vectorization transformations over the LLVM intermediate
// representation, including the C bindings for that library.
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_Transforms_Vectorize.h"
#include "llvm_include_llvm-c_Initialization.h"
#include "llvm_include_llvm-c_Transforms_Vectorize.h"
#include "llvm_include_llvm_IR_LegacyPassManager.h"
#include "llvm_include_llvm_InitializePasses.h"
#include "llvm_include_llvm_PassRegistry.h"

using namespace llvm;

/// Initialize all passes linked into the Vectorization library.
void llvm::initializeVectorization(PassRegistry &Registry) {
  initializeLoopVectorizePass(Registry);
  initializeSLPVectorizerPass(Registry);
  initializeLoadStoreVectorizerLegacyPassPass(Registry);
  initializeVectorCombineLegacyPassPass(Registry);
}

void LLVMInitializeVectorization(LLVMPassRegistryRef R) {
  initializeVectorization(*unwrap(R));
}

void LLVMAddLoopVectorizePass(LLVMPassManagerRef PM) {
  unwrap(PM)->add(createLoopVectorizePass());
}

void LLVMAddSLPVectorizePass(LLVMPassManagerRef PM) {
  unwrap(PM)->add(createSLPVectorizerPass());
}
