.sect .text
.extern	__mrt_getsval
.define	_mrt_getsval

.align 2
_mrt_getsval:
	jmp	__mrt_getsval
