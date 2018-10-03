//===-- AGCMCCodeEmitter.cpp - Convert AGC MC code to machine code --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the AGCMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCMCCODEEMITTER_H
#define LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCMCCODEEMITTER_H

#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInstrInfo.h"

namespace llvm {
class MCInst;
class MCSubtargetInfo;
class raw_ostream;

class AGCMCCodeEmitter : public MCCodeEmitter {
  const MCInstrInfo &MCII;
  MCContext &Ctx;

public:
  /// Emit a 15 bit binary code to the given stream with an additional odd
  /// parity bit.
  static void emitBitsWithParity(raw_ostream &OS, uint16_t Bits);

  AGCMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx)
      : MCII(MCII), Ctx(Ctx) {}

  ~AGCMCCodeEmitter() override {}

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  /// TableGen'erated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// Return binary encoding of operand. If the machine operand requires
  /// relocation, record the relocation and return zero. Required by TableGen.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

private:
  AGCMCCodeEmitter(const AGCMCCodeEmitter &) = delete;
  void operator=(const AGCMCCodeEmitter &) = delete;
};
} // namespace llvm

#endif
