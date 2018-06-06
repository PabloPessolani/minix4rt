/*===========================================================================*
 *				mrt_getpint			  	  				*
 * Gets internal variables of a process descriptor					*
 * Input:												* 	
 * 			pid_t pid;			PID of the process to change        *
 * Output:												*
 *                mrt_pint_t *procint; process internals data structure	  	*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *				EINVAL	: if the destination data structure if	*
 *						 out of memory segment				*
 *				E_BAD_PROC	: if the process ID is not valid		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getpint _mrt_getpint
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getpint(pid, procint)
pid_t pid;
mrt_pint_t *procint;
{
  message m;

	m.m1_i2  = (int) pid;
	m.m1_p1  = (char * ) procint;

	m.m1_i1  = MRT_GETPINT;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);

}
