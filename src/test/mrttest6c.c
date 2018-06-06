/******************************************************************************/
/* 					mrttest6c.c							*/
/* Test the mrt_set2rt(), mrt_print() and mrt_set2nrt(), mrt_sleep() Sys Calls*/
/* Usage:												*/
/* 	mrttest6c											*/
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
	mrt_pstat_t pstats;
	int rcode, pid;

	pattr.flags 	= MRT_P_REALTIME;
	pattr.baseprty 	= MRT_PRI03;
	pattr.period 	= 0;
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

	rcode = mrt_print("I am going to sleep...\n");
	if( rcode != 0 )
		{
		printf("mrt_print1 rcode = %d\n",rcode);
		exit(1);
		}
		
	rcode = mrt_sleep(200*10);

	if( rcode != 0 )
		{
		mrt_set2nrt();
		printf("mrt_sleep rcode = %d\n",rcode);
		exit(1);
		}

	rcode = mrt_print("I wake up...\n");
	if( rcode != 0 )
		{
		printf("mrt_print2 rcode = %d\n",rcode);
		exit(1);
		}

	rcode = mrt_set2nrt();
}
