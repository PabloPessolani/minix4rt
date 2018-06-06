.sect .text; .sect .rom; .sect .data; .sect .bss

! See ../h/com.h for C definitions
RTKCALL = 4

SYSVEC = 33

RTK_NR = 8
PARMPTR   = 12

!*========================================================================*
!                           _rtkcall                                      *
!*========================================================================*
! _rtkcall(), save ebp, but destroy eax and ecx.
.define __rtkcall
.sect .text
__rtkcall:
	push	ebp
	mov	ebp, esp
	push	ebx
	mov	eax, RTK_NR(ebp)	      ! ax = rtk_nr
	mov	ebx, PARMPTR(ebp)		! ebx = parm_ptr
	mov	ecx, RTKCALL		! RTKCALL
	int	SYSVEC			! trap to the kernel
	pop	ebx
	pop	ebp
	ret
