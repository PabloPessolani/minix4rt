/*===========================================================================*
 *				mrt_setiattr			     				*
 * Input:												* 	
 * 		irq: number 0 <= irq <=15							*
 *		iattrs: IRQ descriptor 								*
 * Output:												*
 *		iattrs: IRQ descriptor 								*
 *		return 	OK ( 0 )								*
 *			E_MRT_BADIRQ  (-2001): for an invalid irq Number		*
 * 			E_MRT_BADTASK (-2002): for an invalid TASK Number		*
 * 			E_MRT_BADPRTY (-2003): for an invalid Priority			*
 * 			E_MRT_BADIRQT (-2004): for an invalid IRQ handler type	*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_setiattr _mrt_setiattr
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_setiattr(irq, iattrs)
int irq;
mrt_irqattr_t *iattrs;
{
  message m;
	
	m.m1_i2  = irq;
	m.m1_p1  = (char *)iattrs;

	m.m1_i1  = MRT_SETIATTR;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
