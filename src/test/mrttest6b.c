/******************************************************************************/
/* 					mrttest6b.c							*/
/* Test the mrt_set2rt(), mrt_print() and _exit System Call 			*/
/* Usage:												*/
/* 	mrttest6b											*/
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
		printf("MRTPRINT mrt_set2rt rcode = %d\n",rcode);
		exit(1);
		}

	rcode = mrt_print("THAT'S ALL FOLKS!! Exit Without mrt_set2nrt");

}
