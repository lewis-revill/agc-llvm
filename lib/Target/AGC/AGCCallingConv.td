//===-- AGCCallingConv.td - Calling Conventions for AGC ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the AGC architecture.
//
//===----------------------------------------------------------------------===//

// AGC C return-value convention.
def RetCC_AGC : CallingConv<[
  CCIfType<[i32], CCAssignToReg<[RD0]>>,
  CCIfType<[i16], CCAssignToReg<[R0, R1]>>
]>;

// AGC C Calling convention.
def CC_AGC : CallingConv<[
  // We can use the upper and lower accumulators to store arguments.
  CCIfType<[i32], CCAssignToReg<[RD0]>>,
  CCIfType<[i16], CCAssignToReg<[R0, R1]>>,
  // For now, use some fixed 'registers' to store more arguments. In future a
  // stack should be emulated.
  CCIfType<[i32], CCAssignToReg<[RD49]>>,
  CCIfType<[i16], CCAssignToReg<[R49, R50]>>
]>;

def CSR : CalleeSavedRegs<(add R2, R3, R4, R5, R6, R7)>;
