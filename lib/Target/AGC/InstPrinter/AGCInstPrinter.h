//===-- AGCInstPrinter.h - Convert AGC MCInst to asm syntax -------*- C++ -*--//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an AGC MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_INSTPRINTER_AGCINSTPRINTER_H
#define LLVM_LIB_TARGET_AGC_INSTPRINTER_AGCINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {

class AGCInstPrinter : public MCInstPrinter {
public:
  AGCInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                 const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printInst(const MCInst *MI, raw_ostream &O, StringRef Annot,
                 const MCSubtargetInfo &STI) override;
  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O,
                    const char *Modifier = nullptr);

  // Autogenerated by TableGen.
  void printInstruction(const MCInst *MI, raw_ostream &O);
  static const char *getRegisterName(unsigned RegNo);
};
} // namespace llvm

#endif
