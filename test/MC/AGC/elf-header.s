# RUN: llvm-mc -filetype=obj -triple agc < %s | llvm-readobj -h \
# RUN:     | FileCheck -check-prefix=AGC %s

# AGC: Format: ELF32-agc
# AGC: Arch: agc
# AGC: AddressSize: 32bit
# AGC: ElfHeader {
# AGC:   Ident {
# AGC:     Magic: (7F 45 4C 46)
# AGC:     Class: 32-bit (0x1)
# AGC:     DataEncoding: BigEndian (0x2)
# AGC:     FileVersion: 1
# AGC:     OS/ABI: SystemV (0x0)
# AGC:     ABIVersion: 0
# AGC:   }
# AGC:   Type: Relocatable (0x1)
# AGC:   Machine: EM_AGC (0x7B1)
# AGC:   Version: 1
# AGC:   Flags [ (0x0)
# AGC:   ]
# AGC: }
