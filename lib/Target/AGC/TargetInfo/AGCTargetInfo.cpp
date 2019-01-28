//===-- AGCTargetInfo.cpp - AGC Target Implementation ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
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
