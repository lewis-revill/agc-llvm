# RUN: llvm-mc -triple agc -show-encoding < %s \
# RUN:     | FileCheck -check-prefixes=CHECK,CHECK-INST %s

# CHECK-INST: ca 0
# CHECK: encoding: [0x60,0x01]
CA   0
# CHECK-INST: ca K
# CHECK: encoding: [0x60,0x01]
CA   K
# CHECK-INST: ca 7777
# CHECK: encoding: [0x7f,0xff]
CA   7777

# CHECK-INST: extend
# CHECK-INST: dca 0
# CHECK: encoding: [0x00,0x0d,0x60,0x01]
EXTEND
DCA  0
# CHECK-INST: extend
# CHECK-INST: dca K
# CHECK: encoding: [0x00,0x0d,0x60,0x01]
EXTEND
DCA  K
# CHECK-INST: extend
# CHECK-INST: dca 7777
# CHECK: encoding: [0x00,0x0d,0x7f,0xff]
EXTEND
DCA  7777

# CHECK-INST: cs 0
# CHECK: encoding: [0x80,0x00]
CS   0
# CHECK-INST: cs K
# CHECK: encoding: [0x80,0x00]
CS   K
# CHECK-INST: cs 7777
# CHECK: encoding: [0x9f,0xfe]
CS   7777

# CHECK-INST: extend
# CHECK-INST: dcs 0
# CHECK: encoding: [0x00,0x0d,0x80,0x00]
EXTEND
DCS  0
# CHECK-INST: extend
# CHECK-INST: dcs K
# CHECK: encoding: [0x00,0x0d,0x80,0x00]
EXTEND
DCS  K
# CHECK-INST: extend
# CHECK-INST: dcs 7777
# CHECK: encoding: [0x00,0x0d,0x9f,0xfe]
EXTEND
DCS  7777

# CHECK-INST: ad 0
# CHECK: encoding: [0xc0,0x01]
AD   0
# CHECK-INST: ad K
# CHECK: encoding: [0xc0,0x01]
AD   K
# CHECK-INST: ad 7777
# CHECK: encoding: [0xdf,0xff]
AD   7777

# CHECK-INST: extend
# CHECK-INST: su 0
# CHECK: encoding: [0x00,0x0d,0xc0,0x01]
EXTEND
SU   0
# CHECK-INST: extend
# CHECK-INST: su K
# CHECK: encoding: [0x00,0x0d,0xc0,0x01]
EXTEND
SU   K
# CHECK-INST: extend
# CHECK-INST: su 1777
# CHECK: encoding: [0x00,0x0d,0xc7,0xff]
EXTEND
SU   1777

# CHECK-INST: das 0
# CHECK: encoding: [0x40,0x00]
DAS  0
# CHECK-INST: das K
# CHECK: encoding: [0x40,0x00]
DAS  K
# CHECK-INST: das 1777
# CHECK: encoding: [0x47,0xfe]
DAS  1777

# CHECK-INST: extend
# CHECK-INST: msu 0
# CHECK: encoding: [0x00,0x0d,0x40,0x00]
EXTEND
MSU  0
# CHECK-INST: extend
# CHECK-INST: msu K
# CHECK: encoding: [0x00,0x0d,0x40,0x00]
EXTEND
MSU  K
# CHECK-INST: extend
# CHECK-INST: msu 1777
# CHECK: encoding: [0x00,0x0d,0x47,0xfe]
EXTEND
MSU  1777

# CHECK-INST: mask 0
# CHECK: encoding: [0xe0,0x00]
MASK 0
# CHECK-INST: mask K
# CHECK: encoding: [0xe0,0x00]
MASK K
# CHECK-INST: mask 7777
# CHECK: encoding: [0xff,0xfe]
MASK 7777

# CHECK-INST: extend
# CHECK-INST: mp 0
# CHECK: encoding: [0x00,0x0d,0xe0,0x00]
EXTEND
MP   0
# CHECK-INST: extend
# CHECK-INST: mp K
# CHECK: encoding: [0x00,0x0d,0xe0,0x00]
EXTEND
MP   K
# CHECK-INST: extend
# CHECK-INST: mp 7777
# CHECK: encoding: [0x00,0x0d,0xff,0xfe]
EXTEND
MP   7777

