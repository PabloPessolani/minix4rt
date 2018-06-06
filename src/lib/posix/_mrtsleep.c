/*===========================================================================*
 *				mrt_sleep				     				*
 * Put the current process in blocked state for a specified number of ticks	*
 * Input:	lcounter_t	timeout								* 	
 * Output:	OK or E_MRT_BADPTYPE								*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_sleep _mrt_sleep
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_sleep(timeout)
lcounter_t	timeout;
{

	_rtkrncall(MRTSLEEP,(void *)&timeout);
}
