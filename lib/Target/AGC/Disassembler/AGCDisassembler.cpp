//===-- AGCDisassembler.cpp - Disassembler for AGC ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the AGCDisassembler class.
//
//===----------------------------------------------------------------------===//

#include "AGCDisassembler.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/MC/MCFixedLenDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "agc-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

static MCDisassembler *createAGCDisassembler(const Target &T,
                                             const MCSubtargetInfo &STI,
                                             MCContext &Ctx) {
  return new AGCDisassembler(STI, Ctx);
}

extern "C" void LLVMInitializeAGCDisassembler() {
  // Register the disassembler for the AGC target.
  TargetRegistry::RegisterMCDisassembler(getTheAGCTarget(),
                                         createAGCDisassembler);
}

static DecodeStatus decodeMem12Operand(MCInst &Inst, uint64_t Imm,
                                       int64_t Address, const void *Decoder) {
  assert(isUInt<12>(Imm) && "Invalid immediate");
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

static DecodeStatus decodeMem10Operand(MCInst &Inst, uint64_t Imm,
                                       int64_t Address, const void *Decoder) {
  assert(isUInt<10>(Imm) && "Invalid immediate");
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

static DecodeStatus decodeIO9Operand(MCInst &Inst, uint64_t Imm,
                                     int64_t Address, const void *Decoder) {
  assert(isUInt<9>(Imm) && "Invalid immediate");
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

static DecodeStatus decodeAddress12Operand(MCInst &Inst, uint64_t Imm,
                                           int64_t Address,
                                           const void *Decoder) {
  assert(isUInt<12>(Imm) && "Invalid immediate");
  Inst.addOperand(MCOperand::createImm(Imm));
  return MCDisassembler::Success;
}

static DecodeStatus DecodeAccRegisterClass(MCInst &Inst, uint64_t Insn,
                                           int64_t Address,
                                           const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(0UL));
  return MCDisassembler::Success;
}

static DecodeStatus DecodeAccLowerRegisterClass(MCInst &Inst, uint64_t Insn,
                                                int64_t Address,
                                                const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(1UL));
  return MCDisassembler::Success;
}

static DecodeStatus DecodeAccPairRegisterClass(MCInst &Inst, uint64_t Insn,
                                               int64_t Address,
                                               const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(0UL));
  return MCDisassembler::Success;
}

static DecodeStatus DecodeRARegisterClass(MCInst &Inst, uint64_t Insn,
                                          int64_t Address,
                                          const void *Decoder) {
  Inst.addOperand(MCOperand::createImm(2UL));
  return MCDisassembler::Success;
}

#include "AGCGenDisassemblerTables.inc"

DecodeStatus AGCDisassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                             ArrayRef<uint8_t> Bytes,
                                             uint64_t Address, raw_ostream &OS,
                                             raw_ostream &CS) const {
  uint16_t Instruction;
  DecodeStatus Result;

  // Try non-extracode instructions first.
  Instruction = support::endian::read16be(Bytes.data());
  // Mask off the parity bit.
  Instruction = (Instruction & 0xFFFE) >> 1;
  Result =
      decodeInstruction(DecoderTable16, MI, Instruction, Address, this, STI);

  if (MI.getOpcode() != AGC::EXTEND) {
    Size = 2;
    return Result;
  }

  // Try parsing the following instruction using the decoder table for
  // extracodes.
  Instruction = support::endian::read16be(Bytes.drop_front(2).data());
  // Mask off the parity bit of the second instruction.
  Instruction = (Instruction & 0xFFFE) >> 1;
  Result = decodeInstruction(DecoderTableExtracode16, MI, Instruction, Address,
                             this, STI);
  /*
    // An EXTEND followed by another EXTEND is valid, and the first should be
    // considered a standalone instruction.
    if (MI.getOpcode() == AGC::EXTEND) {
      Size = 2;
      return Result;
    }*/

  Size = 4;
  return Result;
}
