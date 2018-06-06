/*===========================================================================*
 *				mrt_getpstat			     				*
 * Gets processing statistics of a process						*
 * Input:												* 	
 * 			pid_t pid;			PID of the process to change        *
 * Output:												*
 *                mrt_pstat_t *pstat; process statistics data structure  	*
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *				EINVAL	: if the destination data structure if	*
 *						 out of memory segment				*
 *				E_BAD_PROC	: if the process ID is not valid		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getpstat _mrt_getpstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getpstat(pid, pstat )
pid_t pid;
mrt_pstat_t *pstat;
{
  message m;

	m.m1_i2  = (int) pid;
	m.m1_p1  = (char * ) pstat;

	m.m1_i1  = MRT_GETPSTAT;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);

}
