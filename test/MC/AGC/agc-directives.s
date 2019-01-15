# RUN: llvm-mc -triple agc < %s | FileCheck %s

# CHECK: block 0
BLOCK 0
# CHECK: block 2
BLOCK 2
# CHECK: block 3
BLOCK 3

# CHECK: bank 0
BANK 0
# CHECK: bank 1
BANK 1
# CHECK: bank 2
BANK 2
# CHECK: bank 3
BANK 3
# CHECK: bank 4
BANK 4
# CHECK: bank 5
BANK 5
# CHECK: bank 6
BANK 6
# CHECK: bank 7
BANK 7
# CHECK: bank 10
BANK 10
# CHECK: bank 11
BANK 11
# CHECK: bank 12
BANK 12
# CHECK: bank 13
BANK 13
# CHECK: bank 14
BANK 14
# CHECK: bank 15
BANK 15
# CHECK: bank 16
BANK 16
# CHECK: bank 17
BANK 17
# CHECK: bank 20
BANK 20
# CHECK: bank 21
BANK 21
# CHECK: bank 22
BANK 22
# CHECK: bank 23
BANK 23
# CHECK: bank 24
BANK 24
# CHECK: bank 25
BANK 25
# CHECK: bank 26
BANK 26
# CHECK: bank 27
BANK 27
# CHECK: bank 30
BANK 30
# CHECK: bank 31
BANK 31
# CHECK: bank 32
BANK 32
# CHECK: bank 33
BANK 33
# CHECK: bank 34
BANK 34
# CHECK: bank 35
BANK 35
# CHECK: bank 36
BANK 36
# CHECK: bank 37
BANK 37
# CHECK: bank 40
BANK 40
# CHECK: bank 41
BANK 41
# CHECK: bank 42
BANK 42
# CHECK: bank 43
BANK 43

# CHECK: bnksum 0
BNKSUM 0
