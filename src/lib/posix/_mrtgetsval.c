/*===========================================================================*
 *				mrt_getsval				     				*
 * Input:												* 	
 *		vals: system wide processing values data structure			*
 * Output:												*
 *		vals: system wide processing values data structure			*
 *		return 	OK ( 0 )								*
 *				EINVAL: for out of segmente data strcuture		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getsval _mrt_getsval
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getsval(vals)
mrt_sysval_t *vals;
{
  message m;
	
	m.m1_p1  = (char *)vals;

	m.m1_i1  = MRT_GETSVAL;
	_syscall(MM, MRTCALL, &m);	
	return(m.m_type);
}
