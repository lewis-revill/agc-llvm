//===-- RISCVTargetObjectFile.cpp - RISCV Object Info ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AGCTargetObjectFile.h"
#include "AGCTargetMachine.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCContext.h"

using namespace llvm;

void AGCELFTargetObjectFile::Initialize(MCContext &Ctx,
                                        const TargetMachine &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
  InitializeELF(TM.Options.UseInitArray);
  TextSection = (MCSection *)Ctx.getELFSection(
      "BLOCK2", ELF::SHT_PROGBITS, ELF::SHF_EXECINSTR | ELF::SHF_ALLOC);
}

MCSection *AGCELFTargetObjectFile::SelectSectionForGlobal(
    const GlobalObject *GO, SectionKind Kind, const TargetMachine &TM) const {
  if (!Kind.isText())
    return TargetLoweringObjectFileELF::SelectSectionForGlobal(GO, Kind, TM);

  const Function *F = dyn_cast<Function>(GO);
  if (!F || F->hasFnAttribute("implicit-section-name"))
    return TargetLoweringObjectFileELF::SelectSectionForGlobal(GO, Kind, TM);

  return TextSection;
}
