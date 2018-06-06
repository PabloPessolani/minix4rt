/******************************************************************************/
/* 					mrttest1.c							*/
/* Test the mrt_getiattr() System Call to get Interrupt Descriptor Attributes	*/
/* Usage:												*/
/* 	mrttest1											*/
/******************************************************************************/

#include <minix/config.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
 
#include <lib.h>
#include <minix/syslib.h>

_PROTOTYPE(int main, (int argc, char *argv []));

int main(argc, argv)
int argc;
char *argv[];
{

	int irq, rcode;
	mrt_irqattr_t iattrs;

	printf("IRQ PERIOD TASK WATCHDOG PRIORITY IRQTYPE NAME\n");

	for( irq = 0; irq < (NR_IRQ_VECTORS + NR_IRQ_SOFT); irq++)
		{
		rcode = mrt_getiattr(irq, &iattrs);
		if( rcode != 0)
			printf("irq=%2d rcode=%5d. The test is OK if irq=32 rcode=-2001 \n"
				,irq ,rcode);
		else
			printf("%3d %6d %4d %8d %8d %7X %-16s\n",
				irq,
				iattrs.period,
				iattrs.task,
				iattrs.watchdog,
				iattrs.priority,
				iattrs.irqtype,
				iattrs.name);
		}
}
