//===-- AGCMCTargetDesc.cpp - AGC Target Descriptions ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides AGC specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "AGCMCTargetDesc.h"
#include "AGCMCAsmInfo.h"
#include "InstPrinter/AGCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "AGCGenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "AGCGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "AGCGenSubtargetInfo.inc"

using namespace llvm;

static MCInstrInfo *createAGCMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitAGCMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createAGCMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitAGCMCRegisterInfo(X, AGC::R2);
  return X;
}

static MCAsmInfo *createAGCMCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT) {
  return new AGCMCAsmInfo(TT);
}

static MCInstPrinter *createAGCMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  return new AGCInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializeAGCTargetMC() {
  Target &T = getTheAGCTarget();
  TargetRegistry::RegisterMCAsmInfo(T, createAGCMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(T, createAGCMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(T, createAGCMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(T, createAGCMCSubtargetInfoImpl);
  TargetRegistry::RegisterELFStreamer(T, createAGCMCELFStreamer);
  TargetRegistry::RegisterMCCodeEmitter(T, createAGCMCCodeEmitter);
  TargetRegistry::RegisterMCAsmBackend(T, createAGCAsmBackend);
  TargetRegistry::RegisterMCInstPrinter(T, createAGCMCInstPrinter);
}
