/*===========================================================================*
 *				mrt_arqst				     				*
 * Send a MT_AREQUEST message to the message Queue of a RT-process		*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * 			mrt_msg_t *m_ptr;		message pointer 				*
 * Output:												*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *				E_BAD_PROC	: if the process ID is not valid   		*
 * 				E_BAD_PTYPE : if the process is not RT			*
 * 				ENOSPC	: if the msgQ is Full				*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_arqst _mrt_arqst
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_arqst(mrtpid, m_ptr)
mrtpid_t 	mrtpid;
mrt_msg_t 	*m_ptr;
{
	mrt_rqst_t rqst;

	rqst.p_nr = mrtpid.p_nr;
	rqst.pid  = mrtpid.pid;
	rqst.msg = m_ptr;
	rqst.timeout = 0;

	return(_rtkrncall(MRTARQST,(void *)&rqst));
}
