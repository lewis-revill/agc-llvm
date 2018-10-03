//===-- AGCELFObjectWriter.cpp - AGC ELF Writer --------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of the AGCMCELFObjectWriter class.
//
//===----------------------------------------------------------------------===//

#include "AGCMCELFObjectWriter.h"
#include "AGCMCTargetDesc.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"

using namespace llvm;

unsigned AGCELFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel) const {
  switch (Fixup.getKind()) {
  default:
    llvm_unreachable("Unimplemented fixup kind!");
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createAGCELFObjectWriter(uint8_t OSABI) {
  return llvm::make_unique<AGCELFObjectWriter>(OSABI);
}