/******************************************************************************/
/* 					mrttest3c.c							*/
/* Test the mrt_getsval() System Call to get System Values 				*/
/* Usage:												*/
/* 	mrttest3c											*/
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
	mrt_sysval_t val;
	int rcode;

	rcode = mrt_getsval(&val);
	printf("mrt_getsval: rcode=%d.\n\n", rcode);
	if( rcode != 0) exit(1);

	printf("flags		= x%-X\n",val.flags);
	printf("virtual_PIC   	= x%-X\n",val.virtual_PIC);
	printf("PIT_latency 	= %-d\n",val.PIT_latency);
	printf("PIT_latch      	= %-d\n",val.PIT_latch);
	printf("tickrate   	= %-d\n",val.tickrate);
	printf("harmonic   	= %-d\n",val.harmonic);
	printf("refresh   	= %-d\n",val.refresh);

}
