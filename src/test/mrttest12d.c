/******************************************************************************/
/* 					mrttest12d.c						*/
/* Test the mrt_semfree() System Call to free a Semaphore				*/
/* Usage:												*/
/* 	mrttest12d											*/
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
	mrt_semattr_t sattr;
	mrt_semstat_t sstat;
	int rcode, semid;

	/*---------------------- ALLOC A SEMAPHORE  --------------------------*/
	sattr.flags = SEM_PRTYORDER | SEM_PRTYINHERIT;
	sattr.value = 3;
	sattr.priority = MRT_PRI09;
	strncpy(sattr.name,"SEM_TEST", MAXPNAME);

	semid = mrt_semalloc(&sattr);
	if( semid < 0)
		{
		printf("mrt_semalloc: rcode=%5d.\n", semid);
		exit(1);
		}
	else
		printf("Semaphore Allocated %d\n", semid);
	/*---------------------- GET SEMAPHORE STATISTICS -----------------------*/

	rcode = mrt_getsemstat(semid, &sstat);
	if( rcode != 0)
		{
		printf("mrt_getsemstat: rcode=%5d.\n", rcode);
		exit(1);
		}

	printf("Semaphore #UPs		= %d\n", sstat.ups);
	printf("Semaphore #DOWNs 	= %d\n", sstat.downs);
	printf("Maximum Process in Queue= %d\n", sstat.maxinQ);

	/*---------------------- FREE SEMAPHORE  -----------------------*/

	rcode = mrt_semfree(semid);
	if( rcode != 0)
		{
		printf("mrt_semfree: rcode=%5d.\n", rcode);
		exit(1);
		}

	printf("mrt_semfree: OK\n");
}
