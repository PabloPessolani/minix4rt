/******************************************************************************/
/* 					mrttest4.c							*/
/* Test the mrt_getpattr() System Call to get Process Descriptor Attributes	*/
/* Usage:												*/
/* 	mrttest4											*/
/******************************************************************************/

#include <minix/config.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdio.h>
#include <string.h>


#include <lib.h>
#include <minix/syslib.h>

_PROTOTYPE(int main, (int argc, char *argv []));

int main(argc, argv)
int argc;
char **argv;
{
	mrt_pattr_t pattr;
	int rcode;
	pid_t pid;

	if(argc != 2)
		{
		printf("format: mrttest4 <pid>\n");
		exit(1);
		}
	pid = atoi(argv[1]);
	rcode = mrt_getpattr(pid, &pattr);
	if( rcode != 0)
		{
		printf("mrt_getpattr: pid= %d rcode=%5d.\n", pid, rcode);
		exit(1);
		}

	printf("flags      = %X\n",pattr.flags);
	printf("baseprty   = %X\n",pattr.baseprty);
	printf("period     = %d\n",pattr.period);
	printf("limit      = %d\n",pattr.limit);
	printf("deadline   = %d\n",pattr.deadline);
	printf("watchdog   = %d\n",pattr.watchdog);
	printf("MQ size    = %d\n",pattr.mq_size);
	printf("MQ flags   = %X\n",pattr.mq_flags);
}
