//===-- AGCMCAsmInfo.cpp - AGC Asm properties -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the definitions of the AGCMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "AGCMCAsmInfo.h"

using namespace llvm;

void AGCMCAsmInfo::anchor() {}

AGCMCAsmInfo::AGCMCAsmInfo(const Triple &TT) {
  CodePointerSize = 2;
  MinInstAlignment = 2;
  // Set to 4 due to handling of an extracode producing another 2 bytes.
  MaxInstLength = 4;

  IsLittleEndian = false;

  CommentString = "#";
}
