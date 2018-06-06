#include "syslib.h"

PUBLIC int rtt_clrmqstat(caller,mqid)
int  caller;
int  mqid;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = mqid;
  return(_taskcall(MRTTASK, MRT_CLRMQSTAT, &m));
}
