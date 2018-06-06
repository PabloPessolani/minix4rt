/******************************************************************************/
/* 					mrttest2.c							*/
/* Test the mrt_setiattr() System Call to set Interrupt Descriptor Attributes	*/
/* Usage:												*/
/* 	mrttest2											*/
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
#define RS232_TASK 	-11

_PROTOTYPE(int main, (int argc, char *argv []));


int main(argc, argv)
int argc;
char *argv[];
{

	int rcode;
	mrt_irqattr_t rs_attrs;

  	rs_attrs.period    = 0;
  	rs_attrs.task      = RS232_TASK;
  	rs_attrs.watchdog  = HARDWARE;
  	rs_attrs.priority  = MRT_PRI05;
  	rs_attrs.irqtype   = MRT_RTIRQ;
  	strncpy(rs_attrs.name,"RT-RS232NEW",15); 

	rcode = mrt_setiattr(RS232_IRQ , &rs_attrs);
	printf("mrt_setiattr: \nirq=%2d rcode=%5d.\n", RS232_IRQ,rcode); 
	if( rcode != 0) exit(1);
}
