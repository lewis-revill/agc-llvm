//===-- AGCInstrInfo.cpp - AGC Instruction Information ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the AGC implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "AGCInstrInfo.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

#define GET_INSTRINFO_MC_DESC
#define GET_INSTRINFO_CTOR_DTOR
#include "AGCGenInstrInfo.inc"

using namespace llvm;

AGCInstrInfo::AGCInstrInfo() : AGCGenInstrInfo() {}

bool AGCInstrInfo::isCopyInstrImpl(const MachineInstr &MI,
                                   const MachineOperand *&SourceOpNum,
                                   const MachineOperand *&Destination) const {
  switch (MI.getOpcode()) {
  default:
    break;
  case AGC::CA:
  case AGC::TS:
    return true;
  }
  return false;
}

void AGCInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator MI,
                               const DebugLoc &DL, unsigned DestReg,
                               unsigned SrcReg, bool KillSrc) const {
  if (SrcReg == DestReg)
    return;

  if (DestReg == AGC::R0) {
    assert(AGC::MM12RegClass.contains(SrcReg) && "Can only copy from memory");
    BuildMI(MBB, MI, DL, get(AGC::CA), DestReg)
        .addReg(SrcReg, RegState::Define)
        .addReg(SrcReg, getKillRegState(KillSrc));
    return;
  }

  assert(AGC::MM10RegClass.contains(DestReg) &&
         "Can only copy to erasable memory");
  if (SrcReg != AGC::R0) {
    // If the source is not the accumulator, first move it into the accumulator.
    assert(AGC::MM12RegClass.contains(SrcReg) && "Can only copy from memory");
    BuildMI(MBB, MI, DL, get(AGC::CA), DestReg)
        .addReg(SrcReg, RegState::Define)
        .addReg(SrcReg, getKillRegState(KillSrc));
    KillSrc = true;
  }

  assert(KillSrc && "Cannot copy accumulator without killing it");
  BuildMI(MBB, MI, DL, get(AGC::TS), DestReg)
      .addReg(SrcReg, RegState::Define)
      .addReg(SrcReg, getKillRegState(KillSrc));
}