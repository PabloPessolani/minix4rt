/******************************************************************************/
/* 					mrttest8d.c							*/
/* Test the mrt_arqst() and mrt_rcv() Kernel Calls			 		*/
/* Usage:												*/
/* 	mrttest8d											*/
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
	int rcode,i;
	pid_t pid, ppid;
	mrtpid_t mrtpid;
	mrt_pint_t pint;
	mrt_msg_t 	msg;
	mrt_mhdr_t hmsg;
	char text[50];

/*----------------------------------------------------------------------------------*/
/*						FATHER							*/
/*----------------------------------------------------------------------------------*/
	if( (pid = fork()) != 0)
		{
		ppid = getpid();
		printf("[FATHER]I am [%d],I will REQUEST messages to my son [%d]\n",ppid,pid);

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
		s_pattr.mq_size 	= 1;
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

		mrt_print("[FATHER] I will REQUESTS 10 msgs to my son...\n");

		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[FATHER] mrt_print rcode = %d\n",rcode);
			exit(1);
			}

		mrt_sleep(200*10);

		for( i = 0; i < 10; i++)
			{
			sprintf(msg.m_m7.m7ca1,"Hello son (%d)",i);
			rcode = mrt_arqst(mrtpid,&msg);
			if( rcode != 0)
				sprintf(text,"[FATHER] msg %d NOT sent\n",i);
			else
				sprintf(text,"[FATHER] msg %d sent\n",i);
			mrt_print(text);
			}
				
		mrt_set2nrt();
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
		ppid = getppid();
		printf("[SON] I am [%d] , and I will change to Real Time and put on sleep\n",pid);

		rcode = mrt_getpint(ppid, &pint);
		if( rcode != 0)
			{
			printf("[SON] mrt_getpattr: pid= %d rcode=%5d.\n", ppid, rcode);
			exit(1);
			}
		mrtpid.pid 	= ppid;
		mrtpid.p_nr = pint.p_nr;

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

		rcode = mrt_set2rt();
		if( rcode != 0 )
			{
			printf("[SON] mrt_set2rt rcode = %d\n",rcode);
			exit(1);
			}

		rcode = mrt_print("[SON] I am going to RECEIVE messages..\n");
		if( rcode != 0 )
			{
			mrt_set2nrt();
			printf("[SON] mrt_print1 rcode = %d\n",rcode);
			exit(1);
			}

		i = 0;
		mrt_sleep(200*10);
		while(1)
			{
			rcode = mrt_rcv(mrtpid,&msg,&hmsg,MRT_FOREVER);
			if( rcode != 0 )
				{
				mrt_set2nrt();
				printf("[SON] mrt_rcv rcode = %d\n",rcode);
				exit(1);
				}
			else
				{
				sprintf(text,"[SON]%d: %s",i,msg.m_m7.m7ca1);
				mrt_print(text);
				i++;
				}
			}

		}	
}
