.sect .text
.extern	__mrt_RTstart
.define	_mrt_RTstart

.align 2
_mrt_RTstart:
	jmp	__mrt_RTstart

