/*===========================================================================*
 *				mrt_set2nrt				     				*
 * Converts the RT-process into a NRT-process 						*
 * Input:												*
 * Output:												*
 *		returns 										*
 *			OK (0) 			: if all it's OK				*
 * 			E_MRT_BADPTYPE		: Error in process type			*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_set2nrt _mrt_set2nrt
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_set2nrt(void)
{
  int rcode;

  return(_rtkrncall(MRTSET2NRT,NULL)); /* Converts the NRT-process into a RT-process */
}

