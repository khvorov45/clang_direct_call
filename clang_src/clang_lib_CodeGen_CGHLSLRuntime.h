//===----- CGHLSLRuntime.h - Interface to HLSL Runtimes -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This provides an abstract class for HLSL code generation.  Concrete
// subclasses of this implement code generation for specific HLSL
// runtime libraries.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_CODEGEN_CGHLSLRUNTIME_H
#define LLVM_CLANG_LIB_CODEGEN_CGHLSLRUNTIME_H

#include "llvm_include_llvm_IR_IRBuilder.h"

#include "clang_include_clang_Basic_HLSLRuntime.h"

#include "llvm_include_llvm_ADT_Optional.h"
#include "llvm_include_llvm_ADT_SmallVector.h"
#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_Frontend_HLSL_HLSLResource.h"

#include <vector>

namespace llvm {
class GlobalVariable;
class Function;
class StructType;
} // namespace llvm

namespace clang {
class VarDecl;
class ParmVarDecl;
class HLSLBufferDecl;
class HLSLResourceBindingAttr;
class Type;
class DeclContext;

class FunctionDecl;

namespace CodeGen {

class CodeGenModule;

class CGHLSLRuntime {
public:
  struct BufferResBinding {
    // The ID like 2 in register(b2, space1).
    llvm::Optional<unsigned> Reg;
    // The Space like 1 is register(b2, space1).
    // Default value is 0.
    unsigned Space;
    BufferResBinding(HLSLResourceBindingAttr *Attr);
  };
  struct Buffer {
    Buffer(const HLSLBufferDecl *D);
    llvm::StringRef Name;
    // IsCBuffer - Whether the buffer is a cbuffer (and not a tbuffer).
    bool IsCBuffer;
    BufferResBinding Binding;
    // Global variable and offset for each constant.
    std::vector<std::pair<llvm::GlobalVariable *, unsigned>> Constants;
    llvm::StructType *LayoutStruct = nullptr;
  };

protected:
  CodeGenModule &CGM;

  llvm::Value *emitInputSemantic(llvm::IRBuilder<> &B, const ParmVarDecl &D,
                                 llvm::Type *Ty);

public:
  CGHLSLRuntime(CodeGenModule &CGM) : CGM(CGM) {}
  virtual ~CGHLSLRuntime() {}

  void annotateHLSLResource(const VarDecl *D, llvm::GlobalVariable *GV);
  void generateGlobalCtorDtorCalls();

  void addBuffer(const HLSLBufferDecl *D);
  void finishCodeGen();

  void setHLSLEntryAttributes(const FunctionDecl *FD, llvm::Function *Fn);

  void emitEntryFunction(const FunctionDecl *FD, llvm::Function *Fn);
  void setHLSLFunctionAttributes(llvm::Function *, const FunctionDecl *);

private:
  void addBufferResourceAnnotation(llvm::GlobalVariable *GV,
                                   llvm::StringRef TyName,
                                   llvm::hlsl::ResourceClass RC,
                                   llvm::hlsl::ResourceKind RK,
                                   BufferResBinding &Binding);
  void addConstant(VarDecl *D, Buffer &CB);
  void addBufferDecls(const DeclContext *DC, Buffer &CB);
  llvm::SmallVector<Buffer> Buffers;
};

} // namespace CodeGen
} // namespace clang

#endif
