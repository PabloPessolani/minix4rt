#include "syslib.h"

PUBLIC int rtt_clrsstat(caller)
int  caller;
{
  message m;

  m.m1_i1 = caller;
  return(_taskcall(MRTTASK, MRT_CLRSSTAT, &m));
}
