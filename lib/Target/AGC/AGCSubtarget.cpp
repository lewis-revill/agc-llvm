//===-- AGCSubtarget.cpp - AGC Subtarget Information ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the AGC specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "AGCSubtarget.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "agc-subtarget"

#define GET_SUBTARGETINFO_MC_DESC
#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "AGCGenSubtargetInfo.inc"

void AGCSubtarget::anchor() {}

AGCSubtarget::AGCSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const TargetMachine &TM)
    : AGCGenSubtargetInfo(TT, CPU, FS), InstrInfo(), RegInfo(getHwMode()),
      FLInfo(), TLInfo(TM, *this) {
  std::string CPUName = CPU;
  if (CPUName.empty())
    CPUName = "agc-block2";
  ParseSubtargetFeatures(CPUName, FS);
}
