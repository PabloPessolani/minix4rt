.sect .text
.extern	__mrt_RTstop
.define	_mrt_RTstop

.align 2
_mrt_RTstop:
	jmp	__mrt_RTstop

