/*===========================================================================*
 *				mrt_rqst				     				*
 * Send a MT_REQUEST message to the message Queue of a RT-process			*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * 			mrt_msg_t *m_ptr;		message pointer 				*
 * 			lcounter_t timeout;	timeout in RT-ticks			*
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

PUBLIC int mrt_rqst(mrtpid, m_ptr, timeout)
mrtpid_t 	mrtpid;
mrt_msg_t 	*m_ptr;
lcounter_t	timeout;
{
	mrt_rqst_t rqst;

	rqst.p_nr = mrtpid.p_nr;
	rqst.pid  = mrtpid.pid;
	rqst.msg = m_ptr;
	rqst.timeout = timeout;

	return(_rtkrncall(MRTRQST,(void *)&rqst));
}
