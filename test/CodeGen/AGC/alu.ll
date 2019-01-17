; RUN: llc -mtriple=agc -verify-machineinstrs < %s | FileCheck %s

; Check that basic ALU operations can be lowered to AGC instructions.

define i16 @addi(i16 %a) {
entry:
; CHECK-LABEL: addi:
; CHECK: setloc 1
; CHECK: oct 52
; CHECK: bank 2
; CHECK: ad 1
; CHECK: tc 2
  %0 = add i16 %a, 42
  ret i16 %0
}

define i16 @subi(i16 %a) {
entry:
; CHECK-LABEL: subi:
; CHECK: setloc 1
; CHECK: oct 77725
; CHECK: bank 2
; CHECK: ad 1
; CHECK: tc 2
  %0 = sub i16 %a, 42
  ret i16 %0
}

define i16 @andi(i16 %a) {
entry:
; CHECK-LABEL: andi:
; CHECK: setloc 1
; CHECK: oct 52
; CHECK: bank 2
; CHECK: mask 1
; CHECK: tc 2
  %0 = and i16 %a, 42
  ret i16 %0
}

define i16 @addi_explicitsection(i16 %a) #0 {
entry:
; CHECK-LABEL: addi_explicitsection:
; CHECK: setloc 1
; CHECK: oct 52
; CHECK: bank 20
; CHECK: ad 1
; CHECK: tc 2
  %0 = add i16 %a, 42
  ret i16 %0
}

define i16 @subi_explicitsection(i16 %a) #0 {
entry:
; CHECK-LABEL: subi_explicitsection:
; CHECK: setloc 1
; CHECK: oct 77725
; CHECK: bank 20
; CHECK: ad 1
; CHECK: tc 2
  %0 = sub i16 %a, 42
  ret i16 %0
}

define i16 @andi_explicitsection(i16 %a) #0 {
entry:
; CHECK-LABEL: andi_explicitsection:
; CHECK: setloc 1
; CHECK: oct 52
; CHECK: bank 20
; CHECK: mask 1
; CHECK: tc 2
  %0 = and i16 %a, 42
  ret i16 %0
}

define i16 @add(i16 %a, i16 %b) {
entry:
; CHECK-LABEL: add:
; CHECK: ad 1
; CHECK: tc 2
  %0 = add i16 %a, %b
  ret i16 %0
}

define i16 @sub(i16 %a, i16 %b) {
entry:
; CHECK-LABEL: sub:
; CHECK: extend
; CHECK: su 1
; CHECK: tc 2
  %0 = sub i16 %a, %b
  ret i16 %0
}

define i16 @and(i16 %a, i16 %b) {
entry:
; CHECK-LABEL: and:
; CHECK: mask 1
; CHECK: tc 2
  %0 = and i16 %a, %b
  ret i16 %0
}

define i32 @mul(i16 %a, i16 %b) {
entry:
; CHECK-LABEL: mul:
; CHECK: extend
; CHECK: mp 1
; CHECK: tc 2
  %0 = sext i16 %a to i32
  %1 = sext i16 %b to i32
  %2 = mul i32 %0, %1
  ret i32 %2
}

define i16 @div(i32 %a, i16 %b) {
entry:
; CHECK-LABEL: div:
; CHECK: dv 61
; CHECK: tc 2
  %0 = sext i16 %b to i32
  %1 = sdiv i32 %a, %0
  %2 = trunc i32 %1 to i16
  ret i16 %2
}

define i16 @rem(i32 %a, i16 %b) {
entry:
; CHECK-LABEL: rem:
; CHECK: dv 61
; CHECK: ca 1
; CHECK: tc 2
  %0 = sext i16 %b to i32
  %1 = srem i32 %a, %0
  %2 = trunc i32 %1 to i16
  ret i16 %2
}

attributes #0 = { "implicit-section-name"="BANK16" }
