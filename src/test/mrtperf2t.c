/******************************************************************************/
/* 					mrtperf2t.c							*/
/* This is a performance test of message transfer					*/
/* 			10 uprqst<->reply								*/
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

#define	NRMSG		1000

_PROTOTYPE(int main, (int argc, char *argv []));

int main(argc, argv)
int argc;
char *argv[];
{
	mrt_pattr_t s_pattr, f_pattr;
	int rcode, rcode1, rcode2, i,j;
	pid_t pid, ppid;
	mrtpid_t mrtpid;
	mrt_pint_t pint;
	mrt_msg_t 	rqst, rply;
	mrt_mhdr_t  hrqst, hrply;
	mrt_sysstat_t stats;
	mrt_sysval_t val;
	double 	tput;
	unsigned long start, stop , sched1, sched2;

/*----------------------------------------------------------------------------------*/
/*						FATHER							*/
/*----------------------------------------------------------------------------------*/
	if( (pid = fork()) != 0)
		{
		ppid = getpid();
		printf("[FATHER]I am [%d],I will REQUEST my son [%d] in 20 seconds\n",ppid,pid);
		sleep(20);

		rcode = mrt_getpint(pid, &pint);
		if( rcode != 0)
			{
			printf("[FATHER] mrt_getpattr: pid= %d rcode=%5d.\n", pid, rcode);
			exit(1);
			}
		mrtpid.pid 	= pid;
		mrtpid.p_nr = pint.p_nr;

		s_pattr.flags 	= MRT_P_REALTIME;
		s_pattr.baseprty 	= MRT_PRI03;
		s_pattr.period 	= 0;
		s_pattr.limit 	= 0;
		s_pattr.deadline 	= 0;
		s_pattr.watchdog 	= -1;
		s_pattr.mq_size 	= 10;
		s_pattr.mq_flags	= 0;

		rcode = mrt_setpattr(&s_pattr);
		if( rcode != 0)
			{
			printf("[FATHER] mrt_setpattr: rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("[FATHER] Process Attributes set\n");

		rcode = mrt_getsstat( &stats);
		if( rcode != 0)
			{
			printf("mrt_getsstat: rcode=%d.\n\n", rcode);
			exit(1);
			}
		start = stats.ticks;
		sched1 = stats.scheds;
		printf("nrmsg %d [msg]\n",11*NRMSG);		
		printf("start %d [tick]\n",start);		
		rcode = mrt_getsval(&val);
		if( rcode != 0)
			{
			printf("mrt_getsval: rcode=%d.\n\n", rcode);
			exit(1);
			}
		printf("tickrate %d [tick/s]\n",val.tickrate);

		mrt_set2rt();
		for( i = 0; i < NRMSG; i++)
			{
			for( j = 0; j < 10; j++)
				 mrt_arqst(mrtpid,&rqst);
			mrt_rcv(mrtpid,&rply,&hrply,val.tickrate*60);		
			}
	
		mrt_set2nrt();

		rcode = mrt_getsstat( &stats);
		if( rcode != 0)
			{
			printf("mrt_getsstat: rcode=%d.\n\n", rcode);
			exit(1);
			}
		stop = stats.ticks;
		printf("stop %d [tick]\n",stop);		
		printf("testing time %d [tick]\n",stop-start);		

		tput = (double)11 * NRMSG * val.tickrate;
		tput /= (double)(stop-start);
		printf("throughput %f [msg/s]\n", tput);
		printf("msg time   %f [microsec]\n", 1000000/tput);
		rcode = mrt_getsstat( &stats);
		if( rcode != 0)
			{
			printf("mrt_getsstat: rcode=%d.\n\n", rcode);
			exit(1);
			}
		sched2 = stats.scheds;
		printf("RTscheds %d\n",sched2-sched1);
		exit(0);
		}
/*----------------------------------------------------------------------------------*/
/*						SON								*/
/*----------------------------------------------------------------------------------*/
	else
		{
		pid = getpid();
		printf("[SON] I am [%d] , and I will change to Real Time \n",pid);

		s_pattr.flags 	= MRT_P_REALTIME;
		s_pattr.baseprty 	= MRT_PRI04;
		s_pattr.period 	= 0;
		s_pattr.limit 	= 0;
		s_pattr.deadline 	= 0;
		s_pattr.watchdog 	= -1;
		s_pattr.mq_size 	= 10;
		s_pattr.mq_flags	= 0;

		rcode = mrt_setpattr(&s_pattr);
		if( rcode != 0)
			{
			printf("[SON] mrt_setpattr: rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("[SON] Process Attributes set\n");

		ppid = getppid();
		rcode = mrt_getpint(ppid, &pint);
		if( rcode != 0)
			{
			printf("[SON] mrt_getpattr: pid= %d rcode=%5d.\n", pid, rcode);
			exit(1);
			}
		mrtpid.pid 	= ppid;
		mrtpid.p_nr = pint.p_nr;
		rcode = mrt_getsval(&val);
		if( rcode != 0)
			{
			printf("mrt_getsval: rcode=%d.\n\n", rcode);
			exit(1);
			}
		printf("tickrate %d [tick/s]\n",val.tickrate);

		rcode = mrt_set2rt();
		if( rcode != 0 )
			{
			printf("[SON] mrt_set2rt rcode = %d\n",rcode);
			exit(1);
			}

		while(1)
			{
			for( j = 0; j < 10; j++)
				{
				rcode = mrt_rcv(mrtpid,&rqst,&hrqst,val.tickrate*60);
				if( rcode != 0 )
					{
					mrt_set2nrt();
					printf("[SON] mrt_rcv rcode = %d\n",rcode);
					exit(1);
					}
				}		
			mrt_reply(mrtpid,&rply);
			}
		}
}
