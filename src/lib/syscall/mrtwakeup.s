.sect .text
.extern	__mrt_wakeup
.define	_mrt_wakeup

.align 2
_mrt_wakeup:
	jmp	__mrt_wakeup
