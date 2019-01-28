//===-- AGCTargetMachine.cpp - Define TargetMachine for AGC ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about AGC target spec.
//
//===----------------------------------------------------------------------===//

#include "AGC.h"
#include "AGCTargetMachine.h"
#include "AGCTargetObjectFile.h"
#include "MCTargetDesc/AGCMCTargetDesc.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
using namespace llvm;

extern "C" void LLVMInitializeAGCTarget() {
  RegisterTargetMachine<AGCTargetMachine> A(getTheAGCTarget());
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

AGCTargetMachine::AGCTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   Optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, "e-m:e-p:16:16-n16-S16", TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(make_unique<AGCELFTargetObjectFile>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

namespace {
class AGCPassConfig : public TargetPassConfig {
public:
  AGCPassConfig(AGCTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  AGCTargetMachine &getAGCTargetMachine() const {
    return getTM<AGCTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreEmitPass2() override;
};
}

bool AGCPassConfig::addInstSelector() {
  addPass(createAGCISelDag(getAGCTargetMachine()));

  return false;
}

void AGCPassConfig::addPreEmitPass2() { addPass(createAGCExpandPseudoPass()); }

TargetPassConfig *AGCTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new AGCPassConfig(*this, PM);
}
