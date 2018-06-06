.sect .text
.extern	__mrt_arqst
.define	_mrt_arqst

.align 2
_mrt_arqst:
	jmp	__mrt_arqst
