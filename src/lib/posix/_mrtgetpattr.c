/*===========================================================================*
 *				mrt_getpattr				     				*
 * Gets process attributes									*
 * Input:												* 	
 * 			pid_t pid;			PID of the process 			*
 * Output:												*
 *                mrt_pattrib_t *p_attrib; process attributes data structure  *
 *		returns 										*
 * 				OK  		: if all it's OK					*
 *				EINVAL	: if the destination data structure if	*
 *						 out of memory segment				*
 *				E_BAD_PROC	: if the process ID is not valid		*
 *===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_getpattr _mrt_getpattr
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_getpattr(pid, p_attrib )
pid_t pid;
mrt_pattr_t *p_attrib;
{
  message m;

	m.m1_i2  = (int) pid;
	m.m1_p1  = (char * ) p_attrib;

	m.m1_i1  = MRT_GETPATTR;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
