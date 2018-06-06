.sect .text
.extern	__mrt_getiint
.define	_mrt_getiint

.align 2
_mrt_getiint:
	jmp	__mrt_getiint
