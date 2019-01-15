//===-- AGCDisassembler.h - Disassembler for AGC --------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the AGCDisassembler class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_DISASSEMBLER_AGCDISASSEMBLER_H
#define LLVM_LIB_TARGET_AGC_DISASSEMBLER_AGCDISASSEMBLER_H

#include "llvm/MC/MCDisassembler/MCDisassembler.h"

namespace llvm {
class AGCDisassembler : public MCDisassembler {
public:
  AGCDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}

  ~AGCDisassembler() override = default;

  MCDisassembler::DecodeStatus
  getInstruction(MCInst &Instr, uint64_t &Size, ArrayRef<uint8_t> Bytes,
                 uint64_t Address, raw_ostream &VStream,
                 raw_ostream &CStream) const override;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_AGC_DISASSEMBLER_AGCDISASSEMBLER_H
