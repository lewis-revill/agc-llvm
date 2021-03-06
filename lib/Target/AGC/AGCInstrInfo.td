//===-- AGCInstrInfo.td - Target Description for AGC -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  Declarations that describe the AGC instruction set
//
//===----------------------------------------------------------------------===//

include "AGCInstrFormats.td"

//===----------------------------------------------------------------------===//
// AGC specific DAG nodes.
//===----------------------------------------------------------------------===//

def RetFlag : SDNode<"AGCISD::RET_FLAG", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// Operand and SDNode transformation definitions
//===----------------------------------------------------------------------===//

class AGCAsmOperand<string N> : AsmOperandClass {
  let Name = N;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

// A 12 bit address into general purpose memory, encompassing erasable and
// read-only memory locations.
def mem12 : Operand<i16>, ImmLeaf<i16, [{return isUInt<12>(Imm);}]> {
  let ParserMatchClass = AGCAsmOperand<"Mem12">;
  let DecoderMethod = "decodeMem12Operand";
}

// A 10 bit address into general purpose memory, encompassing only erasable
// memory locations.
def mem10 : Operand<i16>, ImmLeaf<i16, [{return isUInt<10>(Imm);}]> {
  let ParserMatchClass = AGCAsmOperand<"Mem10">;
  let DecoderMethod = "decodeMem10Operand";
}

// A 9 bit address into IO channel memory.
def io9 : Operand<i16>, ImmLeaf<i16, [{return isUInt<9>(Imm);}]> {
  let ParserMatchClass = AGCAsmOperand<"IO9">;
  let DecoderMethod = "decodeIO9Operand";
}

// A 12 bit address to instruction locations.
def address12 : Operand<i16>, ImmLeaf<i16, [{return isUInt<12>(Imm);}]> {
  let ParserMatchClass = AGCAsmOperand<"Address12">;
  let DecoderMethod = "decodeAddress12Operand";
}

// A 6 bit immediate representing a bank number.
def bank6 : Operand<i16>, ImmLeaf<i16, [{return Imm < 36;}]> {
  let ParserMatchClass = AGCAsmOperand<"Bank6">;
}

// A 2 bit immediate representing a block number.
def block2 : Operand<i16>,
             ImmLeaf<i16, [{return isUInt<2>(Imm) && Imm != 1;}]> {
  let ParserMatchClass = AGCAsmOperand<"Block2">;
}

// A 15 bit immediate representing an entire data word.
def imm15 : Operand<i16>, ImmLeaf<i16, [{return isUInt<15>(Imm);}]> {
  let ParserMatchClass = AGCAsmOperand<"Imm15">;
}

// A 16 bit immediate expanded in codegen.
def imm16 : Operand<i16>;

//===----------------------------------------------------------------------===//
// Instruction class templates
//===----------------------------------------------------------------------===//

// Base instruction for defining instructions which read in from a general
// purpose memory location.
class ALUReadInst<bits<3> opcode, dag outs, dag ins, string opcodestr,
                  list<dag> pattern = []>
    : AGCInst12<opcode, outs, ins, opcodestr, pattern> {
  let hasSideEffects = 0;
  let mayLoad = 0;
  let mayStore = 0;
}

// Base instruction for defining instructions which write out to a general
// purpose memory location.
class ALUWriteInst<bits<5> opcode, dag outs, dag ins, string opcodestr,
                   list<dag> pattern = []>
    : AGCInst10<opcode, outs, ins, opcodestr, pattern> {
  let hasSideEffects = 0;
  let mayLoad = 0;
  let mayStore = 0;
}

// Base instruction for defining instructions which transfer control within the
// program.
class TCInst<bits<3> opcode, dag outs, dag ins, string opcodestr,
             list<dag> pattern = []>
    : AGCInst12<opcode, outs, ins, opcodestr, pattern> {
  let hasSideEffects = 0;
  let mayLoad = 0;
  let mayStore = 0;
  let isBranch = 1;
  let isBarrier = 1;
  let isTerminator = 1;
}

// Base instruction for defining conditional branch instructions.
class BranchInst<bits<3> opcode, dag outs, dag ins, string opcodestr,
             list<dag> pattern = []>
    : AGCInst12<opcode, outs, ins, opcodestr, pattern> {
  let hasSideEffects = 0;
  let mayLoad = 0;
  let mayStore = 0;
  let isBranch = 1;
  let isTerminator = 1;
}

// Base instruction for defining instructions which read in from an IO channel.
class IOReadInst<bits<6> opcode, dag outs, dag ins, string opcodestr,
                 list<dag> pattern = []>
    : AGCInst9<opcode, outs, ins, opcodestr, pattern> {
  let hasSideEffects = 0;
  let mayLoad = 1;
  let mayStore = 0;
  let isExtracode = 1;
  let DecoderNamespace = "Extracode";
}

// Base instruction for defining instructions which write out to an IO channel.
class IOWriteInst<bits<6> opcode, dag outs, dag ins, string opcodestr,
                  list<dag> pattern = []>
    : AGCInst9<opcode, outs, ins, opcodestr, pattern> {
  let hasSideEffects = 1;
  let mayLoad = 0;
  let mayStore = 1;
  let isExtracode = 1;
  let DecoderNamespace = "Extracode";
}

// Base instruction for defining instructions which read in from an IO channel
// and write out to the same IO channel.
class IOReadWriteInst<bits<6> opcode, dag outs, dag ins, string opcodestr,
                      list<dag> pattern = []>
    : AGCInst9<opcode, outs, ins, opcodestr, pattern> {
  let hasSideEffects = 1;
  let mayLoad = 1;
  let mayStore = 1;
  let isExtracode = 1;
  let DecoderNamespace = "Extracode";
}

//===----------------------------------------------------------------------===//
// EXTEND instruction
//===----------------------------------------------------------------------===//

// The extend instruction is used to indicate that the next instruction is to be
// interpreted as an extracode instruction, and to inform the assembler that the
// next instruction should appear as an extracode instruction.
def EXTEND : AGCInst0<0b000000000000110, "extend">;

//===----------------------------------------------------------------------===//
// Basic arithmetic instructions
//===----------------------------------------------------------------------===//

// Add value of general purpose memory location to accumulator. This instruction
// writes back the original value of the memory location, so memory locations
// which edit their contents are updated. Note that this occurs even though the
// operand is not necessarily erasable.
let Constraints = "$k = $k_wb", isAdd = 1 in
def AD : ALUReadInst<0b110, (outs Acc:$a_dst, mem12:$k_wb),
                     (ins Acc:$a, mem12:$k), "ad">;

// Double precision add value of accumulator to general purpose memory location.
// This instruction writes the sign of the output to the accumulator, and writes
// zero to the lower accumulator.
let Constraints = "$k = $k_dst", isAdd = 1 in
def DAS : ALUWriteInst<0b01000, (outs mem10:$k_dst, AccPair:$al_wb),
                       (ins mem10:$k, AccPair:$al), "das">;

// Subtract value of general purpose memory location from accumulator. This
// instruction writes back the original value of the memory location, so memory
// locations which edit their contents are updated.
let Constraints = "$k = $k_wb", isExtracode = 1,
    DecoderNamespace = "Extracode" in
def SU : ALUWriteInst<0b11000, (outs Acc:$a_dst, mem10:$k_wb),
                      (ins Acc:$a, mem10:$k), "su">;

// Modular subtract value of general purpose memory location from accumulator.
// This instruction writes back the original value of the memory location, so
// memory locations which edit their contents are updated.
let Constraints = "$k = $k_wb", isExtracode = 1,
    DecoderNamespace = "Extracode" in
def MSU : ALUWriteInst<0b01000, (outs Acc:$a_dst, mem10:$k_wb),
                       (ins Acc:$a, mem10:$k), "msu">;

// And value of general purpose memory location with value of accumulator into
// accumulator.
def MASK : ALUReadInst<0b111, (outs Acc:$a_dst), (ins Acc:$a, mem12:$k),
                       "mask">;

// Douple precision multiply value of general purpose memory location with
// accumulator. This instruction stores the result of the multiplication in both
// the accumulator and the lower accumulator.
let isExtracode = 1, DecoderNamespace = "Extracode" in
def MP : ALUReadInst<0b111, (outs AccPair:$a_dst), (ins Acc:$a, mem12:$k),
                     "mp">;

// Double precision divide value of the accumulator and lower accumulator pair
// by general purpose memory location. This instruction stores the result of the
// division in the accumulator and any remainder in the lower accumulator. Note
// that if the divisor is not larger than the value in the accumulator the
// result will not fit in a single memory location. Also note that if the
// divisor is larger than the value of the accumulator and lower accumulator
// pair then the result is completely undefined. Note that even though the
// operand is erasable, no editing or write back occurs.
let isExtracode = 1, DecoderNamespace = "Extracode" in
def DV : ALUWriteInst<0b00100, (outs Acc:$a_dst, AccLower:$l_dst),
                      (ins AccPair:$a, mem10:$k), "dv">;



// Increment general purpose memory location by 1.
let Constraints = "$k = $k_dst" in
def INCR : ALUWriteInst<0b01010, (outs mem10:$k_dst), (ins mem10:$k), "incr">;

// Add value of accumulator to general purpose memory location.
let Constraints = "$k = $k_dst" in
def ADS : ALUWriteInst<0b01011, (outs mem10:$k_dst), (ins mem10:$k, Acc:$a),
                       "ads">;

// 'Diminish' general purpose memory location by 1. This instruction either adds
// 1 to a negative value or subtracts 1 from a positive value. If the value is
// zero, it is unchanged.
let Constraints = "$k = $k_dst", isExtracode = 1,
    DecoderNamespace = "Extracode" in
def DIM : ALUWriteInst<0b01011, (outs mem10:$k_dst), (ins mem10:$k), "dim">;

// 'Augment' general purpose memory location by 1. This instruction either adds
// 1 to a positive value or subtracts 1 from a negative value. If the value is
// zero, it is augmented according to its sign.
let Constraints = "$k = $k_dst", isExtracode = 1,
    DecoderNamespace = "Extracode" in
def AUG : ALUWriteInst<0b01010, (outs mem10:$k_dst), (ins mem10:$k), "aug">;


// Copy value of general purpose memory location to accumulator. This
// instruction writes back the original value of the memory location, so memory
// locations which edit their contents are updated. Note that this occurs even
// though the operand is not necessarily erasable.
let Constraints = "$k = $k_wb" in
def CA : ALUReadInst<0b011, (outs Acc:$a_dst, mem12:$k_wb), (ins mem12:$k),
                     "ca">;

// Double precision copy general purpose memory location pair to accumulator and
// lower accumulator pair. This instruction writes back the original value of
// the memory location, so memory locations which edit their contents are
// updated. Note that this occurs even though the operand is not necessarily
// erasable.
let Constraints = "$k = $k_wb", isExtracode = 1,
    DecoderNamespace = "Extracode" in
def DCA : ALUReadInst<0b011, (outs AccPair:$a_dst, mem12:$k_wb),
                      (ins mem12:$k), "dca">;

// Copy inverted value of general purpose memory location to accumulator. This
// instruction writes back the original value of the memory location, so memory
// locations which edit their contents are updated. Note that this occurs even
// though the operand is not necessarily erasable.
let Constraints = "$k = $k_wb" in
def CS : ALUReadInst<0b100, (outs Acc:$a_dst, mem12:$k_wb), (ins mem12:$k),
                     "cs">;

// Double precision copy inverted value of general purpose memory location pair
// to accumulator pair. This instruction writes back the original value of the
// memory location, so memory locations which edit their contents are updated.
// Note that this occurs even though the operand is not necessarily erasable.
let Constraints = "$k = $k_wb", isExtracode = 1,
    DecoderNamespace = "Extracode" in
def DCS : ALUReadInst<0b100, (outs AccPair:$a_dst, mem12:$k_wb),
                      (ins mem12:$k), "dcs">;


// Transfer value of accumulator to general purpose memory location. This
// instruction corrects overflow of the accumulator value if the destination is
// not also a 16 bit memory location. If the accumulator contained overflow, the
// accumulator is set to +1 or -1 depending on the sign of the overflow.
def TS : ALUWriteInst<0b10110, (outs mem10:$k, Acc:$a_wb), (ins Acc:$a), "ts">;

// Exchange accumulator and general purpose memory location. This instruction
// corrects overflow of the accumulator value if the destination is not also a
// 16 bit memory location.
let Constraints = "$k = $k_dst" in
def XCH : ALUWriteInst<0b10111, (outs Acc:$a_dst, mem10:$k_dst),
                       (ins Acc:$a, mem10:$k), "xch">;

// Exchange lower accumulator and general purpose memory location. This
// instruction corrects overflow of the destination value if it is a 16 bit
// memory location.
let Constraints = "$k = $k_dst" in
def LXCH : ALUWriteInst<0b01001, (outs AccLower:$l_dst, mem10:$k_dst),
                        (ins AccLower:$l, mem10:$k), "lxch">;

// Exchange return address and general purpose memory location. This instruction
// corrects overflow of the return address value if the destination is not also
// a 16 bit memory location.
let Constraints = "$k = $k_dst", isExtracode = 1,
    DecoderNamespace = "Extracode" in
def QXCH : ALUWriteInst<0b01001, (outs RA:$q_dst, mem10:$k_dst),
                        (ins RA:$q, mem10:$k), "qxch">;

// Double precision exchange accumulator and lower accumulator pair and general
// purpose memory location pair. This instruction corrects only the overflow of
// the accumulator value if the destination is not also a 16 bit memory
// location.
let Constraints = "$k = $k_dst" in
def DXCH : ALUWriteInst<0b10101, (outs AccPair:$a_dst, mem10:$k_dst),
                        (ins AccPair:$a, mem10:$k), "dxch">;

//===----------------------------------------------------------------------===//
// Control flow instructions
//===----------------------------------------------------------------------===//

// Transfer control to address, saving the return address. Note that the address
// cannot reference the return address register, the erasable bank select
// register, the fixed bank select register or the both banks select register.
let Defs = [R5, R2], isCall = 1 in
def TC : TCInst<0b000, (outs), (ins address12:$k), "tc">;

// Transfer control to address, without saving the return address.
let Defs = [R5] in
def TCF : TCInst<0b001, (outs), (ins address12:$k), "tcf">;

// Branch if accumulator is zero to fixed address.
let Defs = [R5], isExtracode = 1, DecoderNamespace = "Extracode" in
def BZF : BranchInst<0b001, (outs), (ins Acc:$a, address12:$k), "bzf">;

// Branch if accumulator is zero or a negative number to fixed address.
let Defs = [R5], isExtracode = 1, DecoderNamespace = "Extracode" in
def BZMF : BranchInst<0b110, (outs), (ins Acc:$a, address12:$k), "bzmf">;

//===----------------------------------------------------------------------===//
// IO instructions
//===----------------------------------------------------------------------===//

// Read value of IO channel to accumulator.
def READ : IOReadInst<0b000000, (outs Acc:$a_dst), (ins io9:$kc), "read">;

// Write value of accumulator to IO channel.
def WRITE : IOWriteInst<0b000001, (outs io9:$kc), (ins Acc:$a), "write">;

// And value of IO channel to accumulator.
def RAND : IOReadInst<0b000010, (outs Acc:$a_dst), (ins Acc:$a, io9:$kc),
                      "rand">;

// And value of IO channel to accumulator and write resulting value back to IO
// channel.
let Constraints = "$kc = $kc_wb" in
def WAND : IOReadWriteInst<0b000011, (outs Acc:$a_dst, io9:$kc_wb),
                           (ins Acc:$a, io9:$kc), "wand">;

// Or value of IO channel to accumulator.
def ROR : IOReadInst<0b000100, (outs Acc:$a_dst), (ins Acc:$a, io9:$kc), "ror">;

// Or value of IO channel to accumulator and write resulting value back to IO
// channel.
let Constraints = "$kc = $kc_wb" in
def WOR : IOReadWriteInst<0b000101, (outs Acc:$a_dst, io9:$kc_wb),
                           (ins Acc:$a, io9:$kc), "wor">;

// Xor value of IO channel to accumulator.
def RXOR : IOReadInst<0b000110, (outs Acc:$a_dst), (ins Acc:$a, io9:$kc),
                      "rxor">;

//===----------------------------------------------------------------------===//
// Pseudo-instructions and codegen patterns
//===----------------------------------------------------------------------===//

/// Generic pattern classes

class PatAccMem12<SDPatternOperator OpNode, AGCInst12 Inst>
    : Pat<(OpNode Acc:$src1, MM12:$src2), (Inst Acc:$src1, mem12:$src2)>;
class PatAccMem10<SDPatternOperator OpNode, AGCInst10 Inst>
    : Pat<(OpNode Acc:$src1, MM10:$src2), (Inst Acc:$src1, mem10:$src2)>;

def : PatAccMem12<add, AD>;
def : PatAccMem10<sub, SU>;
def : PatAccMem12<and, MASK>;

let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def PseudoRET : Pseudo<(outs), (ins), "ret", "", [(RetFlag)]>,
                PseudoInstExpansion<(TC 2)>;

// Pseudo instruction used to expand a materialized constant.
def PseudoCONST : Pseudo<(outs address12:$k), (ins imm16:$value), "const",
                         "$value", []>;

//===----------------------------------------------------------------------===//
// Directives
//===----------------------------------------------------------------------===//

// Switch blocks for assembly output.
let isCodeGenOnly = 0, isAsmParserOnly = 1 in
def DirectiveBLOCK : Pseudo<(outs), (ins block2:$block), "block", "$block", []>;

// Switch banks for assembly output.
let isCodeGenOnly = 0 in
def DirectiveBANK : Pseudo<(outs), (ins bank6:$bank), "bank", "$bank", []>;

// Print out the number of instructions in a bank for debugging.
let isCodeGenOnly = 0, isAsmParserOnly = 1 in
def DirectiveBNKSUM : Pseudo<(outs), (ins bank6:$bank), "bnksum", "$bank", []>;

// Set a number of words to zero in the assembly output.
let isCodeGenOnly = 0, isAsmParserOnly = 1 in
def DirectiveERASE : Pseudo<(outs), (ins address12:$n), "erase", "$n", []>;

// Set the location to an explicit address in the assembly output.
let isCodeGenOnly = 0 in
def DirectiveSETLOC : Pseudo<(outs), (ins address12:$n), "setloc", "$n", []>;

// Emit an explicit immediate word to the assembly output.
let isCodeGenOnly = 0 in
def DirectiveOCT : Pseudo<(outs), (ins imm15:$value), "oct", "$value", []>;
