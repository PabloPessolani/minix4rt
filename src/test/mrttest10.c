/******************************************************************************/
/* 					mrttest10.c							*/
/* Test the MRT_send System Call setting the RS232 interrupt Descriptor Attrib*/
/* Usage:												*/
/* 	mrttest10											*/
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
#include <minix/com.h>
#include <minix/const.h>

#define RS232_IRQ 	4
#define HARDWARE		-1

_PROTOTYPE(int main, (int argc, char *argv []));


int main(argc, argv)
int argc;
char *argv[];
{

	int rcode, pid;
	mrt_pattr_t pattr;
	mrt_irqattr_t rs_attrs;
	mrt_pint_t pint;
	mrtpid_t mrtpid;
	mrt_msg_t 	rqst;	
	mrt_mhdr_t  hrqst;
	char text[100];


	/*======= get the process internal values to get the process number =======*/
   	pid = getpid();
	rcode = mrt_getpint(pid, &pint);
	if( rcode != 0)
		{
		printf("mrt_getpint: pid= %d rcode=%5d.\n", pid, rcode);
		exit(rcode);
		}
	else
		printf("mrt_getpint: OK\n");

	/*========== set the interrupt descriptor attributes	======================*/
  	rs_attrs.period    = 0;
  	rs_attrs.task      = pint.p_nr;
  	rs_attrs.watchdog  = HARDWARE;
  	rs_attrs.priority  = MRT_PRI05;
  	rs_attrs.irqtype   = MRT_RTIRQ;
  	strncpy(rs_attrs.name,"RT-RS232NEW",15); 

	rcode = mrt_setiattr(RS232_IRQ , &rs_attrs);
	if( rcode != 0)
		{
		printf("mrt_setiattr: irq=%2d rcode=%5d.\n", RS232_IRQ,rcode); 
 		exit(rcode);
		}
	else
		printf("mrt_setiattr: OK\n"); 

	mrtpid.pid 	= 0;
	mrtpid.p_nr = MRT_ANYPROC;

	/*========================== start RT MODE =======================*/
	rcode = mrt_RTstart(20,1000);
	if( rcode != 0)
		{
		printf("mrt_RTstart: rcode=%5d.\n", rcode);
		exit(rcode);
		}
	else
		printf("mrt_RTstart: OK\n");

	/*========================== SET PROCESS ATTRIBUTES ================*/
	pattr.flags 	= MRT_P_REALTIME;
	pattr.baseprty 	= MRT_PRI13;
	pattr.period 	= 0;
	pattr.limit 	= 0;
	pattr.deadline 	= 0;
	pattr.watchdog 	= -1;
	pattr.mq_size 	= 10;
	pattr.mq_flags	= 0;
	
	rcode = mrt_setpattr(&pattr);
	if( rcode != 0)
		{
		printf("mrt_setpattr: rcode=%5d.\n", rcode);
		exit(1);
		}
	else
		printf("mrt_setpattr: OK\n");	

	/*========================== CONVERT TO RT  =======================*/
	rcode = mrt_set2rt();
	if( rcode != 0)
		{
		printf("mrt_set2rt: rcode=%5d.\n", rcode);
		exit(rcode);
		}

	while(1)
		{
		rcode = mrt_rcv(mrtpid,&rqst,&hrqst,MRT_FOREVER);
		if ( rcode != 0)
			{
			mrt_set2nrt();
			printf("mrt_rcv: rcode=%d\n",rcode);
			exit(rcode);
			}	
		sprintf(text,"src=%d, mtype=%d, mid=%d, tstamp=%d\n"
			,hrqst.src.p_nr
			,hrqst.mtype
			,hrqst.mid
			,hrqst.tstamp);
		mrt_print(text);
		}
}
