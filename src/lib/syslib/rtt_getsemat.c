#include "syslib.h"

PUBLIC int rtt_getsemattr(caller,semid,ptr)
int  caller;
int semid;
char *ptr;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = semid;
  m.m1_p1 = ptr;
  return(_taskcall(MRTTASK, MRT_GETSEMATTR, &m));
}

