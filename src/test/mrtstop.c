/******************************************************************************/
/* 					mrtstop.c							*/
/* Stops the Real Time processing mode using mrt_RTstop() System Call		*/
/* Usage:												*/
/* 	mrtstop											*/
/******************************************************************************/
#include <minix/config.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <minix/type.h>
#include <unistd.h>
#include <stdio.h> 

#include <lib.h>
#include <minix/syslib.h>

_PROTOTYPE(int main, (int argc, char *argv []));

int main()
{
	int rcode;

	rcode = mrt_RTstop();
	if( rcode != 0)
		{
		printf("mrt_RTstop: rcode=%5d.\n", rcode);
 		exit(1);
		}
	else
		printf("System is in Non Realtime Mode\n");
	
	exit(0);
}
