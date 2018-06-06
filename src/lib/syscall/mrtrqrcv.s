.sect .text
.extern	__mrt_rqrcv
.define	_mrt_rqrcv

.align 2
_mrt_rqrcv:
	jmp	__mrt_rqrcv

