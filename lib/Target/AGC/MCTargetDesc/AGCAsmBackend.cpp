//===-- AGCAsmBackend.cpp - AGC Assembler Backend --------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the definition of the AGCAsmBackend class.
//
//===----------------------------------------------------------------------===//

#include "AGCAsmBackend.h"
#include "AGCMCCodeEmitter.h"
#include "AGCMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"

using namespace llvm;

std::unique_ptr<MCObjectTargetWriter>
AGCAsmBackend::createObjectTargetWriter() const {
  return createAGCELFObjectWriter(OSABI);
}

bool AGCAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count) const {
  // Can only write 2-byte nops. Should be an assert since getMinimumNopSize is
  // defined.
  if (Count % 2 != 0)
    return false;

  uint64_t NopCount = Count / 2;
  for (uint64_t i = 0; i < NopCount; i++)
    // Canonical nop on AGC is CA A (copy value of accumulator to accumulator).
    AGCMCCodeEmitter::emitBitsWithParity(OS, 0x3000);

  return true;
}

void AGCAsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                               const MCValue &Target,
                               MutableArrayRef<char> Data, uint64_t Value,
                               bool IsResolved,
                               const MCSubtargetInfo *STI) const {
  return;
}

void AGCAsmBackend::relaxInstruction(const MCInst &Inst,
                                     const MCSubtargetInfo &STI,
                                     MCInst &Res) const {
  llvm_unreachable("No relaxation implemented");
}

MCAsmBackend *llvm::createAGCAsmBackend(const Target &T,
                                        const MCSubtargetInfo &STI,
                                        const MCRegisterInfo &MRI,
                                        const MCTargetOptions &Options) {
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(Triple::UnknownOS);
  return new AGCAsmBackend(OSABI);
}
