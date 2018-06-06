/*===========================================================================*
 *				mrt_semalloc			     				*
 * Input:												* 	
 *		semaphore attributes								*
 * Output:												*
 *		return  >= 0 => semphore ID							*
 *          return  < 1 => error 								*
 *			E_MRT_NOSEM     (-2025): Not free Semaphore			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_semalloc _mrt_semalloc
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_semalloc(sattrs)
mrt_semattr_t *sattrs;
{
  message m;
	
	m.m1_p1  = (char *)sattrs;
	m.m1_i1  = MRT_SEMALLOC;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
