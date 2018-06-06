.sect .text
.extern	__mrt_getistat
.define	_mrt_getistat

.align 2
_mrt_getistat:
	jmp	__mrt_getistat
