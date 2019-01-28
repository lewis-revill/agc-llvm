//===-- AGCInstrFormats.td - AGC Instruction Formats ---------*- tablegen -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  Declarations that describe the AGC instruction formats. See
//  http://www.ibiblio.org/apollo/assembly_language_manual.html for a full
//  description of the AGC instruction set.
//
//===----------------------------------------------------------------------===//

// InstFormat - Used to keep track of the encoding used by the instruction to
// allow further analysis before emission of machine instructions.
class InstFormat<bits<3> value> {
  bits<3> Value = value;
}
def InstFormatPseudo : InstFormat<0>;
def InstFormat12     : InstFormat<1>;
def InstFormat10     : InstFormat<2>;
def InstFormat9      : InstFormat<3>;
def InstFormat0      : InstFormat<4>;

// AGCInst - The base instruction for the AGC instruction set.
class AGCInst<dag outs, dag ins, string opcodestr, string argstr,
              list<dag> pattern, InstFormat format> : Instruction {
  let Namespace = "AGC";

  // Inst - The encoding for this instruction. For AGC, instructions only use 15
  // bits and so the first bit should always be set to 0.
  field bits<16> Inst;
  let Inst{15} = 0;

  // isExtracode - Indicates whether this instruction is an extracode, IE an
  // instruction that must be preceded by an EXTEND instruction to be
  // interpreted correctly.
  bit isExtracode = 0;

  // SoftFail - Used by the disassembler to provide a way for instructions to
  // not match without killing the whole decode process. It is mainly used for
  // ARM, but Tablegen expects this field to exist or it fails to build the
  // decode table.
  field bits<16> SoftFail = 0;

  dag OutOperandList = outs;
  dag InOperandList = ins;

  let AsmString = opcodestr # "\t" # argstr;

  let Pattern = pattern;

  // Most of the AGC instructions are accumulator or special-purpose register
  // based operations, so include operands that have no encoding in the
  // instruction but must be present in the decoded MCInst. DecodeNullOps allows
  // tablegen to decode these operands to a default value.
  let DecodeNullOps = 1;

  // Store the instruction format in target specific flags for later use.
  let TSFlags{2-0} = format.Value;

  // Store the extracode status in target specific flags for later use.
  let TSFlags{3} = isExtracode;

  let Size = 2;
}

// Pseudo - Base instruction for any pseudo instructions.
class Pseudo<dag outs, dag ins, string opcodestr, string argstr,
             list<dag> pattern = []>
    : AGCInst<outs, ins, opcodestr, argstr, pattern, InstFormatPseudo> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// AGCInst12 - Base instruction for instructions which have a memory location
// operand that could index any section of memory, in conjunction with the use
// of memory bank selection. In reality, some instructions impose constraints on
// this address such that it would not be possible to index all sections.
class AGCInst12<bits<3> opcode, dag outs, dag ins, string opcodestr,
                list<dag> pattern>
    : AGCInst<outs, ins, opcodestr, "$k", pattern, InstFormat12> {
  bits<12> k;

  let Inst{14-12} = opcode;
  let Inst{11-0} = k;
}

// AGCInst10 - Base instruction for instructions which have a memory location
// operand that could index any section of erasable memory, in conjunction with
// the use of erasable memory bank selection. In reality, some instructions
// impose constraints on this address such that it would not be possible to
// index all erasable memory sections.
class AGCInst10<bits<5> opcode, dag outs, dag ins, string opcodestr,
                list<dag> pattern = []>
    : AGCInst<outs, ins, opcodestr, "$k", pattern, InstFormat10> {
  bits<10> k;

  let Inst{14-10} = opcode;
  let Inst{9-0} = k;
}

// AGCInst9 - Base instruction for instructions which have a memory location
// operand that could index any of the IO channels. The IO channel addresses
// overlap with erasable memory but IO instructions cannot access erasable
// memory and vice-versa.
class AGCInst9<bits<6> opcode, dag outs, dag ins, string opcodestr,
               list<dag> pattern = []>
    : AGCInst<outs, ins, opcodestr, "$kc", pattern, InstFormat9> {
  bits<9> kc;

  let Inst{14-9} = opcode;
  let Inst{8-0} = kc;
}

// AGCInst0 - Base instruction for instructions which have no location
// operand.
class AGCInst0<bits<15> opcode, string opcodestr, list<dag> pattern = []>
    : AGCInst<(outs), (ins), opcodestr, "", pattern, InstFormat0> {
  let Inst{14-0} = opcode;
}
