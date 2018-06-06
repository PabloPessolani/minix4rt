.sect .text
.extern	__mrt_rcv
.define	_mrt_rcv

.align 2
_mrt_rcv:
	jmp	__mrt_rcv
