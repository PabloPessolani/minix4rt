/*===========================================================================*
 *				mrt_semdown				     				*
 * Downs a Semaphore										*
 * Input:	semaphore ID									* 	
 * Output:	OK or E_MRT_BADSEM								*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_semdown _mrt_semdown
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_semdown(semid, timeout)
int semid;
lcounter_t	timeout;

{
	mrt_down_t dw;

	dw.index 	= semid;
	dw.timeout 	= timeout;

	return(_rtkrncall(MRTSEMDOWN,(void *)&dw));

}
