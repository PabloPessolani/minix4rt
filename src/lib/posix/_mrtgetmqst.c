/*===========================================================================*
 *				mrt_getmqstat				     			*
 * Input:												* 	
 *		msgqID										*
 * Output:												*
 *          mqstat = message queue statistics data structure			*
 *		return  = 0 => OK									*
 *          return  < 1 => error 								*
 *			E_MRT_BADMSGQ    (-2031) :The Message Queue does not exists	*
 *			E_MRT_FREEMSGQ   (-2032) :The Message Queue is free		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getmqstat _mrt_getmqstat
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getmqstat(msgqid, mqstat)
int		msgqid;
mrt_msgqstat_t *mqstat;
{
  message m;
	
	m.m1_i2  = msgqid;
	m.m1_p1  = (char *)mqstat;
	m.m1_i1  = MRT_GETMQSTAT;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
