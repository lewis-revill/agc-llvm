//===-- AGCMCAsmInfo.cpp - AGC Asm properties -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
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
