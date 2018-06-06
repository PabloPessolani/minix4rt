#include "syslib.h"

PUBLIC int rtt_getiattr(caller,irq,ptr)
int  caller;
int  irq;
char *ptr;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = irq;
  m.m1_p1 = ptr;
  
  return(_taskcall(MRTTASK, MRT_GETIATTR, &m));
}
