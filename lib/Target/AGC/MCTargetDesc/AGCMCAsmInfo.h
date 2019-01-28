//===-- AGCMCAsmInfo.h - AGC Asm Info --------------------------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
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
