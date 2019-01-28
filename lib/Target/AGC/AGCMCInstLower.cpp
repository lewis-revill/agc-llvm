//===-- AGCMCInstLower.cpp - Convert an AGC MachineInstr to an MCInst -------=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower AGC MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "AGC.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

bool llvm::LowerAGCMachineOperandToMCOperand(const MachineOperand &MO,
                                             MCOperand &MCOp) {
  switch (MO.getType()) {
  default:
    report_fatal_error("LowerAGCMachineInstrToMCInst: unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit())
      return false;
    // FIXME: Map registers to memory locations.
    MCOp = MCOperand::createImm(MO.getReg() - 1);
    break;
  case MachineOperand::MO_Immediate:
    MCOp = MCOperand::createImm(MO.getImm());
    break;
  }
  return true;
}

void llvm::LowerAGCMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI) {

  OutMI.setOpcode(MI->getOpcode());

  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp;
    if (LowerAGCMachineOperandToMCOperand(MO, MCOp))
      OutMI.addOperand(MCOp);
  }
}
