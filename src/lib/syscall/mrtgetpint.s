.sect .text
.extern	__mrt_getpint
.define	_mrt_getpint

.align 2
_mrt_getpint:
	jmp	__mrt_getpint

