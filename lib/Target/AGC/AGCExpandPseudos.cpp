//===-- AGCExpandPseudos.cpp - Expand pseudo instructions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a pass that expands pseudo instructions into target
// instructions.
//
//===----------------------------------------------------------------------===//

#include "AGC.h"
#include "AGCInstrInfo.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

namespace {

class AGCExpandPseudo : public MachineFunctionPass {
public:
  const AGCInstrInfo *TII;
  static char ID;

  AGCExpandPseudo() : MachineFunctionPass(ID) {
    initializeAGCExpandPseudoPass(*PassRegistry::getPassRegistry());
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override {
    return "AGC pseudo instruction expansion pass";
  }

private:
  bool expandMBB(MachineBasicBlock &MBB);
  bool expandMI(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
                MachineBasicBlock::iterator &NextMBBI);
  bool expandPseudoCONST(MachineBasicBlock &MBB,
                         MachineBasicBlock::iterator MBBI,
                         MachineBasicBlock::iterator &NextMBBI);
};

char AGCExpandPseudo::ID = 0;

bool AGCExpandPseudo::runOnMachineFunction(MachineFunction &MF) {
  TII = static_cast<const AGCInstrInfo *>(MF.getSubtarget().getInstrInfo());
  bool Modified = false;
  for (auto &MBB : MF)
    Modified |= expandMBB(MBB);
  return Modified;
}

bool AGCExpandPseudo::expandMBB(MachineBasicBlock &MBB) {
  bool Modified = false;

  MachineBasicBlock::iterator MBBI = MBB.begin(), E = MBB.end();
  while (MBBI != E) {
    MachineBasicBlock::iterator NMBBI = std::next(MBBI);
    Modified |= expandMI(MBB, MBBI, NMBBI);
    MBBI = NMBBI;
  }

  return Modified;
}

bool AGCExpandPseudo::expandMI(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator MBBI,
                               MachineBasicBlock::iterator &NextMBBI) {
  switch (MBBI->getOpcode()) {
  default:
    break;
  case AGC::PseudoCONST:
    return expandPseudoCONST(MBB, MBBI, NextMBBI);
  }

  return false;
}

static bool toAGCConstant(int64_t &Constant) {
  // 14 non-sign bits are available for each word.
  constexpr int64_t MAX_VAL = (1 << 14) - 1;

  // One's complement range is symmetric.
  if (Constant > MAX_VAL)
    return false;
  if (Constant < -MAX_VAL)
    return false;

  // Negative values are shifted down by 1 compared to two's complement.
  if (Constant < 0)
    Constant = (Constant - 1) & 0x7FFF;

  return true;
}

static int64_t strtoi(StringRef Input) {
  size_t Idx;
  int64_t Result = std::stoi(Input.str(), &Idx, 10);
  if (Idx < Input.size() - 1)
    report_fatal_error("Could not convert string to integer");
  return Result;
}

// Retrieve the bank number from a given section name.
static int64_t toBankNumber(StringRef SectionName) {
  if (SectionName.startswith("BANK"))
    return strtoi(SectionName.drop_front(4));
  if (SectionName.startswith("BLOCK")) {
    int64_t BlockNumber = strtoi(SectionName.drop_front(5));
    // Only blocks 2 & 3 map to fixed banks.
    if (BlockNumber == 2 || BlockNumber == 3)
      return BlockNumber;
  }
  report_fatal_error("Could not determine bank from section");
}

bool AGCExpandPseudo::expandPseudoCONST(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MBBI,
                                        MachineBasicBlock::iterator &NextMBBI) {
  MachineFunction *MF = MBB.getParent();
  MachineInstr &MI = *MBBI;
  DebugLoc DL = MI.getDebugLoc();

  unsigned DestReg = MI.getOperand(0).getReg();
  int64_t Constant = MI.getOperand(1).getImm();

  // Get the constant as represented in 15-bit ones complement.
  if (!toAGCConstant(Constant))
    report_fatal_error("Could not represent constant");

  MachineBasicBlock *NewMBB = MF->CreateMachineBasicBlock(MBB.getBasicBlock());
  MF->insert(++MBB.getIterator(), NewMBB);

  // Emitting the following sequence will allow the constant to be materialized
  // in the correct location:
  //
  // SETLOC DestReg
  // OCT Constant
  BuildMI(NewMBB, DL, TII->get(AGC::DirectiveSETLOC)).addReg(DestReg);
  BuildMI(NewMBB, DL, TII->get(AGC::DirectiveOCT)).addImm(Constant);

  // Now we need to return to the original output location of the function.
  const TargetMachine &TM = MF->getTarget();
  StringRef SectionName =
      ((MCSectionELF *)TM.getObjFileLowering()->SectionForGlobal(
           &MF->getFunction(), TM))
          ->getSectionName();

  BuildMI(NewMBB, DL, TII->get(AGC::DirectiveBANK))
      .addImm(toBankNumber(SectionName));

  // Move all the rest of the instructions to NewMBB.
  NewMBB->splice(NewMBB->end(), &MBB, std::next(MBBI), MBB.end());
  // Update machine-CFG edges.
  NewMBB->transferSuccessorsAndUpdatePHIs(&MBB);
  // Make the original basic block fall-through to the new.
  MBB.addSuccessor(NewMBB);

  // Make sure live-ins are correctly attached to this new basic block.
  LivePhysRegs LiveRegs;
  computeAndAddLiveIns(LiveRegs, *NewMBB);

  NextMBBI = MBB.end();
  MI.eraseFromParent();
  return true;
}
} // namespace

INITIALIZE_PASS(AGCExpandPseudo, "agc-expand-pseudo",
                "AGC pseudo instruction expansion pass", false, false)

namespace llvm {

FunctionPass *createAGCExpandPseudoPass() { return new AGCExpandPseudo(); }

} // namespace llvm
