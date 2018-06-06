/*===========================================================================*
 *				mrt_getsemattr			     				*
 * Input:												* 	
 *		semID											*
 * Output:												*
 *          sattr = semaphore attribute data structure				*
 *		return  = 0 => OK									*
 *          return  < 1 => error 								*
 *			E_MRT_BADSEM    (-2026)	:The Semaphore does not exists	*
 *			E_MRT_FREESEM   (-2027)	:The Semaphore is free			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getsemattr _mrt_getsemattr
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getsemattr(semid, sattr)
int		semid;
mrt_semattr_t *sattr;
{
  message m;
	
	m.m1_i2  = semid;
	m.m1_p1  = (char *)sattr;
	m.m1_i1  = MRT_GETSEMATTR;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
