//===-- AGCMCAsmInfo.h - AGC Asm Info --------------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the AGCMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCMCASMINFO_H
#define LLVM_LIB_TARGET_AGC_MCTARGETDESC_AGCMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class AGCMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit AGCMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
