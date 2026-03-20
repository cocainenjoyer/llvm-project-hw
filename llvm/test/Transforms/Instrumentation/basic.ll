; RUN: opt -passes=instrument-func -S %s | FileCheck %s

; CHECK-LABEL: define i32 @foo()
define i32 @foo() {
entry:
  ; CHECK: call void @instrument_start()
  ; original instructions
  %add = add i32 1, 2
  ; CHECK: call void @instrument_end()
  ret i32 %add
}

; CHECK-LABEL: define i32 @bar()
define i32 @bar() {
entry:
  ; CHECK: call void @instrument_start()
  %mul = mul i32 3, 4
  ; CHECK: call void @instrument_end()
  ret i32 %mul
}

; CHECK-LABEL: declare void @instrument_start()
declare void @instrument_start()

; CHECK-LABEL: declare void @instrument_end()
declare void @instrument_end()
