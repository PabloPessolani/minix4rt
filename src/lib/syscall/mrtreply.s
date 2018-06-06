.sect .text
.extern	__mrt_reply
.define	_mrt_reply

.align 2
_mrt_reply:
	jmp	__mrt_reply
