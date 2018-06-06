/*===========================================================================*
 *				mrt_getistat				     			*
 * Input:												* 	
 * 		irq: number 0 <= irq <=15							*
 *		istat: IRQ statistics descriptor to be filled by the syscall	*
 * Output:												*
 *		istat: IRQ statistics descriptor filled by the syscall		*
 *		return OK ( 0 )									*
 * 		E_MRT_BADIRQ  (-2001): for an invalid irq number			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getistat _mrt_getistat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getistat(irq, istat)
int irq;
mrt_irqstat_t *istat;
{
  message m;
	
	m.m1_i2  = irq;
	m.m1_p1  = (char *)istat;
	m.m1_i1  = MRT_GETISTAT;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
