/******************************************************************************/
/* 					mrttest8b.c							*/
/* Test the mrt_rcv() Kernel Call						 		*/
/* Usage:												*/
/* 	mrttest8b											*/
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

int main()
{
	mrt_pattr_t s_pattr, f_pattr;
	int rcode;
	pid_t pid, ppid;
	mrtpid_t mrtpid;
	mrt_pint_t pint;
	mrt_msg_t 	msg;
	mrt_mhdr_t hmsg;

/*----------------------------------------------------------------------------------*/
/*						FATHER							*/
/*----------------------------------------------------------------------------------*/
	if( (pid = fork()) != 0)
		{
		ppid = getpid();
		printf("[FATHER]I am [%d],I will RECEIVE a message from my son [%d] in 10 seconds\n",ppid,pid);
		sleep(10);

		rcode = mrt_getpint(pid, &pint);
		if( rcode != 0)
			{
			printf("[FATHER] mrt_getpattr: pid= %d rcode=%5d.\n", pid, rcode);
			exit(1);
			}
		mrtpid.pid 	= pid;
		mrtpid.p_nr = pint.p_nr;

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

		rcode = mrt_print("[FATHER] I will RECEIVE a msg from my son...\n");
		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[FATHER] mrt_print rcode = %d\n",rcode);
			exit(1);
			}
		
		rcode = mrt_rcv(mrtpid,&msg,&hmsg,10*200);
		mrt_set2nrt();
		printf("[FATHER] mrt_rcv rcode = %d\n",rcode);
		sleep(30);
		printf("[FATHER] bye\n");
		exit(0);
		}
/*----------------------------------------------------------------------------------*/
/*						SON								*/
/*----------------------------------------------------------------------------------*/
	else
		{
		pid = getpid();
		printf("[SON] I am [%d] , and I will change to Real Time and put on sleep\n",pid);

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
		
		rcode = mrt_sleep(60*200);
		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[SON] mrt_sleep rcode = %d\n",rcode);
			exit(0);
			}

		rcode = mrt_print("[SON] I wake up...\n");
		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[SON] mrt_print2 rcode = %d\n",rcode);
			exit(1);
			}

		rcode = mrt_set2nrt();
		printf("[SON]Bye\n");
		}	
}
