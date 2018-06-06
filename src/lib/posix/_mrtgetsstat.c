/*===========================================================================*
 *				mrt_getsstat				     				*
 * Input:												* 	
 *		counters: system wide counters data structure 				*
 * Output:												*
 *		counters: system wide counters data structure				*
 *		return 	OK ( 0 )								*
 *				EINVAL: for out of segmente data strcuture		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getsstat _mrt_getsstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getsstat(stats)
mrt_sysstat_t *stats;
{
  message m;
	
	m.m1_p1  = (char *)stats;

	m.m1_i1  = MRT_GETSSTAT;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
