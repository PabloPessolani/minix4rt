! This is the Pascal run-time start-off routine.  It's job is to take the
! arguments as put on the stack by EXEC, and to parse them and set them up the
! way _m_a_i_n expects them.

.extern __m_a_i_n, _exit, prtso, hol0, __penvp
.extern begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
prtso:
	xor	bp, bp			! clear for backtrace of core files
	mov	bx, sp
	mov	ax, (bx)		! argc
	lea	dx, 2(bx)		! argv
	lea	cx, 4(bx)
	add	cx, ax
	add	cx, ax			! envp

	! Test if environ is in the initialized data area and is set to our
	! magic number.  If so then it is not redefined by the user.
	mov	bx, #_environ
	cmp	bx, #__edata		! within initialized data?
	jae	0f
	testb	bl, #1			! aligned?
	jnz	0f
	cmp	(bx), #0x5353		! is it our _environ?
	jne	0f
	mov	__penviron, bx		! _penviron = &environ;
0:	mov	bx, __penviron
	mov	(bx), cx		! *_penviron = envp;

	push	cx			! push envp
	push	dx			! push argv
	push	ax			! push argc

	mov	.ignmask, #56

	call	__m_a_i_n		! Run Pascal program

	push	ax			! push exit status
	call	__exit

	hlt				! force a trap if exit fails

.data
begdata:
hol0:	.data2 0, 0
	.data2 0, 0
__penviron:
	.data4	__penvp			! Pointer to environ, or hidden pointer

.bss
begbss:
	.comm	__penvp, 2		! Hidden environment vector
