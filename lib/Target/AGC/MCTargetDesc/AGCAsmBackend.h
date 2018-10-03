//===-- AGCAsmBackend.h - AGC Assembler Backend ----------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the AGCAsmBackend class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCASMBACKEND_H
#define LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCASMBACKEND_H

#include "llvm/MC/MCAsmBackend.h"

namespace llvm {
class AGCAsmBackend : public MCAsmBackend {
  uint8_t OSABI;

public:
  AGCAsmBackend(uint8_t OSABI) : MCAsmBackend(support::big), OSABI(OSABI) {}

  ~AGCAsmBackend() override {}

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  unsigned getMinimumNopSize() const override { return 2; }

  bool writeNopData(raw_ostream &OS, uint64_t Count) const override;

  unsigned getNumFixupKinds() const override { return 1; }

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  bool mayNeedRelaxation(const MCInst &Inst,
                         const MCSubtargetInfo &STI) const override {
    return false;
  }

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    return false;
  }

  void relaxInstruction(const MCInst &Inst, const MCSubtargetInfo &STI,
                        MCInst &Res) const override;
};
} // namespace llvm

#endif
