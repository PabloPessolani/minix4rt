/*===========================================================================*
 *				mrt_clrmqstat			     				*
 * Clears Message Queue statistics								*
 * Input:												* 	
 * 			int mqid;			Message Queue ID number			*
 * Output:												*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_clrmqstat _mrt_clrmqstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_clrmqstat(mqid)
int mqid;
{
  message m;
	m.m1_i2  = (int) mqid;
	m.m1_i1  = MRT_CLRMQSTAT;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
