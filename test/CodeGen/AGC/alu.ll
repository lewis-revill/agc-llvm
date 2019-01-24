; RUN: llc -mtriple=agc -verify-machineinstrs < %s | FileCheck %s

; Check that basic ALU operations can be lowered to AGC instructions.

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
