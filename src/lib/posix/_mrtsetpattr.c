/*===========================================================================*
 *				mrt_setpattr				     				*
 * Set processing RT-attributtes of a process						*
 * Input:												* 	
 *                mrt_pattrib_t *p_attrib; process attributes data structure  *
 * Output:												*
 *		returns 										*
 * 			OK  			: if all it's OK					*
 *			EINVAL		: if the destination data structure if	*
 *					 	out of memory segment				*
 * 			E_MRT_BADPTYPE 	: if the process type is not valid		*
 *			E_MRT_BADPRTY 	: if the process priority is not valid	*
 *			E_MRT_BADPER	: if the period is not valid			*
 *			E_MRT_BADDEAD	: if the deadline is not valid		*
 * 			E_MRT_BADWDOG	: if the watchdog process ID is not valid *
*===========================================================================*/

#include <lib.h>
#include <minix/syslib.h>

#define mrt_setpattr _mrt_setpattr
#include <unistd.h>

#ifndef MRTTASK
#include <minix/com.h>
#endif

PUBLIC int mrt_setpattr(p_attrib )
mrt_pattr_t *p_attrib;
{
  message m;

	m.m1_p1  = (char * ) p_attrib;

	m.m1_i1  = MRT_SETPATTR;
	_syscall(MM, MRTCALL, &m);
	return(m.m_type);
}
