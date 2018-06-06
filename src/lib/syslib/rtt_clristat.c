#include "syslib.h"

PUBLIC int rtt_clristat(caller,irqd)
int  caller;
int  irqd;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = irqd;
  return(_taskcall(MRTTASK, MRT_CLRISTAT, &m));
}
