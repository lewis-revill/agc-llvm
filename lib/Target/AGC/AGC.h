//===-- AGC.h - Top-level interface for AGC ---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// AGC back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_AGC_H
#define LLVM_LIB_TARGET_AGC_AGC_H

namespace llvm {
class AGCTargetMachine;
class FunctionPass;
class MachineInstr;
class MachineOperand;
class MCInst;
class MCOperand;
class PassRegistry;

bool LowerAGCMachineOperandToMCOperand(const MachineOperand &MO,
                                       MCOperand &MCOp);
void LowerAGCMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI);

FunctionPass *createAGCISelDag(AGCTargetMachine &TM);

FunctionPass *createAGCExpandPseudoPass();
void initializeAGCExpandPseudoPass(PassRegistry &);
}

#endif // LLVM_LIB_TARGET_AGC_AGC_H
