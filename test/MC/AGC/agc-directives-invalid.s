# RUN: not llvm-mc -triple agc < %s 2>&1 | FileCheck %s

BANK 44 # CHECK: error: bank number must be an immediate in the range [0, 35]
BANK K # CHECK: error: bank number must be an immediate in the range [0, 35]

BLOCK 1 # CHECK: error: block number must be either 0, 2 or 3
BLOCK K # CHECK: error: block number must be either 0, 2 or 3

BNKSUM 44 # CHECK: error: bank number must be an immediate in the range [0, 35]
BNKSUM K # CHECK: error: bank number must be an immediate in the range [0, 35]
