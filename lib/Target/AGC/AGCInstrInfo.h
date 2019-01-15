//===-- AGCInstrInfo.h - AGC Instruction Information ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the AGC implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_AGCINSTRINFO_H
#define LLVM_LIB_TARGET_AGC_AGCINSTRINFO_H

#include "AGCRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "AGCGenInstrInfo.inc"

namespace llvm {

// AGCInst definitions.
namespace AGCII {
enum { IsExtracode = 1 << 3 };
}

class AGCInstrInfo : public AGCGenInstrInfo {
public:
  AGCInstrInfo();

  bool isCopyInstrImpl(const MachineInstr &MI,
                       const MachineOperand *&SourceOpNum,
                       const MachineOperand *&Destination) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, unsigned DestReg, unsigned SrcReg,
                   bool KillSrc) const override;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_AGC_AGCINSTRINFO_H
