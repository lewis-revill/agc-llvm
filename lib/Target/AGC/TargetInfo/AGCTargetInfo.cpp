//===-- AGCTargetInfo.cpp - AGC Target Implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

namespace llvm {
Target &getTheAGCTarget() {
  static Target TheAGCTarget;
  return TheAGCTarget;
}
} // namespace llvm

extern "C" void LLVMInitializeAGCTargetInfo() {
  RegisterTarget<Triple::agc> A(getTheAGCTarget(), "agc",
                                "Apollo Guidance Computer", "AGC");
}
