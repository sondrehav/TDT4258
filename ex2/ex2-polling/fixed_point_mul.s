.syntax unified

// Implementation of Fixed point multiplication function.
// It is implemented in assembly so it can use umull.
// This function only use register r0 and r1,
// and does not call any other functions,
// so it does not need to push anything to and from the stack.

.section .text

.globl fixed_point_mul
fixed_point_mul:
	umull r0, r1, r0, r1 // 32 bit multiplication with 64 bit answer.
	
	// Move upper 16 bits of r0 to lower 16 bits of r0, and lower 16 bits of r1 to upper 16 bits of r0.
	// This has the effect of storing the result of umull, divided by 2^16, in r0.
	lsr r0, r0, #16
	lsl r1, r1, #16
	orr r0, r0, r1
	mov pc, lr
