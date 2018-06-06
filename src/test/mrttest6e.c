/******************************************************************************/
/* 					mrttest6e.c							*/
/* Test the mrt_sleep(), mrt_wakeup() Sys Calls						*/
/* Usage:												*/
/* 	mrttest6e											*/
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

int main()
{
	mrt_pattr_t s_pattr, f_pattr;
	int rcode,i,j;
	pid_t pid[10], ppid;
	mrtpid_t mrtpid[10];
	mrt_pint_t pint[10];

/*----------------------------------------------------------------------------------*/
/*						FATHER							*/
/*----------------------------------------------------------------------------------*/
	for( i = 0; i < 10 ; i++)
		pid[i] = 1;

	for( i = 0; i < 10 && pid[i] != 0 ; i++)
		pid[i] = fork();

	if( i == 10) 
		{
		ppid = getpid();
		printf("[FATHER]I am [%d],I will wakeup my sons \n",ppid,pid);

		for ( j = 0; j < 10; j++)
			{
			rcode = mrt_getpint(pid[j], &pint[j]);
			if( rcode != 0)
				{
				printf("[FATHER] mrt_getpattr: pid= %d rcode=%5d.\n", pid[j], rcode);
				exit(1);
				}
			mrtpid[j].pid  = pid[j];
			mrtpid[j].p_nr = pint[j].p_nr;
			}

		s_pattr.flags 	= MRT_P_REALTIME;
		s_pattr.baseprty 	= MRT_PRI04;
		s_pattr.period 	= 0;
		s_pattr.limit 	= 0;
		s_pattr.deadline 	= 0;
		s_pattr.watchdog 	= -1;
		s_pattr.mq_size 	= 3;
		s_pattr.mq_flags	= 0;

		rcode = mrt_setpattr(&s_pattr);
		if( rcode != 0)
			{
			printf("[FATHER] mrt_setpattr: rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("[FATHER] Process Attributes set\n");

		rcode = mrt_set2rt();
		if( rcode != 0 )
			{
			printf("[FATHER] mrt_set2rt rcode = %d\n",rcode);
			exit(1);
			}

		mrt_print("[FATHER] I will wakeup my sons...\n");
			
		for ( j = 0; j < 10; j++);
			mrt_wakeup(mrtpid[j]);

		rcode = mrt_set2nrt();

		printf("[FATHER]Bye\n");
		}
/*----------------------------------------------------------------------------------*/
/*						SON								*/
/*----------------------------------------------------------------------------------*/
	else
		{
		pid[i] = getpid();
		printf("[SON]%d I am [%d] , and I will change to RT and put on sleep\n",i,pid[i]);

		s_pattr.flags 	= MRT_P_REALTIME;
		s_pattr.baseprty 	= MRT_PRI03;
		s_pattr.period 	= 0;
		s_pattr.limit 	= 0;
		s_pattr.deadline 	= 0;
		s_pattr.watchdog 	= -1;
		s_pattr.mq_size 	= 3;
		s_pattr.mq_flags	= 0;

		rcode = mrt_setpattr(&s_pattr);
		if( rcode != 0)
			{
			printf("[SON] mrt_setpattr: rcode=%5d.\n", rcode);
			exit(1);
			}
		else
			printf("[SON] Process Attributes set\n");


		rcode = mrt_set2rt();
		if( rcode != 0 )
			{
			printf("[SON] mrt_set2rt rcode = %d\n",rcode);
			exit(1);
			}

		rcode = mrt_print("[SON] I am going to sleep...\n");
		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[SON] mrt_print1 rcode = %d\n",rcode);
			exit(1);
			}
		
		rcode = mrt_sleep(MRT_FOREVER);
		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[SON] mrt_sleep rcode = %d\n",rcode);
			exit(1);
			}

		rcode = mrt_print("[SON] I wake up...\n");
		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[SON] mrt_print2 rcode = %d\n",rcode);
			exit(1);
			}

		rcode = mrt_set2nrt();
		printf("[SON]%d Bye\n",i);
		}	
}
