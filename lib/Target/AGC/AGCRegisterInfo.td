//===-- AGCRegisterInfo.td - AGC Register defs -------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  Declarations that describe the AGC register files
//
//===----------------------------------------------------------------------===//

let Namespace = "AGC" in {
// AGCReg - The base register for all internal AGC registers.
class AGCReg<bits<12> enc, string n> : Register<n> {
  let HWEncoding{11-0} = enc;
  let RegAltNameIndices = [NoRegAltName];
}

// AGCRegWithSubRegs - The base register for AGC registers composed of
// sub-registers.
class AGCRegWithSubRegs<bits<12> enc, string n, list<Register> subregs>
    : RegisterWithSubRegs<n, subregs> {
  let HWEncoding{11-0} = enc;
  let RegAltNameIndices = [NoRegAltName];
}

// Define upper/lower subreg indices.
def subreg_upper : SubRegIndex<16>;
def subreg_lower : SubRegIndex<16>;
} // Namespace = "AGC"

// Note that registers are defined by not only their descriptive name but also
// their register number/memory mapped address in octal. For general purpose and
// IO channel memory locations that are simply treated as registers, this is the
// only way to refer to them. For special purpose memory mapped locations,
// including the internal registers defined here, these can also be referenced
// by their aliases. The only way to achieve this functionality is to implement
// it in the parser & printer, so we don't actually specify the register numbers
//  or all special purpose locations in the register description.

// Note that all values in registers are treated as 15 bit ones complement,
// unless documented otherwise, and the instruction using the register knows
// about how to handle the register.

//===----------------------------------------------------------------------===//
// Internal registers used during operation of the CPU
//===----------------------------------------------------------------------===//

// Accumulator register. Used to store the result of operations done by the CPU.
// Consists of 16 bits due to the addition of an overflow/underflow bit.
def R0 : AGCReg<0, "A">, DwarfRegNum<[0]>;

// Lower accumulator register. Used to store the lower half of the result of
// operations done by the CPU when these operations are 'double precision'. Only
// 15 bits used in contrast to the standard accumulator.
def R1 : AGCReg<1, "L">, DwarfRegNum<[1]>;

// Return address register. Used to store the address that a procedure should
// return to. The AGC has no physical stack and so this register is the only
// available option for calls, implying recursion is not well supported.
// Consists of 16 bits due to the addition of an overflow/underflow bit.
def R2 : AGCReg<2, "Q">, DwarfRegNum<[2]>;

// Erasable memory bank select register. Used to switch the contents present in
// the memory locations 0x300 to 0x3FF between the 8 erasable memory banks. Only
// 3 bits of this register are needed and are arranged as follows:
//
// X000 0EEE 0000 0000
//
// The both banks select register aliases these bits.
def R3 : AGCReg<3, "EB">, DwarfRegNum<[3]>;

// Fixed memory bank select register. Used to switch the contents present in the
// memory locations 0x400 to 0x7FF between the 36 fixed memory banks. Only 5
// bits of this register are needed and are arranged as follows:
//
// XFFF FF00 0000 0000
//
// The both banks select register aliases these bits
def R4 : AGCReg<4, "FB">, DwarfRegNum<[4]>;

// Program counter. Used to store 12 bits of the address of the next
// instruction. The full address depends on the contents of the memory bank
// select registers to access the full range. The bits are arranged as follows:
//
// X000 AAAA AAAA AAAA
def R5 : AGCReg<5, "Z">, DwarfRegNum<[5]>;

// Both banks select register. Aliases both the erasable memory bank select and
// the fixed memory bank select registers. The bits are arranged as follows:
//
// XFFF FF00 0000 0EEE
def R6 : AGCReg<6, "BB">, DwarfRegNum<[6]>;

// Fixed zeroes register. This register has a fixed contents of all zeroes.
def R7 : AGCReg<7, "R7">, DwarfRegNum<[7]>;

// Double accumulator register. Composed of both the upper and lower accumulator
// registers. The effective value in this register is the concatenation of the
// lower 14 bits of both registers, with the sign bits in each register set to
// match each other. The overflow bit in the upper accumulator is set
// accordingly.
let SubRegIndices = [subreg_upper, subreg_lower] in
def RD0 : AGCRegWithSubRegs<0, "AL", [R0, R1]>, DwarfRegNum<[8]>;

//===----------------------------------------------------------------------===//
// AGC memory location registers
//===----------------------------------------------------------------------===//

foreach Index = 8-4095 in {
  def R#Index : AGCReg<Index, "R"#Index>, DwarfRegNum<[-1]>;
}

foreach Index = 1-4094 in {
  let SubRegIndices = [subreg_upper, subreg_lower] in
  def RD#Index : AGCRegWithSubRegs<Index, "RD"#Index,
                                   [!cast<AGCReg>("R"#Index),
                                    !cast<AGCReg>("R"#!add(Index, 1))]>,
                 DwarfRegNum<[-1]>;
}

//===----------------------------------------------------------------------===//
// AGC internal register class
//===----------------------------------------------------------------------===//

// Define the accumulator as its own register class.
def Acc : RegisterClass<"AGC", [i16], 1, (add R0)>;

// Define the lower accumulator as its own register class.
def AccLower : RegisterClass<"AGC", [i16], 1, (add R1)>;

// Define the upper/lower accumulator pair as its own register class.
def AccPair : RegisterClass<"AGC", [i32], 1, (add RD0)>;

// Define the return address register as its own register class.
def RA : RegisterClass<"AGC", [i16], 1, (add R2)>;

// Define a register class that maps directly to all memory locations.
def MM12 : RegisterClass<"AGC", [i16], 4096, (add
    (sequence "R%u", 0, 4095)
)>;

// Define a register class that maps directly to all memory pairs.
def MMD12 : RegisterClass<"AGC", [i32], 4095, (add
    (sequence "RD%u", 0, 4094)
)>;

// Define a register class that maps directly to erasable memory locations.
def MM10 : RegisterClass<"AGC", [i16], 1024, (add
    (sequence "R%u", 0, 1023)
)>;

// Define a register class that maps directly to erasable memory pairs.
def MMD10 : RegisterClass<"AGC", [i32], 1023, (add
    (sequence "RD%u", 0, 1022)
)>;
