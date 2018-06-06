#include "syslib.h"

PUBLIC int rtt_setsval(caller,flags)
int caller;
unsigned int flags;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = flags;
  return(_taskcall(MRTTASK, MRT_SETSVAL, &m));
}
