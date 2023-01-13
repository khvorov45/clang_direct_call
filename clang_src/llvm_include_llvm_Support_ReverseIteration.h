#ifndef LLVM_SUPPORT_REVERSEITERATION_H
#define LLVM_SUPPORT_REVERSEITERATION_H

#include "llvm_include_llvm_Config_abi-breaking.h"
#include "llvm_include_llvm_Support_PointerLikeTypeTraits.h"

namespace llvm {

template<class T = void *>
bool shouldReverseIterate() {
#if LLVM_ENABLE_REVERSE_ITERATION
  return detail::IsPointerLike<T>::value;
#else
  return false;
#endif
}

}
#endif
