/*===========================================================================*
 *				mrt_clrsemstat			     				*
 * Clears semaphore statistics								*
 * Input:												* 	
 * 			int semid;			Semaphore ID number 			*
 * Output:												*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_clrsemstat _mrt_clrsemstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_clrsemstat(semid)
int semid;
{
  message m;
	m.m1_i2  = (int) semid;
	m.m1_i1  = MRT_CLRSEMSTAT;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
