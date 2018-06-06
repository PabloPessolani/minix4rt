#include "syslib.h"

PUBLIC int rtt_exit(caller)
int caller;			/* which process has exited */
{
/* A process has exited.  Tell the RT-kernel */

  message m;

  /* Tell the RT-kernel that the process is no longer runnable. */
  m.m1_i1  = caller;
  return(_taskcall(MRTTASK, MRT_EXIT, &m));
}

