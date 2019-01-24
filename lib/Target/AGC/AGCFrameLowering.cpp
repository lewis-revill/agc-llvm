//===-- AGCFrameLowering.cpp - AGC Frame Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the AGC implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "AGCFrameLowering.h"

using namespace llvm;

void AGCFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void AGCFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}
