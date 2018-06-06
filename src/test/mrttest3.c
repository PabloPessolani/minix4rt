/******************************************************************************/
/* 					mrttest3.c							*/
/* Test the mrt_getsstat() System Call to get System Wide Statistics		*/
/* Usage:												*/
/* 	mrttest3											*/
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

int main()
{
	mrt_sysstat_t stats;
	int rcode;

	rcode = mrt_getsstat( &stats);
	printf("mrt_getsstat: rcode=%d.\n\n", rcode);
	if( rcode != 0) exit(1);

	printf("schedulings	= %-d\n",stats.scheds);
	printf("messages   	= %-d\n",stats.messages);
	printf("interrupts 	= %-d\n",stats.interrupts);
	printf("ticks      	= %-d:%-d\n",stats.highticks,stats.ticks);
	printf("idle last/max	= %-d/%-d\n",stats.idlelast,stats.idlemax);
}
