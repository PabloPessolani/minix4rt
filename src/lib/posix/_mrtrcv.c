/*===========================================================================*
 *				mrt_rcv				     				*
 * Receive a message from a specified RT-proc from its message Queue		*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * 			mrt_msg_t   *m_ptr;     msg buffer pointer			*
 * 			mrt_mhdr_t  *h_ptr;     msg header buffer pointer		*
 *			lcounter_t	timeout;    timeout in timer ticks			*
 * Output:												*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *				E_BAD_PROC	: if the process ID is not valid   		*
 * 				E_BAD_PTYPE : if the process is not RT			*
 * 				ENOSPC	: if the msgQ is Full				*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_rcv _mrt_rcv
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_rcv(mrtpid, m_ptr, h_ptr, timeout)
mrtpid_t 	mrtpid;
mrt_msg_t     *m_ptr;
mrt_mhdr_t *h_ptr;
lcounter_t	timeout;
{
	mrt_rcv_t rcv;
	
	rcv.p_nr = mrtpid.p_nr;
	rcv.pid  = mrtpid.pid;
	rcv.hdr  = h_ptr;
	rcv.msg  = m_ptr;
	rcv.timeout = timeout;

	return(_rtkrncall(MRTRCV,(void *)&rcv));
}
