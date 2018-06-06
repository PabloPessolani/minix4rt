/*===========================================================================*
 *				mrt_setsval			  	   				*
 * Input:												*
 * 		unsigned int flags = MRT_sv.flags				 		*
 * Output:												*
 *		returns 										*
 *			OK (0) 			: if all it's OK				*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_setsval _mrt_setsval
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_setsval( flags )
unsigned int flags;
{
  message m;

	m.m1_i1  = MRT_SETSVAL;
	m.m1_i2  = flags;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
