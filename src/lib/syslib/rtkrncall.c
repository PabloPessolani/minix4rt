#include <lib.h>
#include "syslib.h"

PUBLIC int _rtkrncall(rtk_nr, parm_ptr)
int rtk_nr;
void *parm_ptr;
{
  return(_rtkcall(rtk_nr, parm_ptr));
}
