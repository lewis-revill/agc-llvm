//===- AGCMCELFStreamer.h - AGC subclass of MCElfStreamer -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Defines AGC specific behaviour when emitting to ELF.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCMCELFSTREAMER_H
#define LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCMCELFSTREAMER_H

#include "llvm/MC/MCELFStreamer.h"
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCObjectWriter;

class AGCMCELFStreamer : public MCELFStreamer {
public:
  AGCMCELFStreamer(MCContext &Context, std::unique_ptr<MCAsmBackend> TAB,
                   std::unique_ptr<MCObjectWriter> OW,
                   std::unique_ptr<MCCodeEmitter> Emitter);

  void EmitInstruction(const MCInst &Inst, const MCSubtargetInfo &STI,
                       bool) override;
};
} // namespace llvm

#endif
