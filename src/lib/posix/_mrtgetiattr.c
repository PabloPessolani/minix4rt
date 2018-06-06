/*===========================================================================*
 *				mrt_getiattr			     				*
 * Input:												* 	
 * 		irq: number 0 <= irq <=15							*
 *		iattr: IRQ descriptor to be filled by the syscall			*
 * Output:												*
 *		iattr: IRQ descriptor filled by the syscall				*
 *		return OK ( 0 )									*
 * 		E_MRT_BADIRQ  (-2001): for an invalid irq number			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getiattr _mrt_getiattr
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getiattr(irq, iattr)
int irq;
mrt_irqattr_t *iattr;
{
  message m;
	
	m.m1_i2  = irq;
	m.m1_p1  = (char *)iattr;
	m.m1_i1  = MRT_GETIATTR;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
