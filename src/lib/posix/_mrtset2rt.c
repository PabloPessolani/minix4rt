/*===========================================================================*
 *				mrt_set2rt				     				*
 * Converts the NRT-process into a RT-process 						*
 * Input:												*
 * Output:												*
 *		returns 										*
 *			OK (0) 			: if all it's OK				*
 * 			E_MRT_BADPTYPE		: Error in process attributes		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_set2rt _mrt_set2rt
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_set2rt(void)
{
  return(_rtkrncall(MRTSET2RT,NULL)); /* Converts the NRT-process into a RT-process */
}

