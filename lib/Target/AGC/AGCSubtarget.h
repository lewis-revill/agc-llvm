//===-- AGCSubtarget.h - Define Subtarget for AGC ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the AGC specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_AGCSUBTARGET_H
#define LLVM_LIB_TARGET_AGC_AGCSUBTARGET_H

#include "AGCFrameLowering.h"
#include "AGCInstrInfo.h"
#include "AGCISelLowering.h"
#include "AGCRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "AGCGenSubtargetInfo.inc"

namespace llvm {
class StringRef;
class TargetMachine;
class Triple;

class AGCSubtarget : public AGCGenSubtargetInfo {
  virtual void anchor();

  // AGCFrameLowering FrameLowering;
  AGCInstrInfo InstrInfo;
  AGCRegisterInfo RegInfo;
  AGCFrameLowering FLInfo;
  AGCTargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;

public:
  // Initializes the data members to match that of the specified triple.
  AGCSubtarget(const Triple &TT, const std::string &CPU, const std::string &FS,
               const TargetMachine &TM);

  // Parses features string setting specified subtarget options. The
  // definition of this function is auto-generated by tablegen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  const AGCInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const AGCRegisterInfo *getRegisterInfo() const override { return &RegInfo; }
  const AGCFrameLowering *getFrameLowering() const override { return &FLInfo; }
  const AGCTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_AGC_AGCSUBTARGET_H
