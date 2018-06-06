/*===========================================================================*
 *				mrt_rqrcv				     				*
 * Send a MT_REQUEST message to the message Queue of a RT-process			*
 * and waits for the REPLY									*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * 			mrt_msg_t   *rqst;      rqst buffer pointer			*
 * 			mrt_msg_t   *rply;      reply buffer pointer			*
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

#define mrt_rqst _mrt_rqst
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_rqrcv(mrtpid, rqst, rply, h_ptr, timeout)
mrtpid_t 	mrtpid;
mrt_msg_t   *rqst;
mrt_msg_t   *rply;
mrt_mhdr_t  *h_ptr;
lcounter_t	timeout;
{
	mrt_ipc_t ipc;

	ipc.p_nr = mrtpid.p_nr;
	ipc.pid  = mrtpid.pid;
	ipc.msg  = rqst;
	ipc.msg2 = rply;
	ipc.hdr  = h_ptr;
	ipc.timeout = timeout;

	return(_rtkrncall(MRTRQRCV,(void *)&ipc));
}
