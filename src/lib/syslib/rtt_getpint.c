#include "syslib.h"

PUBLIC int rtt_getpint(caller,pid,ptr)
int  caller;
int  pid;
char *ptr;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = pid;
  m.m1_p1 = ptr;
  return(_taskcall(MRTTASK, MRT_GETPINT, &m));
}
