#include "syslib.h"

PUBLIC int rtt_rtstop(caller)
int caller;
{
  message m;

  m.m1_i1 = caller;
  return(_taskcall(MRTTASK, MRT_RTSTOP, &m));
}
