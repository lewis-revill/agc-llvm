# RUN: not llvm-mc -triple agc < %s 2>&1 | FileCheck %s

CA   10000 # CHECK: error: memory address must be an immediate in the range [0, 4095] or a symbol
CA         # CHECK: error: too few operands for instruction
EXTEND
CA   7777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
DCA  10000 # CHECK: error: memory address must be an immediate in the range [0, 4095] or a symbol
EXTEND
DCA        # CHECK: error: too few operands for instruction
DCA  7777  # CHECK: error: extracode instruction should be prefixed with EXTEND


CS   10000 # CHECK: error: memory address must be an immediate in the range [0, 4095] or a symbol
CS         # CHECK: error: too few operands for instruction
EXTEND
CS   7777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
DCS  10000 # CHECK: error: memory address must be an immediate in the range [0, 4095] or a symbol
EXTEND
DCS        # CHECK: error: too few operands for instruction
DCS  7777  # CHECK: error: extracode instruction should be prefixed with EXTEND

AD   10000 # CHECK: error: memory address must be an immediate in the range [0, 4095] or a symbol
AD         # CHECK: error: too few operands for instruction
EXTEND
AD   7777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
SU   2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
EXTEND
SU         # CHECK: error: too few operands for instruction
SU   1777  # CHECK: error: extracode instruction should be prefixed with EXTEND

DAS  2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
DAS        # CHECK: error: too few operands for instruction
EXTEND
DAS  1777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
MSU  2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
EXTEND
MSU        # CHECK: error: too few operands for instruction
MSU  1777  # CHECK: error: extracode instruction should be prefixed with EXTEND

MASK 10000 # CHECK: error: memory address must be an immediate in the range [0, 4095] or a symbol
MASK       # CHECK: error: too few operands for instruction
EXTEND
MASK 7777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
MP   10000 # CHECK: error: memory address must be an immediate in the range [0, 4095] or a symbol
EXTEND
MP         # CHECK: error: too few operands for instruction
MP   7777  # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
DV   2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
EXTEND
DV         # CHECK: error: too few operands for instruction
DV   1777  # CHECK: error: extracode instruction should be prefixed with EXTEND

INCR 2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
INCR       # CHECK: error: too few operands for instruction
EXTEND
INCR 1777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
AUG  2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
EXTEND
AUG        # CHECK: error: too few operands for instruction
AUG  1777  # CHECK: error: extracode instruction should be prefixed with EXTEND

ADS  2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
ADS        # CHECK: error: too few operands for instruction
EXTEND
ADS  1777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
DIM  2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
EXTEND
DIM        # CHECK: error: too few operands for instruction
DIM  1777  # CHECK: error: extracode instruction should be prefixed with EXTEND

TS   2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
TS         # CHECK: error: too few operands for instruction
EXTEND
TS   1777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

XCH  2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
XCH        # CHECK: error: too few operands for instruction
EXTEND
XCH  1777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

LXCH 2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
LXCH       # CHECK: error: too few operands for instruction
EXTEND
LXCH 1777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
QXCH 2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
EXTEND
QXCH       # CHECK: error: too few operands for instruction
QXCH 1777  # CHECK: error: extracode instruction should be prefixed with EXTEND

DXCH 2000  # CHECK: error: memory address must be an immediate in the range [0, 1023] or a symbol
DXCH       # CHECK: error: too few operands for instruction
EXTEND
DXCH 1777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

TC   10000 # CHECK: error: address must be an immediate in the range [0, 4095] or a symbol
TC         # CHECK: error: too few operands for instruction
EXTEND
TC   7777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

TCF  10000 # CHECK: error: address must be an immediate in the range [0, 4095] or a symbol
TCF        # CHECK: error: too few operands for instruction
EXTEND
TCF  7777  # CHECK: error: instruction prefixed with EXTEND is not an extracode

EXTEND
BZF  10000 # CHECK: error: address must be an immediate in the range [0, 4095] or a symbol
EXTEND
BZF        # CHECK: error: too few operands for instruction
BZF  7777  # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
BZMF 10000 # CHECK: error: address must be an immediate in the range [0, 4095] or a symbol
EXTEND
BZMF       # CHECK: error: too few operands for instruction
BZMF 7777  # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
READ 1000  # CHECK: error: IO channel address must be an immediate in the range [0, 511] or a symbol
EXTEND
READ       # CHECK: error: too few operands for instruction
READ 777   # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
WRITE 1000 # CHECK: error: IO channel address must be an immediate in the range [0, 511] or a symbol
EXTEND
WRITE      # CHECK: error: too few operands for instruction
WRITE 777  # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
RAND 1000  # CHECK: error: IO channel address must be an immediate in the range [0, 511] or a symbol
EXTEND
RAND       # CHECK: error: too few operands for instruction
RAND 777   # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
WAND 1000  # CHECK: error: IO channel address must be an immediate in the range [0, 511] or a symbol
EXTEND
WAND       # CHECK: error: too few operands for instruction
WAND 777   # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
ROR  1000  # CHECK: error: IO channel address must be an immediate in the range [0, 511] or a symbol
EXTEND
ROR        # CHECK: error: too few operands for instruction
ROR  777   # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
WOR  1000  # CHECK: error: IO channel address must be an immediate in the range [0, 511] or a symbol
EXTEND
WOR        # CHECK: error: too few operands for instruction
WOR  777   # CHECK: error: extracode instruction should be prefixed with EXTEND

EXTEND
RXOR 1000  # CHECK: error: IO channel address must be an immediate in the range [0, 511] or a symbol
EXTEND
RXOR       # CHECK: error: too few operands for instruction
RXOR 777   # CHECK: error: extracode instruction should be prefixed with EXTEND

FAIL 777   # CHECK: error: unrecognized instruction mnemonic
