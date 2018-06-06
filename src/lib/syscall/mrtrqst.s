.sect .text
.extern	__mrt_rqst
.define	_mrt_rqst

.align 2
_mrt_rqst:
	jmp	__mrt_rqst
