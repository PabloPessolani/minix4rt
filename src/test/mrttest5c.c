/******************************************************************************/
/* 					mrttest5c.c							*/
/* Test the mrt_getiattr() System Call to clear Process statistics		*/
/* Usage:												*/
/* 	mrttest5C											*/
/******************************************************************************/
/* MRT test 5c: mrt_clrpstat */

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
	int rcode;
	pid_t pid;

	if (argc == 2) 
		pid = atoi(argv[1]);
	else 
   		pid = getpid();

	rcode = mrt_clrpstat(pid);
	printf("mrt_clrpstat: pid = %d, rcode=%5d.\n", pid, rcode);
	if( rcode != 0) exit(1);
	printf("Process %d statistics cleared\n",pid);
}
