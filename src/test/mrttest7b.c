/******************************************************************************/
/* 					mrttest7b.c							*/
/* Test a periodic process							 		*/
/* Usage:												*/
/* 	mrttest7b											*/
/******************************************************************************/

#include <minix/config.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include <minix/type.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#include <lib.h>
#include <minix/syslib.h>

_PROTOTYPE(int main, (int argc, char *argv []));

int main()
{
	mrt_pattr_t pattr;
	int rcode, pid,i;

	pattr.flags 	= MRT_P_REALTIME | MRT_P_PERIODIC;
	pattr.baseprty 	= MRT_PRI03;
	pattr.period 	= 500;
	pattr.limit 	= 0;
	pattr.deadline 	= 0;
	pattr.watchdog 	= -1;
	pattr.mq_size 	= 3;
	pattr.mq_flags	= 0;

	rcode = mrt_setpattr(&pattr);
	if( rcode != 0)
		{
		printf("mrt_setpattr: rcode=%5d.\n", rcode);
		exit(1);
		}
	else
		printf("Process Attributes set\n");


	rcode = mrt_set2rt();
	if( rcode != 0 )
		{
		printf("mrt_set2rt rcode = %d\n",rcode);
		exit(1);
		}

	rcode = mrt_print("Entering in a loop...\n");
	if( rcode != 0 )
		{
		printf("mrt_print1 rcode = %d\n",rcode);
		exit(1);
		}

	for( i = 0; i < 5; i++)
		{
		mrt_print("loop\n");
		rcode = mrt_sleep(MRT_FOREVER);
		}

	mrt_set2nrt();
	printf("exiting...\n");
}
