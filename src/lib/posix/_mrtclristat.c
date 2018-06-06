/*===========================================================================*
 *				mrt_clristat			     				*
 * Clears IRQ descriptor statistics								*
 * Input:												* 	
 * 			int irqd;			IRQ descriptor number 			*
 * Output:												*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_clristat _mrt_clristat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_clristat(irqd)
int irqd;
{
  message m;
	m.m1_i2  = (int) irqd;
	m.m1_i1  = MRT_CLRISTAT;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
