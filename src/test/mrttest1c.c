/******************************************************************************/
/* 					mrttest1c.c							*/
/* Test the mrt_getiint() System Call to get Interrupt Descrip. Internal Data	*/
/* Usage:												*/
/* 	mrttest1c											*/
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
	mrt_irqint_t iint;

	printf("IRQ NUMBER HARMONIC VTIMER FLAGS\n");

	for( irq = 0; irq < 33; irq++)
		{
		rcode = mrt_getiint(irq, &iint);
		if( rcode != 0)
			printf("irq=%2d rcode=%5d. The test is OK if irq=32 rcode=-2001 \n"
				,irq ,rcode);
		else
			printf("%3d %7d %7d %7d %5X\n",
				irq,
				iint.irq,
				iint.harmonic,
				iint.vtimer,
				iint.flags);
		}
}
