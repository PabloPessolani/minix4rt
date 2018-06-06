.sect .text
.extern	__mrt_sleep
.define	_mrt_sleep

.align 2
_mrt_sleep:
	jmp	__mrt_sleep
