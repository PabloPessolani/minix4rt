/*===========================================================================*
 *				mrt_clrsstat			     				*
 * Clears system wide statistics								*
 * Input:												* 	
 * Output:												*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_clrsstat _mrt_clrsstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_clrsstat(void)
{
  message m;
	m.m1_i1  = MRT_CLRSSTAT;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
