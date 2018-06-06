.sect .text
.extern	__mrt_semalloc
.define	_mrt_semalloc

.align 2
_mrt_semalloc:
	jmp	__mrt_semalloc
