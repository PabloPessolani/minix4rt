/*===========================================================================*
 *				mrt_getsemid			     				*
 * Input:												* 	
 *		semaphore name									*
 * Output:												*
 *		return  >= 0 => semphore ID							*
 *          return  < 1 => error 								*
 *			E_MRT_BADSEM    (-2026): The Semaphore does not exist 	*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getsemid _mrt_getsemid
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getsemid(sname)
char *sname;
{
  message m;
	
	m.m1_p1  = sname;
	m.m1_i1  = MRT_GETSEMID;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
