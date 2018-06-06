/******************************************************************************/
/* 					mrttest3b.c							*/
/* Test the mrt_getsstat() System Call displaying a graph of the CPU Usage 	*/
/* Usage:												*/
/* 	mrttest3b											*/
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

#define TOPSCALE		70

_PROTOTYPE(int main, (int argc, char *argv []));

int main()
{
	mrt_sysstat_t stats;
	int rcode, i, j;
	long cpu,idle;
printf("                               CPU USAGE\n");
printf("|------10-----20-----30-----40-----50-----60-----70-----80-----90----100\n");
	for( i = 0; i <  20; i++)
		{
		rcode = mrt_getsstat(&stats);
		if( rcode != 0) 
			{	
			printf("mrt_getsstat: rcode=%d.\n\n", rcode);
			exit(rcode);
			}
		else
			printf("|");
		idle = (stats.idlelast*TOPSCALE);
		idle /= stats.idlemax;
		cpu = (TOPSCALE - idle);
		for(j = 0;  j < cpu; j++) 
			printf("#");
		printf("\n");
		sleep(1);
		}
}
