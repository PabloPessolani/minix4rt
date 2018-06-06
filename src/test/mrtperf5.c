/******************************************************************************/
/* 					mrtperf5.c							*/
/* PRODUCER/CONSUMER WITH SEMAPHORES							*/ 
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

#define	NRTRANSF	10000
#define	BUFSIZE	10
#define	TIMEOUT	1000*60

_PROTOTYPE(int main, (int argc, char *argv []));

int main(argc, argv)
int argc;
char *argv[];
{

	pid_t pid, ppid;
	mrt_pattr_t pattr;
	mrt_sysstat_t stats;
	int rcode, i, status;
	mrt_sysval_t val;
	double 	tput;
	unsigned long start, stop , sched1, sched2;
	mrt_semattr_t 	sattr;
	int	mutex, full, empty;

/*----------------------------------------------------------------------------------*/
/*					FATHER/PRODUCER							*/
/*----------------------------------------------------------------------------------*/
	if( (pid = fork()) != 0)
		{
		/* Get PID of PRODUCER and CONSUMER	*/
		ppid = getpid();
		printf("[PRODUCER]I am [%d],my son is [%d]\n",ppid,pid);
		sleep(10);

		/* Set process RT attributes	*/
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
			printf("[PRODUCER] mrt_setpattr: rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("[PRODUCER] Process Attributes set\n");

		/* GET Semaphores IDs */
		rcode = mrt_getsemid("MUTEX");
		if( rcode < 0)
			{
			printf("mrt_getsemid: MUTEX rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("Semaphore MUTEX ID = %d\n", rcode);
		mutex = rcode;

		rcode = mrt_getsemid("FULL");
		if( rcode < 0)
			{
			printf("mrt_getsemid: FULL rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("Semaphore FULL ID = %d\n", rcode);
		full = rcode;

		rcode = mrt_getsemid("EMPTY");
		if( rcode < 0)
			{
			printf("mrt_getsemid: EMPTY rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("Semaphore EMPTY ID = %d\n", rcode);
		empty = rcode;

		/* Get system statistics for performance computation */
		rcode = mrt_getsstat( &stats);
		if( rcode != 0)
			{
			printf("mrt_getsstat: rcode=%d.\n\n", rcode);
			exit(1);
			}
		start = stats.ticks;
		sched1 = stats.scheds;
		printf("Number of transfers %d \n",NRTRANSF);
		printf("Buffer size %d \n",BUFSIZE);
		printf("start %d [tick]\n",start);		
	
		/* REALTIME PROCESSING	- START */
		mrt_set2rt();
		for( i = 0; i < NRTRANSF; i++)
			{
			mrt_semdown(empty, MRT_FOREVER);
			mrt_semdown(mutex, MRT_FOREVER);
			/* copy to buffer	*/
			mrt_semup(mutex);
			mrt_semup(full);
			}
		mrt_set2nrt();
		/* REALTIME PROCESSING	- STOP */

		/* Get system statistics for performance computation */
		rcode = mrt_getsstat( &stats);
		if( rcode != 0)
			{
			printf("mrt_getsstat: rcode=%d.\n\n", rcode);
			exit(1);
			}
		stop = stats.ticks;
		printf("stop %d [tick]\n",stop);		
		printf("testing time %d [tick]\n",stop-start);		

		rcode = mrt_getsval(&val);
		if( rcode != 0)
			{
			printf("mrt_getsval: rcode=%d.\n\n", rcode);
			exit(1);
			}
		printf("tickrate %d [tick/s]\n",val.tickrate);
		
		tput = (double) (NRTRANSF * 4 * val.tickrate);
		tput /= (double)(stop-start);
		printf("throughput  %f [updown/s]\n", tput);
		printf("updown time %f [microsec]\n", 1000000/tput);

		rcode = mrt_getsstat( &stats);
		if( rcode != 0)
			{
			printf("mrt_getsstat: rcode=%d.\n\n", rcode);
			exit(1);
			}
		sched2 = stats.scheds;
		printf("RTscheds %d\n",sched2-sched1);
		wait(&status);
		sleep(60);
		printf("[PRODUCER] Exiting\n");
		exit(0);
		}
/*----------------------------------------------------------------------------------*/
/*					SON/CONSUMER							*/
/*----------------------------------------------------------------------------------*/
	else
		{
		pid = getpid();
		printf("[CONSUMER] I am [%d] , and I will change to Real Time \n",pid);

		pattr.flags 	= MRT_P_REALTIME;
		pattr.baseprty 	= MRT_PRI04;
		pattr.period 	= 0;
		pattr.limit 	= 0;
		pattr.deadline 	= 0;
		pattr.watchdog 	= -1;
		pattr.mq_size 	= 3;
		pattr.mq_flags	= 0;
		rcode = mrt_setpattr(&pattr);
		if( rcode != 0)
			{
			printf("[CONSUMER] mrt_setpattr: rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("[CONSUMER] Process Attributes set\n");

		/*---------------------- ALLOC SEMAPHORES  --------------------------*/
		sattr.flags = SEM_PRTYORDER | SEM_PRTYINHERIT | SEM_MUTEX;
		sattr.priority = MRT_PRI09;
		sattr.value = 1;
		strncpy(sattr.name,"MUTEX", MAXPNAME);
		rcode = mrt_semalloc(&sattr);
		if( rcode < 0)
			{
			printf("mrt_semalloc: MUTEX rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("Semaphore MUTEX Allocated %d\n", rcode);
		mutex = rcode;

		sattr.flags = SEM_PRTYORDER;
		sattr.value = 0;
		strncpy(sattr.name,"FULL", MAXPNAME);
		rcode = mrt_semalloc(&sattr);
		if( rcode < 0)
			{
			printf("mrt_semalloc: FULL rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("Semaphore FULL Allocated %d\n", rcode);
		full = rcode;

		sattr.flags = SEM_PRTYORDER;
		sattr.value = BUFSIZE;
		strncpy(sattr.name,"EMPTY", MAXPNAME);
		rcode = mrt_semalloc(&sattr);
		if( rcode < 0)
			{
			printf("mrt_semalloc: EMPTY rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("Semaphore EMPTY Allocated %d\n", rcode);
		empty = rcode;

		/* REALTIME PROCESSING	- START */
		rcode = mrt_set2rt();
		if( rcode != 0 )
			{
			printf("[SON] mrt_set2rt rcode = %d\n",rcode);
			exit(1);
			}

		for( i = 0; i < NRTRANSF; i++)
			{
			mrt_semdown(full, MRT_FOREVER);
			mrt_semdown(mutex, MRT_FOREVER);
			mrt_semup(mutex);
			mrt_semup(empty);
			}
		rcode = mrt_set2nrt();
		sleep(60);
		printf("[CONSUMER] Exiting\n");
		/* REALTIME PROCESSING	- STOP */
		}
	exit(0);	
}
