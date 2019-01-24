//===-- AGCSubtarget.cpp - AGC Subtarget Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
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
