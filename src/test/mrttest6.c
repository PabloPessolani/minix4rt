/******************************************************************************/
/* 					mrttest6.c							*/
/* Test the mrt_set2rt(), mrt_print() and mrt_set2nrt() System Call 		*/
/* Usage:												*/
/* 	mrttest6											*/
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

	rcode = mrt_print("THAT'S ALL FOLKS!!");

	if( rcode != 0 )
		{
		printf("mrt_print rcode = %d\n",rcode);
		exit(1);
		}

	rcode = mrt_set2nrt();
	pid = getpid();

	rcode = mrt_getpstat(pid, &pstats);
	printf("mrt_getpstat: pid = %d, rcode=%5d.\n", pid, rcode);
	if( rcode != 0) exit(1);

	printf("scheds     = %10d\n",pstats.scheds);
	printf("mdl        = %10d\n",pstats.mdl);
	printf("timestamp  = %10d\n",pstats.timestamp);
	printf("maxlat     = %10d\n",pstats.maxlat);
	printf("minlax     = %10d\n",pstats.minlax);
	printf("msgsent    = %10d\n",pstats.msgsent);
	printf("msgrcvd    = %10d\n",pstats.msgrcvd);
}
