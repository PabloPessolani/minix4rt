/* This file contains routines for initializing the 8259 interrupt controller:
 *	get_irq_handler: address of handler for a given interrupt
 *	put_irq_handler: register an interrupt handler
 *	intr_init:	initialize the interrupt controller(s)
 */

#include "kernel.h"

#ifdef	MRT

#include <minix/com.h>
#include "proc.h"

#endif	/* MRT */


PUBLIC void test(void)
{
      MRT_si.bitmap = 0;
	MRT_sv.flags = 0;
}
