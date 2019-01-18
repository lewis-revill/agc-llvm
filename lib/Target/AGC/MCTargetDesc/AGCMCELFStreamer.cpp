//===- AGCMCELFStreamer.cpp - AGC subclass of MCELFStreamer ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements AGC specific ELF modifications for certain instructions.
//
//===----------------------------------------------------------------------===//

#include "AGCMCELFStreamer.h"
#include "AGCMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectWriter.h"

using namespace llvm;

AGCMCELFStreamer::AGCMCELFStreamer(MCContext &Context,
                                   std::unique_ptr<MCAsmBackend> TAB,
                                   std::unique_ptr<MCObjectWriter> OW,
                                   std::unique_ptr<MCCodeEmitter> Emitter)
    : MCELFStreamer(Context, std::move(TAB), std::move(OW),
                    std::move(Emitter)) {}

void AGCMCELFStreamer::EmitInstruction(const MCInst &Inst,
                                       const MCSubtargetInfo &STI, bool) {
  switch (Inst.getOpcode()) {
  default:
    break;
  case AGC::DirectiveBLOCK: {
    // The block directive informs the assembler to output to another block.
    // This is implemented by switching to a new section and letting the
    // linker handle it.

    // Block 0 is a special case as it maps to unswitched erasable memory, but
    // blocks 2 and 3 are aliased to existing bank sections.
    int64_t ImmOp = Inst.getOperand(0).getImm();
    StringRef SectionName = ImmOp == 0 ? "BLOCK0" : ("BANK" + itostr(ImmOp));
    MCSection *Section = ((MCSection *)getContext().getELFSection(
        SectionName, ELF::SHT_PROGBITS, ELF::SHF_EXECINSTR | ELF::SHF_ALLOC));
    SwitchSection(Section, nullptr);
    return;
  }
  case AGC::DirectiveBANK: {
    // The bank directive informs the assembler to output to another bank.
    // This is implemented by switching to a new section and letting the
    // linker handle it.
    StringRef SectionName = "BANK" + itostr(Inst.getOperand(0).getImm());
    MCSection *Section = ((MCSection *)getContext().getELFSection(
        SectionName, ELF::SHT_PROGBITS, ELF::SHF_EXECINSTR | ELF::SHF_ALLOC));
    SwitchSection(Section, nullptr);
    return;
  }
  case AGC::DirectiveBNKSUM: {
    // TODO: Output a debug statement showing how many words have been written
    // to the current section.
    return;
  }
  case AGC::DirectiveSETLOC: {
    // The setloc directive informs the assembler to output to an explicit
    // address. This is implemented by switching to a new section encoding the
    // address and letting the linker handle it.
    StringRef SectionName = "ADDR" + itostr(Inst.getOperand(0).getImm());
    MCSection *Section = ((MCSection *)getContext().getELFSection(
        SectionName, ELF::SHT_PROGBITS, ELF::SHF_EXECINSTR | ELF::SHF_ALLOC));
    SwitchSection(Section, nullptr);
    return;
  }
  }
  MCELFStreamer::EmitInstruction(Inst, STI);
}

namespace llvm {
MCStreamer *createAGCMCELFStreamer(const Triple &TT, MCContext &Context,
                                   std::unique_ptr<MCAsmBackend> &&MAB,
                                   std::unique_ptr<MCObjectWriter> &&OW,
                                   std::unique_ptr<MCCodeEmitter> &&CE,
                                   bool RelaxAll) {
  return new AGCMCELFStreamer(Context, std::move(MAB), std::move(OW),
                              std::move(CE));
}
} // namespace llvm
