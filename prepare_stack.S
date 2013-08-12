#define __SFR_OFFSET 0x00
#include <avr/io.h>
#include <avr/common.h>

/* prepare_stack
 *
 * Prepares the address space of a thread for its first context switch
 * (that will actually start the thread's execution).
 * First, the address of thread_exit is stored on the stack so that 
 * when the thread returns it exits nicely.
 * Second, the address of the thread's base function is stored on
 * the stack, so when the context switch occurs it jumps into the
 * correct function.
 * Finally, 33 dummy values are pushed onto the stack, so that the
 * context switch can use them to restore the 32 general purpose 
 * registers and the SREG.
 */
.text
.global prepare_stack
.type prepare_stack,@function
prepare_stack:

push r16
push r17

;r24:25 contains the start address of the function.
;This address will go on the top of this thread's stack,
;followed by its initial register values, which should all
;be zeroes. This way, a context switch can be used to start the thread.

;r22:23 contains the address of the top of the stack

;r20:21 contains the address of the function that can be called when
;the base function of the thread tries to return

;save the SP
in r16, SPL
in r17, SPH

;move the stack pointer to the stack for the new thread
out SPL, r22
out SPH, r23

;store the thread exit address
;This only works because the compiler uses r24:25 for the first
;function argument, and also for the return address of functions.
;As such, returning to a given address with a value is the same
;as "calling" that address (as if it were a function (which it is
;in this case)) and passing that value as an argument. That is why
;return x; and thread_exit(x); are effectively the same.
push r20
push r21

;store the return address
push r24
push r25

clr r22

push r22 ; r0
push r22 ; SREG
push r22 ; r1
push r22 ; r2
push r22 ; r3
push r22 ; r4
push r22 ; r5
push r22 ; r6
push r22 ; r7
push r22 ; r8
push r22 ; r9
push r22 ; r10
push r22 ; r11
push r22 ; r12
push r22 ; r13
push r22 ; r14
push r22 ; r15
push r22 ; r16
push r22 ; r17
push r22 ; r18
push r22 ; r19
push r22 ; r20
push r22 ; r21
push r22 ; r22
push r22 ; r23
push r22 ; r24
push r22 ; r25
push r22 ; r26
push r22 ; r27
push r22 ; r28
push r22 ; r29
push r22 ; r30
push r22 ; r31


;we will return this stack pointer
in r24, SPL
in r25, SPH

;fix up the stack pointer
out SPL, r16
out SPH, r17

pop r17
pop r16

ret


.end prepare_stack
