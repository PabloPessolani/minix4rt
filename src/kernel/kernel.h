/* This is the master header for the kernel.  It includes some other files
 * and defines the principal constants.
 */
#define _POSIX_SOURCE      1	/* tell headers to include POSIX stuff */
#define _MINIX             1	/* tell headers to include MINIX stuff */
#define _SYSTEM            1	/* tell headers that this is the kernel */

/* The following are so basic, all the *.c files get them automatically. */
#include <minix/config.h>	/* MUST be first */
#include <ansi.h>		/* MUST be second */
#include <sys/types.h>
#include <minix/const.h>
#include <minix/type.h>
#include <minix/syslib.h>

#include <string.h>
#include <limits.h>
#include <errno.h>

#include "const.h"
#include "type.h"
#include "proto.h"
#include "glo.h"

#ifdef MRT
#define KRN2USR(proc, dst, src, size)    								\
	do	{												\
		user_phys = numap(proc->p_nr, (vir_bytes) dst, (vir_bytes) size);		\
		if (user_phys == 0) return(EINVAL);							\
		phys_copy(vir2phys(src), user_phys, (phys_bytes) size);			\
		} while(0);

#define USR2KRN(proc, dst, src, size)    								\
	do	{												\
		user_phys = numap(proc->p_nr, (vir_bytes) src, (vir_bytes) size);		\
		if (user_phys == 0) return(EINVAL);							\
		phys_copy(user_phys, vir2phys(dst), (phys_bytes) size);			\
		} while(0);

#define USR2USR(pdst, psrc, dst, src, size)    							\
	do	{												\
		user_phys = numap(pdst->p_nr, (vir_bytes) dst, (vir_bytes) size);		\
		user_phys1= numap(psrc->p_nr, (vir_bytes) src, (vir_bytes) size);		\
		if (user_phys1 == 0 || user_phys1 == 0 ) return(EINVAL);			\
		phys_copy(user_phys1, user_phys, (phys_bytes) size);				\
		} while(0);


#define VTALLOC(pvt, vt, per, lim, act, par, own, pty)					\
	do	{												\
		vt.period = per;										\
		vt.limit  = lim;										\
		vt.action = act;										\
		vt.param  = par;										\
		vt.owner  = own;										\
		vt.priority = pty;									\
		pvt = MRT_vtimer_alloc(&vt);								\
		} while(0);

#define VTSET(pvt, per, lim, act, par, pty)							\
	do	{												\
		pvt->period 	= per;								\
		pvt->limit  	= lim;								\
		pvt->action 	= act;								\
		pvt->param  	= par;								\
		pvt->priority	= pty;								\
		MRT_vtimer_ins(pvt);									\
		} while(0);

#endif

