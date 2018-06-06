/*===========================================================================*
 *				mrt_reply				     				*
 * Send a MT_REPLY message to the message Queue of a RT-process			*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * 			mrt_msg_t *m_ptr;		message pointer 				*
 * Output:												*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_reply _mrt_reply
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_reply(mrtpid, m_ptr)
mrtpid_t 	mrtpid;
mrt_msg_t 	*m_ptr;
{
	mrt_rply_t rply;

	rply.p_nr = mrtpid.p_nr;
	rply.pid  = mrtpid.pid;
	rply.msg = m_ptr;

	return(_rtkrncall(MRTREPLY,(void *)&rply));
}
