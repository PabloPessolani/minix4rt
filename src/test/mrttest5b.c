/******************************************************************************/
/* 					mrttest5b.c							*/
/* Test the mrt_getpint() System Call to get Process Internal Data		*/
/* Usage:												*/
/* 	mrttest5b											*/
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
	mrt_pint_t pint;
	int rcode;
	pid_t pid;

	if (argc == 2) 
		pid = atoi(argv[1]);
	else 
   		pid = getpid();

	rcode = mrt_getpint(pid, &pint);
	printf("mrt_getpint: pid = %d, rcode=%5d.\n", pid, rcode);
	if( rcode != 0) exit(1);
	printf("vt        = %d\n",pint.vt);
	printf("priority  = %d\n",pint.priority);
	printf("mqId      = %d\n",pint.mqID);
	printf("p_nr      = %d\n",pint.p_nr);
}
