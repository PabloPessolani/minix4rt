/*===========================================================================*
 *				mrt_sign				     				*
 * Send a MT_SIGNAL message to the message Queue of a RT-process			*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * 			mrt_msg_t *m_ptr;		message pointer 				*
 * Output:												*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_sign _mrt_sign
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_sign(mrtpid, m_ptr)
mrtpid_t 	mrtpid;
mrt_msg_t 	*m_ptr;
{
	mrt_sign_t sign;

	sign.p_nr = mrtpid.p_nr;
	sign.pid  = mrtpid.pid;
	sign.msg  = m_ptr;

	return(_rtkrncall(MRTSIGN,(void *)&sign));
}
