//===-- AGCFrameLowering.h - Define frame lowering for AGC -----*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
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
