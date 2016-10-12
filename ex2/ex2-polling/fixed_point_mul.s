.syntax unified

.section .text

.globl  fixed_point_mul
fixed_point_mul:
	umull r0, r1, r0, r1
	lsr r0, r0, #16
	lsl r1, r1, #16
	orr r0, r0, r1
	mov pc, lr
