/*===-- llvm-c/TargetMachine.h - Target Machine Library C Interface - C++ -*-=*\
|*                                                                            *|
|* Part of the LLVM Project, under the Apache License v2.0 with LLVM          *|
|* Exceptions.                                                                *|
|* See https://llvm.org/LICENSE.txt for license information.                  *|
|* SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception                    *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|* This header declares the C interface to the Target and TargetMachine       *|
|* classes, which can be used to generate assembly or object files.           *|
|*                                                                            *|
|* Many exotic languages can interoperate with C code but have a harder time  *|
|* with C++ due to name mangling. So in addition to C, this interface enables *|
|* tools written in such languages.                                           *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef LLVM_C_TARGETMACHINE_H
#define LLVM_C_TARGETMACHINE_H

#include "llvm_include_llvm-c_ExternC.h"
#include "llvm_include_llvm-c_Target.h"
#include "llvm_include_llvm-c_Types.h"

LLVM_C_EXTERN_C_BEGIN

/**
 * @addtogroup LLVMCTarget
 *
 * @{
 */

typedef struct LLVMOpaqueTargetMachine *LLVMTargetMachineRef;
typedef struct LLVMTarget *LLVMTargetRef;

typedef enum {
    LLVMCodeGenLevelNone,
    LLVMCodeGenLevelLess,
    LLVMCodeGenLevelDefault,
    LLVMCodeGenLevelAggressive
} LLVMCodeGenOptLevel;

typedef enum {
    LLVMRelocDefault,
    LLVMRelocStatic,
    LLVMRelocPIC,
    LLVMRelocDynamicNoPic,
    LLVMRelocROPI,
    LLVMRelocRWPI,
    LLVMRelocROPI_RWPI
} LLVMRelocMode;

typedef enum {
    LLVMCodeModelDefault,
    LLVMCodeModelJITDefault,
    LLVMCodeModelTiny,
    LLVMCodeModelSmall,
    LLVMCodeModelKernel,
    LLVMCodeModelMedium,
    LLVMCodeModelLarge
} LLVMCodeModel;

typedef enum {
    LLVMAssemblyFile,
    LLVMObjectFile
} LLVMCodeGenFileType;

/*===-- Target ------------------------------------------------------------===*/

/** Finds the target corresponding to the given triple and stores it in \p T.
  Returns 0 on success. Optionally returns any error in ErrorMessage.
  Use LLVMDisposeMessage to dispose the message. */
LLVMBool LLVMGetTargetFromTriple(const char* Triple, LLVMTargetRef *T,
                                 char **ErrorMessage);

/*===-- Target Machine ----------------------------------------------------===*/
/** Returns the Target used in a TargetMachine */
LLVMTargetRef LLVMGetTargetMachineTarget(LLVMTargetMachineRef T);

/** Returns the triple used creating this target machine. See
  llvm::TargetMachine::getTriple. The result needs to be disposed with
  LLVMDisposeMessage. */
char *LLVMGetTargetMachineTriple(LLVMTargetMachineRef T);

/** Returns the cpu used creating this target machine. See
  llvm::TargetMachine::getCPU. The result needs to be disposed with
  LLVMDisposeMessage. */
char *LLVMGetTargetMachineCPU(LLVMTargetMachineRef T);

/** Returns the feature string used creating this target machine. See
  llvm::TargetMachine::getFeatureString. The result needs to be disposed with
  LLVMDisposeMessage. */
char *LLVMGetTargetMachineFeatureString(LLVMTargetMachineRef T);

/** Create a DataLayout based on the targetMachine. */
LLVMTargetDataRef LLVMCreateTargetDataLayout(LLVMTargetMachineRef T);

/** Set the target machine's ASM verbosity. */
void LLVMSetTargetMachineAsmVerbosity(LLVMTargetMachineRef T,
                                      LLVMBool VerboseAsm);

/** Emits an asm or object file for the given module to the filename. This
  wraps several c++ only classes (among them a file stream). Returns any
  error in ErrorMessage. Use LLVMDisposeMessage to dispose the message. */
LLVMBool LLVMTargetMachineEmitToFile(LLVMTargetMachineRef T, LLVMModuleRef M,
                                     const char *Filename,
                                     LLVMCodeGenFileType codegen,
                                     char **ErrorMessage);

/** Compile the LLVM IR stored in \p M and store the result in \p OutMemBuf. */
LLVMBool LLVMTargetMachineEmitToMemoryBuffer(LLVMTargetMachineRef T, LLVMModuleRef M,
  LLVMCodeGenFileType codegen, char** ErrorMessage, LLVMMemoryBufferRef *OutMemBuf);

/*===-- Triple ------------------------------------------------------------===*/
/** Get a triple for the host machine as a string. The result needs to be
  disposed with LLVMDisposeMessage. */
char* LLVMGetDefaultTargetTriple(void);

/** Normalize a target triple. The result needs to be disposed with
  LLVMDisposeMessage. */
char* LLVMNormalizeTargetTriple(const char* triple);

/** Get the host CPU as a string. The result needs to be disposed with
  LLVMDisposeMessage. */
char* LLVMGetHostCPUName(void);

/** Get the host CPU's features as a string. The result needs to be disposed
  with LLVMDisposeMessage. */
char* LLVMGetHostCPUFeatures(void);

/** Adds the target-specific analysis passes to the pass manager. */
void LLVMAddAnalysisPasses(LLVMTargetMachineRef T, LLVMPassManagerRef PM);

/**
 * @}
 */

LLVM_C_EXTERN_C_END

#endif
