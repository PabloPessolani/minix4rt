/*===========================================================================*
 *				mrt_wakeup				     				*
 * Wakeup a blocked RT-process								*
 * If the process is RT-READY, a message is sent to the mailbox of the        *
 * process watchdog 										*
 * Input:												* 	
 * 			mrtpid_t	mrtpid;	MRT PID number			      *
 * Output:												*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *				E_BAD_PROC	: if the process ID is not valid		*
 * 				E_MRT_UNSLEEP : The woken up process is RT-READY      *
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_wakeup _mrt_wakeup
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_wakeup(mrtpid)
mrtpid_t 	mrtpid;
{
	return(_rtkrncall(MRTWAKEUP,&mrtpid));
}
