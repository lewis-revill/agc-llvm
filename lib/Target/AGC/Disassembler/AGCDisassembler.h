//===-- AGCDisassembler.h - Disassembler for AGC --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
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
