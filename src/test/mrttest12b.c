/******************************************************************************/
/* 					mrttest12b.c						*/
/* Test the mrt_getsemint() System Call to get Semaphore internal data		*/
/* Usage:												*/
/* 	mrttest12b											*/
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
	mrt_semint_t sint;
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
	/*---------------------- GET SEMAPHORE INTENAL DATA -----------------------*/

	rcode = mrt_getsemint(semid, &sint);
	if( rcode != 0)
		{
		printf("mrt_getsemint: rcode=%5d.\n", rcode);
		exit(1);
		}

	printf("Semaphore ID 		= %d\n", sint.index);
	printf("Semaphore Owner 	= %d\n", sint.owner);
	printf("Process in Queue 	= %d\n", sint.inQ);
}
