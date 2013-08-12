#define __SFR_OFFSET 0x00
#include <avr/io.h>
#include <avr/common.h>

.text
.global context_switch
.type context_switch,@function
context_switch:
; disable interrupts
cli

; store registers
push r0

; once we have a free register, copy the SREG onto the stack
in r0, SREG
push r0

;keep on storing registers
push r1
push r2
push r3
push r4
push r5
push r6
push r7
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15
push r16
push r17
push r18
push r19
push r20
push r21
push r22
push r23
push r24
push r25
push r26
push r27
push r28
push r29
push r30
push r31


; arguments are:
; r22:23 = &new thread
; r24:25 = &current thread

; store the current SP in the current thread
;the current thread is in X
mov XL, r24
mov XH, r25

;the first 2 bytes of the thread struct contain the SP
in r24, SPL
in r25, SPH

;update the sp of the current thread
st X+, r24
st X , r25


;get the SP of the new thread
;the new thread is in X
mov XL, r22
mov XH, r23

;the SP is in Y
ld YL, X+
ld YH, X

;adjust the actual SP
out SPL, YL
out SPH, YH

;...and now we are in the new thread

pop r31
pop r30
pop r29
pop r28
pop r27
pop r26
pop r25
pop r24
pop r23
pop r22
pop r21
pop r20
pop r19
pop r18
pop r17
pop r16
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop r7
pop r6
pop r5
pop r4
pop r3
pop r2
pop r1

;this location contains the SREG, so restore it
pop r0
out SREG, r0

;restore the actual r0
pop r0

; enable interrupts
sei

; "return" into a new thread
ret
.end context_switch

