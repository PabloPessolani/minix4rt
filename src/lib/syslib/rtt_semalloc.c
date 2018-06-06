#include "syslib.h"

PUBLIC int rtt_semalloc(caller,ptr)
int  caller;
char *ptr;
{
  message m;

  m.m1_i1 = caller;
  m.m1_p1 = ptr;
  return(_taskcall(MRTTASK, MRT_SEMALLOC, &m));
}

