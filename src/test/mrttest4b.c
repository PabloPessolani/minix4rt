/******************************************************************************/
/* 					mrttest4b.c							*/
/* Test the mrt_setpattr() System Call to set Process Descriptor Attributes	*/
/* Usage:												*/
/* 	mrttest4b											*/
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

	pattr.flags = (MRT_P_REALTIME | MRT_P_PERIODIC);
	pattr.baseprty = MRT_PRI03;
	pattr.period = 10;
	pattr.limit = 222;
	pattr.deadline = 11;
	pattr.watchdog = -1;
	pattr.mq_size = 3;
	pattr.mq_flags= 0;

	rcode = mrt_setpattr(&pattr);
	if( rcode != 0)
		{
		printf("mrt_setpattr: rcode=%5d.\n", rcode);
		exit(1);
		}
	else
		printf("Process Attributes set\n");
	sleep(60);
}
