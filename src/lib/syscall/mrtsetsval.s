.sect .text
.extern	__mrt_setsval
.define	_mrt_setsval

.align 2
_mrt_setsval:
	jmp	__mrt_setsval

