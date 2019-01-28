//===-- AGCFrameLowering.h - Define frame lowering for AGC -----*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class implements AGC-specific bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_AGCFRAMELOWERING_H
#define LLVM_LIB_TARGET_AGC_AGCFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class AGCFrameLowering : public TargetFrameLowering {
public:
  explicit AGCFrameLowering() : TargetFrameLowering(StackGrowsUp,
                                                    /*StackAlignment=*/16,
                                                    /*LocalAreaOffset=*/0) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override { return false; }
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_AGC_AGCFRAMELOWERING_H
