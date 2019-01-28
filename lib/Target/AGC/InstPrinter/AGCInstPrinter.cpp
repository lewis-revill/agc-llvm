//===-- AGCInstPrinter.cpp - Convert AGC MCInst to asm syntax -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an AGC MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "AGCInstPrinter.h"
#include "AGCInstrInfo.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "AGCGenAsmWriter.inc"

void AGCInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                               StringRef Annot, const MCSubtargetInfo &STI) {
  unsigned Opcode = MI->getOpcode();

  // Prefix extracode instructions with an EXTEND instruction.
  if (MII.get(Opcode).TSFlags & AGCII::IsExtracode)
    O << "\t" << "extend" << "\n";

  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void AGCInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isImm()) {
    // All immediates must be printed in octal format.
    uint64_t Val = MO.getImm();

    uint64_t Pow = 1;
    uint64_t Octal = 0;
    for (unsigned i = 0; i < 20; i++) {
      if (Val == 0)
        break;
      Octal += (Val % 8) * Pow;
      Val /= 8;
      Pow *= 10;
    }
    O << Octal;
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI);
}
