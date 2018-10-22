//===-- AGCInstPrinter.cpp - Convert AGC MCInst to asm syntax -------------===//
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

#include "AGCInstPrinter.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "AGCGenAsmWriter.inc"

// Temporary - This should be defined in AGCInstrInfo.h
namespace AGCII {
enum { IsExtracode = 1 << 3 };
}

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
