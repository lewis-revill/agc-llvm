//===-- AGCAsmPrinter.cpp - AGC LLVM assembly writer ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the AGC assembly language.
//
//===----------------------------------------------------------------------===//

#include "AGC.h"
#include "AGCAsmPrinter.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

// Simple pseudo-instructions have their lowering (with expansion to real
// instructions) auto-generated.
#include "AGCGenMCPseudoLowering.inc"

void AGCAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  // Do any auto-generated pseudo lowerings.
  if (emitPseudoExpansionLowering(*OutStreamer, MI))
    return;

  MCInst TmpInst;
  LowerAGCMachineInstrToMCInst(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" void LLVMInitializeAGCAsmPrinter() {
  RegisterAsmPrinter<AGCAsmPrinter> X(getTheAGCTarget());
}
