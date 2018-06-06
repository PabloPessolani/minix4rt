/* This file handles the 4 system calls that get and set uids and gids.
 * It also handles getpid(), setsid(), and getpgrp().  The code for each
 * one is so tiny that it hardly seemed worthwhile to make each a separate
 * function.
 */

#include "mm.h"
#include <minix/callnr.h>
#include <signal.h>
#include "mproc.h"
#include "param.h"

/* #include MRT_MMDBG	1			Remove comments for debugging		*/

/*===========================================================================*
 *				do_getset				     *
 *===========================================================================*/
PUBLIC int do_getset()
{
/* Handle GETUID, GETGID, GETPID, GETPGRP, SETUID, SETGID, SETSID.  The four
 * GETs and SETSID return their primary results in 'r'.  GETUID, GETGID, and
 * GETPID also return secondary results (the effective IDs, or the parent
 * process ID) in 'result2', which is returned to the user.
 */

  register struct mproc *rmp = mp;
  register int r;

#ifdef MRT_MMDBG 
printf("do_getset: mm_call %d \n",mm_call); 
#endif

  switch(mm_call) {
	case GETUID:
		r = rmp->mp_realuid;
		result2 = rmp->mp_effuid;
		break;

	case GETGID:
		r = rmp->mp_realgid;
		result2 = rmp->mp_effgid;
		break;

	case GETPID:
		r = mproc[who].mp_pid;
		result2 = mproc[rmp->mp_parent].mp_pid;
		break;

	case SETUID:
		if (rmp->mp_realuid != usr_id && rmp->mp_effuid != SUPER_USER)
			return(EPERM);
		rmp->mp_realuid = usr_id;
		rmp->mp_effuid = usr_id;
		tell_fs(SETUID, who, usr_id, usr_id);
		r = OK;
		break;

	case SETGID:
		if (rmp->mp_realgid != grpid && rmp->mp_effuid != SUPER_USER)
			return(EPERM);
		rmp->mp_realgid = grpid;
		rmp->mp_effgid = grpid;
		tell_fs(SETGID, who, grpid, grpid);
		r = OK;
		break;

	case SETSID:
		if (rmp->mp_procgrp == rmp->mp_pid) return(EPERM);
		rmp->mp_procgrp = rmp->mp_pid;
		tell_fs(SETSID, who, 0, 0);
		/*FALL THROUGH*/

	case GETPGRP:
		r = rmp->mp_procgrp;
		break;
#ifdef MRT
	case MRTCALL:

#ifdef MRT_MMDBG 
printf("MRTCALL: rtt_opcode %d \n",rtt_opcode); 
#endif
		if (rmp->mp_realuid != usr_id && rmp->mp_effuid != SUPER_USER)
			{
#ifdef MRT_MMDBG 
printf("MRTCALL: realuid %d effuid %d\n",rmp->mp_realuid,rmp->mp_effuid); 
#endif
			return(EPERM);
			}
		mp = &mproc[who];			/* the proc who calls MM */
		switch(rtt_opcode) /* MRTTASK Function selection */
			{
			case MRT_GETIATTR:
				r= rtt_getiattr((int)mp->mp_pid, rtt_irq, (char *)rtt_irqattr); 
				break;
			case MRT_GETISTAT:
				r= rtt_getistat((int)mp->mp_pid, rtt_irq, (char *)rtt_irqstat); 
				break;
			case MRT_GETIINT:
				r= rtt_getiint((int)mp->mp_pid, rtt_irq, (char *)rtt_irqint); 
				break;
			case MRT_SETIATTR:
				r= rtt_setiattr((int)mp->mp_pid, rtt_irq, (char *)rtt_irqattr); 
				break;
			case MRT_GETSSTAT:
				r= rtt_getsstat((int)mp->mp_pid, (char *)rtt_sysstat); 
				break;
			case MRT_GETSVAL:
				r= rtt_getsval((int)mp->mp_pid, (char *)rtt_sysval); 
				break;
			case MRT_SETSVAL:
				r= rtt_setsval((int)mp->mp_pid, rtt_flags); 
				break;
			case MRT_RESTART:
				r= rtt_restart((int)mp->mp_pid, rtt_harmonic, rtt_refresh); 
				break;
			case MRT_GETPATTR:
				r= rtt_getpattr((int)mp->mp_pid, rtt_pid, (char *)rtt_pattrib); 
				break;
			case MRT_SETPATTR:
				r= rtt_setpattr((int)mp->mp_pid, (char *)rtt_pattrib); 
				break;
			case MRT_GETPSTAT:
				r= rtt_getpstat((int)mp->mp_pid, rtt_pid, (char *)rtt_pstats); 
				break;
			case MRT_GETPINT:
				r= rtt_getpint((int)mp->mp_pid, rtt_pid, (char *)rtt_pint); 
				break;
			case MRT_CLRPSTAT:
				r= rtt_clrpstat((int)mp->mp_pid, rtt_pid); 
				break;
			case MRT_RTSTART:
				r= rtt_rtstart((int)mp->mp_pid, rtt_harmonic, rtt_refresh); 
				break;
			case MRT_RTSTOP:
				r= rtt_rtstop((int)mp->mp_pid); 
				break;
			case MRT_EXIT:
				r= rtt_exit((int)mp->mp_pid); 
				break;
			case MRT_SEMALLOC:
				r= rtt_semalloc((int)mp->mp_pid, (char *)rtt_sattr); 
				break;
			case MRT_SEMFREE:
				r= rtt_semfree((int)mp->mp_pid, rtt_semid); 
				break;
			case MRT_GETSEMSTAT:
				r= rtt_getsemstat((int)mp->mp_pid, rtt_semid, (char *)rtt_sstat); 
				break;
			case MRT_GETSEMINT:
				r= rtt_getsemint((int)mp->mp_pid, rtt_semid, (char *)rtt_sint); 
				break;
			case MRT_GETSEMID:
				r= rtt_getsemid((int)mp->mp_pid, rtt_sname); 
				break;
			case MRT_GETSEMATTR:
				r= rtt_getsemattr((int)mp->mp_pid, rtt_semid, (char *)rtt_sattr); 
				break;
			case MRT_GETMQSTAT:
				r= rtt_getmqstat((int)mp->mp_pid, rtt_msgqid, (char *)rtt_mqstat); 
				break;
			case MRT_CLRSSTAT:
				r= rtt_clrsstat((int)mp->mp_pid); 
				break;
			case MRT_CLRISTAT:
				r= rtt_clristat((int)mp->mp_pid, rtt_irq); 
				break;
			case MRT_CLRSEMSTAT:
				r= rtt_clrsemstat((int)mp->mp_pid, rtt_semid); 
				break;
			case MRT_CLRMQSTAT:
				r= rtt_clrmqstat((int)mp->mp_pid, rtt_msgqid); 
				break;
			default:
				r = EINVAL;
				break;
			}
		break;
#endif
	default:
		r = EINVAL;
		break;	
  }

#ifdef MRT_MMDBG 
printf("do_getset: return error=%d \n",r); 
#endif

  return(r);
}
