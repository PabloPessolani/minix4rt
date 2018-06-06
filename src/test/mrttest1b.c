/******************************************************************************/
/* 					mrttest1b.c							*/
/* Test the mrt_getistat() System Call to get Interrupt Descriptor Statistics	*/
/* Usage:												*/
/* 	mrttest1b											*/
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
	mrt_irqstat_t istat;

	printf("IRQ COUNT MAXSHWR MDL TSTAMP MAXLAT REENTER\n");

	for( irq = 0; irq < 33; irq++)
		{
		rcode = mrt_getistat(irq, &istat);
		if( rcode != 0)
			printf("irq=%2d rcode=%5d. The test is OK if irq=32 rcode=-2001 \n"
				,irq ,rcode);
		else
			printf("%3d %5d %7d %3d %6d %6d %7d\n",
				irq,
				istat.count,
				istat.maxshower,
				istat.mdl,
				istat.timestamp,
				istat.maxlat,
				istat.reenter);
		}
}
