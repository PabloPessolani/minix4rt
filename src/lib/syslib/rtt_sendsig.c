#include "syslib.h"

PUBLIC int rtt_sendsig(caller,pid)
int caller;
int pid;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = pid;
  return(_taskcall(MRTTASK, MRT_STDSIGNAL, &m));
}
