.sect .text
.extern	__mrt_restart
.define	_mrt_restart

.align 2
_mrt_restart:
	jmp	__mrt_restart

