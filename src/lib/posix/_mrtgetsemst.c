/*===========================================================================*
 *				mrt_getsemstat			     				*
 * Input:												* 	
 *		semID											*
 * Output:												*
 *          sstat = semaphore statistics data structure				*
 *		return  = 0 => OK									*
 *          return  < 1 => error 								*
 *			E_MRT_BADSEM    (-2026)	:The Semaphore does not exists	*
 *			E_MRT_FREESEM   (-2027)	:The Semaphore is free			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getsemstat _mrt_getsemstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getsemstat(semid, sstat)
int		semid;
mrt_semstat_t *sstat;
{
  message m;
	
	m.m1_i2  = semid;
	m.m1_p1  = (char *)sstat;
	m.m1_i1  = MRT_GETSEMSTAT;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