# CHECK-INST: extend
# CHECK-INST: dv 0
# CHECK: encoding: [0x00,0x0d,0x20,0x00]
EXTEND
DV   0
# CHECK-INST: extend
# CHECK-INST: dv K
# CHECK: encoding: [0x00,0x0d,0x20,0x00]
EXTEND
DV   K
# CHECK-INST: extend
# CHECK-INST: dv 1777
# CHECK: encoding: [0x00,0x0d,0x27,0xfe]
EXTEND
DV   1777

# CHECK-INST: incr 0
# CHECK: encoding: [0x50,0x01]
INCR 0
# CHECK-INST: incr K
# CHECK: encoding: [0x50,0x01]
INCR K
# CHECK-INST: incr 1777
# CHECK: encoding: [0x57,0xff]
INCR 1777

# CHECK-INST: extend
# CHECK-INST: aug 0
# CHECK: encoding: [0x00,0x0d,0x50,0x01]
EXTEND
AUG  0
# CHECK-INST: extend
# CHECK-INST: aug K
# CHECK: encoding: [0x00,0x0d,0x50,0x01]
EXTEND
AUG  K
# CHECK-INST: extend
# CHECK-INST: aug 1777
# CHECK: encoding: [0x00,0x0d,0x57,0xff]
EXTEND
AUG  1777

# CHECK-INST: ads 0
# CHECK: encoding: [0x58,0x00]
ADS  0
# CHECK-INST: ads K
# CHECK: encoding: [0x58,0x00]
ADS  K
# CHECK-INST: ads 1777
# CHECK: encoding: [0x5f,0xfe]
ADS  1777

# CHECK-INST: extend
# CHECK-INST: dim 0
# CHECK: encoding: [0x00,0x0d,0x58,0x00]
EXTEND
DIM  0
# CHECK-INST: extend
# CHECK-INST: dim K
# CHECK: encoding: [0x00,0x0d,0x58,0x00]
EXTEND
DIM  K
# CHECK-INST: extend
# CHECK-INST: dim 1777
# CHECK: encoding: [0x00,0x0d,0x5f,0xfe]
EXTEND
DIM  1777

# CHECK-INST: ts 0
# CHECK: encoding: [0xb0,0x00]
TS   0
# CHECK-INST: ts K
# CHECK: encoding: [0xb0,0x00]
TS   K
# CHECK-INST: ts 1777
# CHECK: encoding: [0xb7,0xfe]
TS   1777

# CHECK-INST: xch 0
# CHECK: encoding: [0xb8,0x01]
XCH  0
# CHECK-INST: xch K
# CHECK: encoding: [0xb8,0x01]
XCH  K
# CHECK-INST: xch 1777
# CHECK: encoding: [0xbf,0xff]
XCH  1777

# CHECK-INST: lxch 0
# CHECK: encoding: [0x48,0x01]
LXCH 0
# CHECK-INST: lxch K
# CHECK: encoding: [0x48,0x01]
LXCH K
# CHECK-INST: lxch 1777
# CHECK: encoding: [0x4f,0xff]
LXCH 1777

# CHECK-INST: extend
# CHECK-INST: qxch 0
# CHECK: encoding: [0x00,0x0d,0x48,0x01]
EXTEND
QXCH 0
# CHECK-INST: extend
# CHECK-INST: qxch K
# CHECK: encoding: [0x00,0x0d,0x48,0x01]
EXTEND
QXCH K
# CHECK-INST: extend
# CHECK-INST: qxch 1777
# CHECK: encoding: [0x00,0x0d,0x4f,0xff]
EXTEND
QXCH 1777

# CHECK-INST: dxch 0
# CHECK: encoding: [0xa8,0x00]
DXCH 0
# CHECK-INST: dxch K
# CHECK: encoding: [0xa8,0x00]
DXCH K
# CHECK-INST: dxch 1777
# CHECK: encoding: [0xaf,0xfe]
DXCH 1777

# CHECK-INST: tc 0
# CHECK: encoding: [0x00,0x01]
TC   0
# CHECK-INST: tc K
# CHECK: encoding: [0x00,0x01]
TC   K
# CHECK-INST: tc 7777
# CHECK: encoding: [0x1f,0xff]
TC   7777

# CHECK-INST: tcf 0
# CHECK: encoding: [0x20,0x00]
TCF  0
# CHECK-INST: tcf K
# CHECK: encoding: [0x20,0x00]
TCF  K
# CHECK-INST: tcf 7777
# CHECK: encoding: [0x3f,0xfe]
TCF  7777

