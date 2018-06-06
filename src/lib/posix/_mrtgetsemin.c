/*===========================================================================*
 *				mrt_getsemint			     				*
 * Input:												* 	
 *		semID											*
 * Output:												*
 *          sint = semaphore internal data structure					*
 *		return  = 0 => OK									*
 *          return  < 1 => error 								*
 *			E_MRT_BADSEM    (-2026)	:The Semaphore does not exists	*
 *			E_MRT_FREESEM   (-2027)	:The Semaphore is free			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getsemint _mrt_getsemint
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getsemint(semid, sint)
int		semid;
mrt_semint_t *sint;
{
  message m;
	
	m.m1_i2  = semid;
	m.m1_p1  = (char *)sint;
	m.m1_i1  = MRT_GETSEMINT;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
