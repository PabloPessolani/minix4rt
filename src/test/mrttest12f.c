/******************************************************************************/
/* 					mrttest12f.c						*/
/* Test the mrt_semdown()					 					*/
/* Usage:												*/
/* 	mrttest12f											*/
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
	mrt_pattr_t 	pattr;
	mrt_semattr_t 	sattr;
	int rcode, semid;

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


	/*---------------------- ALLOC A SEMAPHORE  --------------------------*/
	sattr.flags = SEM_PRTYORDER | SEM_PRTYINHERIT;
	sattr.value = 3;
	sattr.priority = MRT_PRI09;
	strncpy(sattr.name,"SEM_TEST", MAXPNAME);

	semid = rcode = mrt_semalloc(&sattr);
	if( rcode < 0)
		{
		printf("mrt_semalloc: rcode=%5d.\n", rcode);
		exit(1);
		}
	else
		printf("Semaphore Allocated %d\n", rcode);

	/*------------------- REAL TIME ----------------------------------*/
	rcode = mrt_set2rt();
	if( rcode < 0)
		{
		printf("mrt_set2rt: rcode=%5d.\n", rcode);
		exit(1);
		}
	
	rcode = mrt_semdown(semid, MRT_FOREVER);
	if( rcode < 0)
		{
		printf("mrt_semdown: rcode=%5d.\n", rcode);
		exit(1);
		}
	else
		mrt_print("mrt_semdown OK\n");

	mrt_set2nrt();
	/*-------------------  NON REAL TIME ----------------------------------*/

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
	sleep(30);

}
