#include "syslib.h"

PUBLIC int rtt_clrsemstat(caller,semid)
int  caller;
int  semid;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = semid;
  return(_taskcall(MRTTASK, MRT_CLRSEMSTAT, &m));
}
