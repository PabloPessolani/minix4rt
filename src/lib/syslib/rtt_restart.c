#include "syslib.h"

PUBLIC int rtt_restart(caller,harmonic,refresh)
int caller;
int harmonic;
int refresh;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = harmonic;
  m.m1_i3 = refresh;
  return(_taskcall(MRTTASK, MRT_RESTART, &m));
}
