.sect .text
.extern	__mrt_semfree
.define	_mrt_semfree

.align 2
_mrt_semfree:
	jmp	__mrt_semfree
