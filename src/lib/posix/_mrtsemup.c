/*===========================================================================*
 *				mrt_semup				     				*
 * Ups a Semaphore										*
 * Input:	semaphore ID									* 	
 * Output:	OK or E_MRT_BADSEM								*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_semup _mrt_semup
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_semup(semid)
int semid;
{

	_rtkrncall(MRTSEMUP,(void *)&semid);
}
