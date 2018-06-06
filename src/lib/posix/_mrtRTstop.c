/*===========================================================================*
 *				mrt_RTstop				     				*
 * Switch the system processing mode from RT to STANDARD				*
 * Input:												* 	
 * Output: 												*
 * returns OK or MRT_RTACTIVE if a RT process is active 				*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_RTstop  _mrt_RTstop
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_RTstop(void)
{
  message m;

	m.m1_i1  = MRT_RTSTOP;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
