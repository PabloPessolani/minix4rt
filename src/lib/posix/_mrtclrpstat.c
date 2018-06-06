/*===========================================================================*
 *				mrt_clrpstat			     				*
 * Clears process statistics									*
 * Input:												* 	
 * 			pid_t pid;			PID of the process 			*
 * Output:												*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_clrpstat _mrt_clrpstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_clrpstat(pid)
pid_t pid;
{
  message m;
	m.m1_i2  = (int) pid;
	m.m1_i1  = MRT_CLRPSTAT;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
