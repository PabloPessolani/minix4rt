/******************************************************************************/
/* 					mrttest12.c							*/
/* Test the mrt_semalloc() System Call to set and alloc a Semaphore		*/
/* Usage:												*/
/* 	mrttest12											*/
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
	int rcode;

	/*---------------------- ALLOC A SEMAPHORE  --------------------------*/
	sattr.flags = SEM_PRTYORDER | SEM_PRTYINHERIT;
	sattr.value = 3;
	sattr.priority = MRT_PRI09;
	strncpy(sattr.name,"SEM_TEST", MAXPNAME);

	rcode = mrt_semalloc(&sattr);
	if( rcode < 0)
		{
		printf("mrt_semalloc: rcode=%5d.\n", rcode);
		exit(1);
		}
	else
		printf("Semaphore Allocated %d\n", rcode);
	sleep(60);
}
