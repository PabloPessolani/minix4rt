/*===========================================================================*
 *				mrt_semfree				     				*
 * Input:												* 	
 *		semID											*
 * Output:												*
 *		return  = 0 => OK									*
 *          return  < 1 => error 								*
 *			E_MRT_BADSEM    (-2026)	:The Semaphore does not exists	*
 *			E_MRT_FREESEM   (-2027)	:The Semaphore is free			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_semfree _mrt_semfree
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_semfree(semid)
int		semid;
{
  message m;
	
	m.m1_i2  = semid;
	m.m1_i1  = MRT_SEMFREE;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
