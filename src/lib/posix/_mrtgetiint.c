/*===========================================================================*
 *				mrt_getiint				     				*
 * Input:												* 	
 * 		irq: number 0 <= irq <=15							*
 *		iint: IRQ internal fields descriptor filled by the syscall 		*
 * Output:												*
 *		iint: IRQ internal fields descriptor filled by the syscall		*
 *		return OK ( 0 )									*
 * 		E_MRT_BADIRQ  (-2001): for an invalid irq number			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getiint _mrt_getiint
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getiint(irq, iint)
int irq;
mrt_irqint_t *iint;
{
  message m;
	
	m.m1_i2  = irq;
	m.m1_p1  = (char *)iint;
	m.m1_i1  = MRT_GETIINT;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
