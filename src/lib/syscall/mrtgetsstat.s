.sect .text
.extern	__mrt_getsstat
.define	_mrt_getsstat

.align 2
_mrt_getsstat:
	jmp	__mrt_getsstat