# CHECK-INST: extend
# CHECK-INST: bzf 2000
# CHECK: encoding: [0x00,0x0d,0x28,0x01]
EXTEND
BZF  2000
# CHECK-INST: extend
# CHECK-INST: bzf 7777
# CHECK: encoding: [0x00,0x0d,0x3f,0xfe]
EXTEND
BZF  7777

# CHECK-INST: extend
# CHECK-INST: bzmf 2000
# CHECK: encoding: [0x00,0x0d,0xc8,0x00]
EXTEND
BZMF 2000
# CHECK-INST: extend
# CHECK-INST: bzmf 7777
# CHECK: encoding: [0x00,0x0d,0xdf,0xff]
EXTEND
BZMF 7777

# CHECK-INST: extend
# CHECK-INST: read 0
# CHECK: encoding: [0x00,0x0d,0x00,0x01]
EXTEND
READ 0
# CHECK-INST: extend
# CHECK-INST: read KC
# CHECK: encoding: [0x00,0x0d,0x00,0x01]
EXTEND
READ KC
# CHECK-INST: extend
# CHECK-INST: read 777
# CHECK: encoding: [0x00,0x0d,0x03,0xfe]
EXTEND
READ 777

# CHECK-INST: extend
# CHECK-INST: write 0
# CHECK: encoding: [0x00,0x0d,0x04,0x00]
EXTEND
WRITE 0
# CHECK-INST: extend
# CHECK-INST: write KC
# CHECK: encoding: [0x00,0x0d,0x04,0x00]
EXTEND
WRITE KC
# CHECK-INST: extend
# CHECK-INST: write 777
# CHECK: encoding: [0x00,0x0d,0x07,0xff]
EXTEND
WRITE 777

# CHECK-INST: extend
# CHECK-INST: rand 0
# CHECK: encoding: [0x00,0x0d,0x08,0x00]
EXTEND
RAND 0
# CHECK-INST: extend
# CHECK-INST: rand KC
# CHECK: encoding: [0x00,0x0d,0x08,0x00]
EXTEND
RAND KC
# CHECK-INST: extend
# CHECK-INST: rand 777
# CHECK: encoding: [0x00,0x0d,0x0b,0xff]
EXTEND
RAND 777

# CHECK-INST: extend
# CHECK-INST: wand 0
# CHECK: encoding: [0x00,0x0d,0x0c,0x01]
EXTEND
WAND 0
# CHECK-INST: extend
# CHECK-INST: wand KC
# CHECK: encoding: [0x00,0x0d,0x0c,0x01]
EXTEND
WAND KC
# CHECK-INST: extend
# CHECK-INST: wand 777
# CHECK: encoding: [0x00,0x0d,0x0f,0xfe]
EXTEND
WAND 777

# CHECK-INST: extend
# CHECK-INST: ror 0
# CHECK: encoding: [0x00,0x0d,0x10,0x00]
EXTEND
ROR  0
# CHECK-INST: extend
# CHECK-INST: ror KC
# CHECK: encoding: [0x00,0x0d,0x10,0x00]
EXTEND
ROR  KC
# CHECK-INST: extend
# CHECK-INST: ror 777
# CHECK: encoding: [0x00,0x0d,0x13,0xff]
EXTEND
ROR  777

# CHECK-INST: extend
# CHECK-INST: wor 0
# CHECK: encoding: [0x00,0x0d,0x14,0x01]
EXTEND
WOR  0
# CHECK-INST: extend
# CHECK-INST: wor KC
# CHECK: encoding: [0x00,0x0d,0x14,0x01]
EXTEND
WOR  KC
# CHECK-INST: extend
# CHECK-INST: wor 777
# CHECK: encoding: [0x00,0x0d,0x17,0xfe]
EXTEND
WOR  777

# CHECK-INST: extend
# CHECK-INST: rxor 0
# CHECK: encoding: [0x00,0x0d,0x18,0x01]
EXTEND
RXOR 0
# CHECK-INST: extend
# CHECK-INST: rxor KC
# CHECK: encoding: [0x00,0x0d,0x18,0x01]
EXTEND
RXOR KC
# CHECK-INST: extend
# CHECK-INST: rxor 777
# CHECK: encoding: [0x00,0x0d,0x1b,0xfe]
EXTEND
RXOR 777
