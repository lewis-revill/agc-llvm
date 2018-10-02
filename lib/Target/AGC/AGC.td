//===-- AGC.td - AGC Target Description --------------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  Describe the AGC tablegen target
//
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register file, instruction descriptions
//===----------------------------------------------------------------------===//

include "AGCRegisterInfo.td"
include "AGCInstrInfo.td"

def AGCInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// AGC processors
//===----------------------------------------------------------------------===//

// Currently only block 2 support is planned.
def : ProcessorModel<"agc-block2", NoSchedModel, []>;

//===----------------------------------------------------------------------===//
// Define the AGC target
//===----------------------------------------------------------------------===//

def AGC : Target {
  let InstructionSet = AGCInstrInfo;
}
