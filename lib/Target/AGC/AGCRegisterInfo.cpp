//===-- AGCRegisterInfo.cpp - AGC Register Information ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the AGC implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "AGCRegisterInfo.h"
#include "AGCFrameLowering.h"
#include "AGCSubtarget.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_REGINFO_MC_DESC
#define GET_REGINFO_TARGET_DESC
#include "AGCGenRegisterInfo.inc"

using namespace llvm;

AGCRegisterInfo::AGCRegisterInfo(unsigned HwMode)
    : AGCGenRegisterInfo(AGC::R2, /*DwarfFlavour=*/0, /*EHFlavor=*/0, /*PC=*/0,
                         HwMode) {}

const MCPhysReg *
AGCRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

BitVector AGCRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  return Reserved;
}

void AGCRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  report_fatal_error("Subroutines not supported yet");
}

unsigned AGCRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return ~0u;
}