#define _exit	__exit
#include <lib.h>
#include <unistd.h>

#ifndef MRTTASK
#include <minix/syslib.h>
#include <minix/com.h>
#endif

PUBLIC void _exit(status)
int status;
{
  message m;

#ifdef MRT
  _rtkrncall(MRTSET2NRT,NULL);		/* Convert the RT-process into a NRT-process */
#endif

   m.m1_i1 = status;
  _syscall(MM, EXIT, &m);
  return;
}
