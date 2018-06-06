#include "syslib.h"

PUBLIC int rtt_getmqstat(caller,msgqid,ptr)
int  caller;
int msgqid;
char *ptr;
{
  message m;

  m.m1_i1 = caller;
  m.m1_i2 = msgqid;
  m.m1_p1 = ptr;
  return(_taskcall(MRTTASK, MRT_GETMQSTAT, &m));
}

