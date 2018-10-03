//===-- AGCELFObjectWriter.h - AGC ELF Writer ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the AGCMCELFObjectWriter class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCELFOBJECTWRITER_H
#define LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCELFOBJECTWRITER_H

#include "llvm/MC/MCELFObjectWriter.h"

namespace llvm {
class MCContext;
class MCFixup;
class MCValue;

class AGCELFObjectWriter : public MCELFObjectTargetWriter {
public:
  AGCELFObjectWriter(uint8_t OSABI)
      : MCELFObjectTargetWriter(/*Is64Bit=*/false, OSABI, ELF::EM_AGC,
                                /*HasRelocationAddend=*/true) {}

  ~AGCELFObjectWriter() override {}

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
} // namespace llvm

#endif
