/*===========================================================================*
 *				mrt_uprqst				     				*
 * Send a MT_UPRQST message to the message Queue of a RT-process			*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * 			mrt_msg_t *m_ptr;		message pointer 				*
 * 			int  		prty;		message priority				*
 * Output:												*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_uprqst _mrt_uprqst
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_uprqst(mrtpid, m_ptr, prty)
mrtpid_t 	mrtpid;
mrt_msg_t 	*m_ptr;
int 		prty;
{
	mrt_uprq_t uprq;

	uprq.p_nr = mrtpid.p_nr;
	uprq.pid  = mrtpid.pid;
	uprq.msg  = m_ptr;
	uprq.priority = prty;

	return(_rtkrncall(MRTUPRQST,(void *)&uprq));
}
