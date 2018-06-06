/******************************************************************************/
/* 					mrttest12e.c						*/
/* Test the mrt_getsemid() System Call to get Semaphore ID				*/
/* Usage:												*/
/* 	mrttest12e											*/
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
	int rcode, semid;
	mrt_semattr_t sattr;


	rcode = mrt_getsemid("SEM_TEST");
	if( rcode < 0)
		{
		printf("mrt_getsemid: rcode=%5d.\n", rcode);
		exit(1);
		}
	else
		printf("Semaphore ID = %d\n", rcode);

	semid = rcode;
	/*---------------------- GET SEMAPHORE ATTRIBUTES -----------------------*/

	rcode = mrt_getsemattr(semid, &sattr);
	if( rcode != 0)
		{
		printf("mrt_getsemattr: rcode=%5d.\n", rcode);
		exit(1);
		}

	printf("Semaphore Value 	= %d\n", sattr.value);
	printf("Semaphore Flags 	= %X\n", sattr.flags);
	printf("Semaphore Priority 	= %X\n", sattr.priority);
	printf("Semaphore Name  	= %s\n", sattr.name);
}
