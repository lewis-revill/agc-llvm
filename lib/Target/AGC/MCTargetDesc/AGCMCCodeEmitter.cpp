//===-- AGCMCCodeEmitter.cpp - Convert AGC MC code to machine code --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the AGCMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "AGCMCCodeEmitter.h"
#include "AGCInstrInfo.h"
#include "AGCMCTargetDesc.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/EndianStream.h"

using namespace llvm;

#define DEBUG_TYPE "agc-mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

MCCodeEmitter *llvm::createAGCMCCodeEmitter(const MCInstrInfo &MCII,
                                            const MCRegisterInfo &MRI,
                                            MCContext &Ctx) {
  return new AGCMCCodeEmitter(MCII, Ctx);
}

// Determine the odd parity bit that applies to the given 15-bit instruction
// code.
static uint16_t getParityBitForEncoding(uint16_t Enc) {
  // Accumulate the correct parity bit for odd parity.
  uint16_t ParityBit = 1;
  for (int i = 0; i < 15; i++)
    ParityBit ^= ((Enc >> i) & 1);

  return ParityBit;
}

// Append an odd parity bit to a 15-bit binary code.
static uint16_t getEncodingWithParity(uint16_t Bits) {
  return (Bits << 1) | getParityBitForEncoding(Bits);
}

void AGCMCCodeEmitter::emitBitsWithParity(raw_ostream &OS, uint16_t Bits) {
  uint16_t Encoding = getEncodingWithParity(Bits);
  support::endian::write<uint16_t>(OS, Encoding, support::big);
}

void AGCMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  assert(Desc.getSize() == 2);

  // Handle extracode instructions.
  if (Desc.TSFlags & AGCII::IsExtracode) {
    // Prefix this instruction with an EXTEND instruction.
    emitBitsWithParity(OS, 0x0006);
    ++MCNumEmitted;
  }

  uint16_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  emitBitsWithParity(OS, Bits);
  ++MCNumEmitted;
}

unsigned AGCMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                             const MCOperand &MO,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  // TODO: Implement fixups to resolve SymbolRef operands.
  if (MO.isExpr() && MO.getExpr()->getKind() == MCExpr::SymbolRef)
    return 0;

  llvm_unreachable("Unhandled expression!");
  return 0;
}

// Get TableGen'erated function definitions.
#include "AGCGenMCCodeEmitter.inc"
