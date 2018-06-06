/******************************************************************************/
/* 					mrttest5.c							*/
/* Test the mrt_getpstat() System Call to get Process Statistics			*/
/* Usage:												*/
/* 	mrttest5											*/
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
	mrt_pstat_t pstats;
	int rcode;
	pid_t pid;

	if (argc == 2) 
		pid = atoi(argv[1]);
	else 
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
