.sect .text
.extern	__mrt_semdown
.define	_mrt_semdown

.align 2
_mrt_semdown:
	jmp	__mrt_semdown
