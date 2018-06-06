/*==========================================================================*
 *					mrtproc.c 						    *
 * This file contains essentially all of the RT-process handling.           *
 *==========================================================================*/

#include "kernel.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/type.h>
#include "proc.h"
#include "unistd.h"

#ifdef MRT

#define MAXPRTSTR		80


 _PROTOTYPE (int (*MRT_kernel[]),(void *parm_ptr)) = {
				rtk_set2rt, 	/*  0 = mrt_set2rt	*/
				rtk_set2nrt, 	/*  1 = mrt_set2nrt	*/
				rtk_sleep, 		/*  2 = mrt_sleep		*/
				rtk_wakeup, 	/*  3 = mrt_wakeup	*/

				rtk_print, 		/*  4 = mrt_print		*/

				rtk_rqst, 		/*  5 = mrt_rqst		*/
				rtk_reply, 		/*  6 = mrt_reply		*/
				rtk_arqst, 		/*  7 = mrt_arqst		*/
				rtk_uprqst, 	/*  8 = mrt_uprqst	*/
				rtk_sign,		/*  9 = mrt_sign		*/
				rtk_receive,	/* 10 = mrt_rcv		*/
				rtk_rqrcv,		/* 11 = mrt_rqrcv		*/

				rtk_semup,		/* 12 = mrt_semup		*/
				rtk_semdown		/* 13 = mrt_semdown	*/
				};

/*===========================================================================*
 *				rtk_call				     				*
 * rtk_nr: 												*
 * MRTSET2RT	   0 	function code to set a RT process after MRT_SETPROC 	*
 * MRTSET2NRT	   1	function code to set a NRT process before exit 		*
 * MRTSLEEP		   2	function code to block a running RT-process 		*
 * MRTWAKEUP	   3  function code to wakeup a blocked RT-process 		*
 * MRTRQST		   4	function code to send Synchronous Request RT-messages *
 * MRTREPLY		   5  function code to send Reply   RT-messages 		*
 * MRTARQST	  	   6	function code to send Asynchronous Request RT-messages*
 * MRTPRINT		   7	function code to print a string in the system console *
 * MRTUPRQST	   8	function code to send and UP Request RT-messages  	*
 * MRTSIGN	   	   9	function code to send a SIGNAL RT-message			*
 * MRTRCV	   	  10	function code to Receive RTmessages 			*
 * MRTRQRCV	   	  11	function code to Send Requests and Receive RTmessages	*
 * MRTSEMUP   	  12	function code to UP a Semaphore				*
 * MRTSEMDOWN   	  13	function code to DOWN a Semaphore				*
 *													* 
 * Interrupts are DISABLED									*
 *===========================================================================*/
PUBLIC int rtk_call(rtkcall, rtk_nr, parm_ptr)
int rtkcall;						/* RTKCALL DON'T USE HERE		*/
int rtk_nr;							/* RT KERNEL function number 		*/
void *parm_ptr;						/* pointer to parameters 		*/
{

  register struct proc *rp;
  int rcode;
  mrt_ipc_t ipc;
  phys_bytes user_phys;

  if( !TEST_BIT(MRT_sv.flags,MRT_RTMODE))		/* check for RT-MODE			*/
		return(E_MRT_NORTMODE);

  if( rtk_nr != MRTSET2RT 				/* check for a valid RT-process	*/
    && !MRT_is_realtime(proc_ptr)) 
		return(E_MRT_BADPTYPE);
 
  if(  rtk_nr < 0 || rtk_nr >= NRTKCALLS)		/* check for valid rtk call number 	*/
		{
		printf("rtk_call: bad RT-kernel call number %d\n",rtk_nr);
		return(E_BAD_FCN);				
		}

  if( rtk_nr >= MRTRQST && rtk_nr <= MRTRQRCV)	/* check for IPC rtk call 		*/
	{
	USR2KRN(proc_ptr,&ipc, parm_ptr,sizeof(mrt_ipc_t));
	rcode = (*MRT_kernel[rtk_nr])((void*)&ipc);/* Run the rt-kernel IPC function	*/
	}
  else
	rcode = (*MRT_kernel[rtk_nr])(parm_ptr);	/* Run the rt-kernel call function	*/
  
  lock_pick_proc();

  return(rcode);
}

/*==========================================================================*
 *				MRT_rdyQ_init()						    *
 * Initialize a ALL process priority queues					    *
 *==========================================================================*/
PUBLIC void MRT_rdyQ_init(void)
{
	MRT_plist_rst(&MRT_sp.rdyQ);
}

/*==========================================================================*
 *				MRT_rdyQ_app() aka MRT_ready()			    *
 * Appends a process descriptor into a priority queue                       *
 *==========================================================================*/
PUBLIC void MRT_rdyQ_app(rp)
MRT_proc_t	*rp;
{
	if( rp->st.scheds > rp->rt.limit && rp->rt.limit > 0 )
		SET_BIT(proc_ptr->p_flags,P_STOP);  
	else
		MRT_plist_app(&MRT_sp.rdyQ, &rp->link, rp->rt.priority);
}


/*==========================================================================*
 *				MRT_rdyQ_ins()						    *
 * Inserts a process descriptor into a priority queue as the first object   *
 *==========================================================================*/
PUBLIC void MRT_rdyQ_ins(rp)
MRT_proc_t	*rp;
{
	MRT_plist_ins(&MRT_sp.rdyQ, &rp->link, rp->rt.priority);
}

/*==========================================================================*
 *				MRT_rdyQ_rmv() aka MRT_unready()			    *
 * Removes a process descriptor from a priority queue                       *
 *==========================================================================*/
PUBLIC void MRT_rdyQ_rmv(rp)
MRT_proc_t	*rp;
{
	MRT_plist_rmv(&MRT_sp.rdyQ, &rp->link, rp->rt.priority);
}

/*===========================================================================*
 *				MRT_pick_proc						     *
 * The RT-process SCHEDULER!!. It is called by lock_pick_proc		     *
 * A new process is selected by setting 'proc_ptr'.				     *
 * Returns OK if it has found a RT process else returns !OK			     *
 *===========================================================================*/
PUBLIC int MRT_pick_proc()
{
/* Decide which Real Time Process to run now.  
 * A new process is selected by setting 'proc_ptr'.
 */
	link_t *plink;
	static long int CPUflags;

	MRT_saveFlock(&CPUflags);
	CLR_BIT(MRT_sv.flags,MRT_NEEDSCHED);		/* clear flag set by an IRQd		*/

	if( (plink = MRT_plist_get(&MRT_sp.rdyQ)) != NULL) /* Highest priority process	*/
		{	
		proc_ptr = (MRT_proc_t*)plink->this; 
		MRT_sv.counter.scheds++;			/* update scheduling sys statistics	*/
		proc_ptr->st.timestamp = MRT_sv.counter.ticks; /* stamp the time			*/
		proc_ptr->st.scheds++;				/* update proc sched  statistics	*/
		if( isuserp(proc_ptr))
			bill_ptr = proc_ptr;			/* updates billing pointer		*/
		MRT_prtylvl_set(proc_ptr->rt.priority); 	/* update  prtylvl			*/
		MRT_restoreF(&CPUflags);  			/* LEAVE_CRITICAL_SECTION		*/
		return(OK);
		}

	MRT_prtylvl_set(MRT_PRILOWEST); 			/* update running prty lvl		*/
	MRT_restoreF(&CPUflags);				/* LEAVE_CRITICAL_SECTION		*/
	return(!OK);
}

/*===========================================================================*
 *				MRT_proc_init						     *
 * Initialize the process descriptor Real Time fields 			     * 
 *===========================================================================*/
PUBLIC void MRT_proc_init(void )
{
	int i,p_nr;
	register struct proc *rp;

	SET_BIT(MRT_sv.flags,MRT_NEEDSCHED); 		/* set the flag for scheduling */
	for ( i = 0, p_nr = -NR_TASKS;  i < NR_PROCS ; i++, p_nr++) 
		{
		rp = proc_addr(p_nr);
		MRT_proc_rst(rp);
		}
}

/*===========================================================================*
 *				MRT_proc_rst						     *
 * Reset all RT parameters and statistics of a process			     *
 *===========================================================================*/
PUBLIC void MRT_proc_rst(rp)
struct proc *rp;
{
	MRT_pattr_rst(rp);
	MRT_pstat_rst(rp);
}

/*===========================================================================*
 *				MRT_pstat_rst						     *
 * Reset all RT statistics of a process						     *
 *===========================================================================*/
PUBLIC void MRT_pstat_rst(rp)
struct proc *rp;
{
	rp->st.scheds	= 0;
	rp->st.mdl		= 0;
	rp->st.timestamp 	= 0;
	rp->st.maxlat	= 0;
	rp->st.minlax 	= 0;
	rp->st.msgsent 	= 0;
	rp->st.msgrcvd 	= 0;
}

/*===========================================================================*
 *				MRT_pattr_rst						     *
 * Reset all RT attributes of a process						     *
 *===========================================================================*/
PUBLIC void MRT_pattr_rst(rp)
struct proc *rp;
{
  	rp->rt.flags 	= 0;
	rp->rt.priority 	= rp->rt.baseprty = MRT_PRILOWEST;
  	rp->rt.period 	= 0;
  	rp->rt.limit  	= 0;
  	rp->rt.deadline 	= 0;
	rp->rt.watchdog 	= HARDWARE;

  	rp->mq.size  	= 0;
  	rp->mq.flags  	= 0;

	rp->pmq 		= NULL;
	rp->pvt		= NULL;
	rp->ipcvt		= NULL;
	MRT_list_rst(&rp->sallocL);
	MRT_list_rst(&rp->slockL);

	rp->pto		= NULL;
	rp->pfrom		= NULL;
	rp->psem 		= NULL;
	rp->pmsg		= NULL;
	rp->pmhdr 		= NULL;

	MRT_link_rst(&rp->link,(void *)rp, MRT_OBJPROC);		
}

/*===========================================================================*
 *				MRT_prtylvl_set						     *
 * Set the system priority level and set the PIC mask according to it	     *
 *===========================================================================*/
PUBLIC void MRT_prtylvl_set(priority)
int priority;
{
	MRT_sv.prtylvl = priority;
	if( TEST_BIT(MRT_sv.flags,MRT_PRTYMASK))
		MRT_set_PIC( ~(MRT_si.mask[priority]) | MRT_sv.real_PIC);				
}

/*===========================================================================*
 *				rtk_set2rt							     *
 * Converts a NRT-process  into a RT-process after calling MRT_setproc	     *
 *===========================================================================*/
PUBLIC int rtk_set2rt(parm_ptr)
void *parm_ptr;
{
	struct proc *rp;
	MRT_vtimer_t vt, *pvt1, *pvt2;
	MRT_msgQ_t mq, *pmq;

	rp = proc_ptr;

	if(MRT_is_realtime(rp)) 		/* check if the RT-process 			*/
		return(E_MRT_BADPTYPE);		/* attributes are NOT loaded 			*/
	if(MRT_st.freeQ.inQ == 0)  		/* check for free VTs				*/
		return(E_MRT_NOVTIMER);
	if(MRT_st.freeQ.inQ == 1 && MRT_is_periodic(rp)) /* check free VT for periodic*/
  		return(E_MRT_NOVTIMER);
	if(MRT_sm.mqeL.inQ == 0)		/* check for free MQ				*/
		return(E_MRT_NOMSGQ);

	/* alloc a MQ for the process */
	mq.owner   = rp->p_nr;
	mq.flags   = rp->mq.flags;
	mq.size    = rp->mq.size;
	if ((rp->pmq = MRT_msgQ_alloc(&mq)) == NULL)
		panic("rtk_set2rt: a MQ must be free for proc", rp->p_nr);

	/* alloc a VT for IPC */
	VTALLOC(rp->ipcvt, vt, 1, 0, MRT_ACT_NONE, 0
		,rp->p_nr, rp->rt.priority);
	if( rp->ipcvt == NULL) /* allocs a VT for IPC */
		panic("rtk_set2rt: a VT for IPC must be free for proc", rp->p_nr);

	/* alloc a VT for a periodic process */
	if(MRT_is_periodic(rp))				/* for PERIODIC RT-processes		*/
		{
		VTALLOC(rp->pvt, vt, rp->rt.period, rp->rt.limit, MRT_ACT_PERIODIC, 0
			,rp->p_nr, rp->rt.priority);
		if( rp->pvt = NULL) 			
			panic("rtk_set2rt: a VT for periodic process must be free", rp->p_nr);
		SET_BIT(rp->rt.flags,MRT_P_SLEEP); 	/* Put the process to sleep		*/
		MRT_vtimer_ins(rp->pvt); 		/* vtimer set				*/
		}
	
	lock_unready(rp);					/* removes proc from MINIX readyQ 	*/
	SET_BIT(rp->p_flags,MRT_P_REALTIME);	/* set MINIX flags to avoid ready() */
	MRT_ready(rp);					/* inserts into a RT ready queue   	*/

	return(OK);
}

/*===========================================================================*
 *				rtk_set2nrt							     *
 * Convert a RT-process into a NRT-process before exit or signal		     *
 *===========================================================================*/
PUBLIC int rtk_set2nrt(parm_ptr)
void *parm_ptr;
{
	return(MRT_set2nrt(proc_ptr)); 
}

/*===========================================================================*
 *				MRT_set2nrt							     *
 *===========================================================================*/
PUBLIC int MRT_set2nrt(rp)
struct proc *rp;
{
	int i,p_nr;
	MRT_vtimer_t *pvt;
	MRT_msgQ_t *pmq;
	MRT_mqe_t *pmqe;
	MRT_proc_t	*rtp;
	struct proc *src_ptr;


	if( !MRT_is_realtime(rp)) 
		return(E_MRT_BADPTYPE);

	if(MRT_is_rtready(rp)) 	
		MRT_unready(rp);	

	if( rp->pvt != NULL) 				/* stops and free VT		*/
		{
		MRT_vtimer_stop(rp->pvt);
		MRT_vtimer_free(rp->pvt);
		rp->pvt = NULL;
		}
	
	if( rp->ipcvt != NULL)				/* stops and free IPC VT	*/
		{
		MRT_vtimer_stop(rp->ipcvt);
		MRT_vtimer_free(rp->ipcvt);
		rp->ipcvt = NULL;
		}

	/* A message is sent to the terminating RT-process watchdog. */
	rtp = proc_addr(rp->rt.watchdog);
	if( rp->rt.watchdog != HARDWARE && MRT_is_realtime(rtp))
		MRT_ksend(rp->p_nr,rp->rt.watchdog
				,rp->rt.priority,MT_KRNPROC);				

	/* A message is sent to all process with the watchdog field 		*/
	/* equals to the terminating RT-process. The watchdog fields are set 	*/
	/* to HARDWARE.										*/
	for ( i = 0, p_nr = -NR_TASKS;  i < NR_PROCS ; i++, p_nr++) 
		{
		rtp = proc_addr(p_nr);
 		if( rp->p_nr == rtp->rt.watchdog && MRT_is_realtime(rtp))
			{
			rtp->rt.watchdog = HARDWARE;
			MRT_ksend(rp->p_nr,rtp->p_nr,rtp->rt.priority,MT_KRNWDOG);				
			}
		}

	/* All RT-interrupt descriptors watchdog fields equals to the 		*/
	/* terminating RT-process are set to HARDWARE.					*/
  	for (i = 0; i < (NR_IRQ_VECTORS+NR_IRQ_SOFT) ; i++)
		{
 		if( rp->p_nr == MRT_si.irqtab[i].watchdog)
			MRT_si.irqtab[i].watchdog = HARDWARE;
		}

	/* The RT-kernel removes all messages into its MQ 				*/
	/* and unlocks all process with synchronous message waiting in the MQ   */
	/* of terminating RT-process.								*/
	if( rp->pmq != NULL)
		{
		while( (pmqe = MRT_mqe_pick(rp->pmq, MRT_ANYPROC)) != NULL) 
			{
			src_ptr = proc_addr(pmqe->msgd.hdr.src.p_nr);
			if( src_ptr->ipcvt != NULL)
				MRT_vtimer_stop(src_ptr->ipcvt);
			CLR_BIT(src_ptr->rt.flags,MRT_MBX_SND);
			src_ptr->pto 	= NULL;
			if(MRT_is_rtready(src_ptr)) MRT_ready(src_ptr);
			src_ptr->p_reg.retreg = E_MRT_EXITING;
			}
		MRT_msgQ_free(rp->pmq);
		rp->pmq = NULL;
		}

	/* All asyncronous messages sent by the terminating RT-process to other */
	/* message queues are removed.							*/
	for ( i = 0;  i < NR_MESSAGES ; i++)
		{ 
		pmqe = mqe_addr(i);
		if( pmqe->msgd.hdr.src.p_nr == rp->p_nr)
			{
			rtp = proc_addr(pmqe->msgd.hdr.dst.p_nr);	/* destination processs	*/
			MRT_mqe_rmv(rtp->pmq, pmqe); 	/* remove the mqe from its MQ		*/
			MRT_mqe_rst(pmqe);
			MRT_mqe_free(pmqe);
			}
		}

	if( rp->slockL.inQ != 0)			/* Has the process lock semaphores?	*/
		MRT_sem_unlock(rp);			/* unlock waiting processes		*/

	if( rp->sallocL.inQ != 0)			/* Has the process own semaphores?	*/
		MRT_sem_empty(rp);			/* unlock waiting processes		*/

	rp->rt.flags = 0;					/* No more RT process			*/
	CLR_BIT(rp->p_flags,MRT_P_REALTIME);	/* Clean bits used for RT processes */
	if(rp->p_flags == 0) lock_ready(rp);	/* now is a NRT process			*/
	return(OK);
}

/*===========================================================================*
 *				MRT_sleep  							     *
 * Sets the process state to RT-BLOCKED							* 
 *===========================================================================*/
PUBLIC int MRT_sleep(rp,timeout)
struct proc *rp;
long int timeout;
{
	if( !MRT_is_realtime(rp)) 			/* check for RT process		*/	
		return(E_MRT_BADPTYPE);

	if( timeout == MRT_NOWAIT)			/* Sleep for nothing ??		*/
		return(OK);
	
	if( timeout != MRT_FOREVER)			/* sleep for a period		*/
		{
		rp->ipcvt->nextexp= 0;
		rp->ipcvt->flags 	= VT_ALLOC;
		MRT_link_rst(&rp->ipcvt->link, &rp->ipcvt, MRT_OBJVT);
		VTSET(rp->ipcvt, timeout, 1, MRT_ACT_WAKEUP, 0, rp->rt.priority);
		}

	SET_BIT(rp->rt.flags,MRT_P_SLEEP);		
	MRT_unready(rp);
	return(OK);
}

/*===========================================================================*
 *				rtk_sleep  							     *
 * Sets the calling process state to RT-BLOCKED						* 
 *===========================================================================*/
PUBLIC int rtk_sleep(parm_ptr)
void *parm_ptr;
{
	long int timeout;
	phys_bytes user_phys;

	USR2KRN( proc_ptr, &timeout, parm_ptr, sizeof(long int));
	return(MRT_sleep(proc_ptr,timeout));
}

/*===========================================================================*
 *				rtk_wakeup							     *
 * Sets the state of a blocked process to RT-READY and inserts it in a  	*
 * priority  Queue 										*
 *===========================================================================*/
PUBLIC int rtk_wakeup(parm_ptr)
void *parm_ptr;
{
	struct proc *rp;
	mrtpid_t mrtpid;
      int rcode;
	phys_bytes user_phys;

	USR2KRN( proc_ptr, &mrtpid, parm_ptr, sizeof(mrtpid_t));

	rp = proc_addr(mrtpid.p_nr);
	if( !MRT_is_process(rp)) 		return(E_BAD_PROC);
	if ( rp->p_pid != mrtpid.pid) 	return(E_BAD_PROC);
	if( !MRT_is_realtime(rp)) 		return(E_MRT_BADPTYPE);

	return(MRT_wakeup(rp));
}

/*===========================================================================*
 *				MRT_wakeup							     *
 *===========================================================================*/
PUBLIC int MRT_wakeup(rp)
struct proc *rp;
{

	MRT_vtimer_stop(rp->ipcvt);			/* stop and removes de VT	*/

	/* Check the SLEEP flag to see if the process is sleeping */
	if( !TEST_BIT(rp->rt.flags,MRT_P_SLEEP))	/* Is the process sleeping?	*/
		{						/* The process is not sleeping*/
		if( rp->rt.watchdog != HARDWARE)	/* if it has a watchdog proc	*/
			MRT_ksend(proc_ptr->p_nr, rp->rt.watchdog,rp->rt.priority,MT_KRNDLINE);
		return(E_MRT_UNSLEEP);			/* sends it a Deadline message*/
		}

	CLR_BIT(rp->rt.flags,MRT_P_SLEEP);		/* Removes sleep flag		*/

	if( !MRT_is_rtready(rp) )  			/* Can be ready?			*/
		return(E_MRT_UNREADY);			/* The process must still unready */

	MRT_ready(rp);					/* insert the process into the*/
	return(OK);						/* ready queue			*/
}

/*===========================================================================*
 *				rtk_print							     *
 *===========================================================================*/
int rtk_print(parm_ptr)
void *parm_ptr;
{
	char string[MAXPRTSTR];
	struct proc *rp;
	phys_bytes user_phys;

	rp = proc_ptr;
	USR2KRN( rp, &string, parm_ptr, MAXPRTSTR);
	printf("%s",string);
	return(OK);
}

/*===========================================================================*
 *				rtk_semup  							     *
 * Ups a Semaphore										*
 *===========================================================================*/
PUBLIC int rtk_semup(parm_ptr)
void *parm_ptr;
{
	int semid;
	phys_bytes user_phys;

	USR2KRN( proc_ptr, &semid, parm_ptr, sizeof(int));
	return(MRT_semup(sem_addr(semid)));
}

/*===========================================================================*
 *				rtk_semdown							     *
 * Downs a Semaphore										*
 *===========================================================================*/
PUBLIC int rtk_semdown(parm_ptr)
void *parm_ptr;
{
	mrt_down_t dw;
	phys_bytes user_phys;

	USR2KRN( proc_ptr, &dw, parm_ptr, sizeof(mrt_down_t));
	return(MRT_semdown(sem_addr(dw.index), dw.timeout));
}

/*===========================================================================*
 *                                   MRT_idle_init                           *
 *===========================================================================*/
PUBLIC void MRT_idle_init(void)
{
	MRT_sv.idlecount			= 0;
	MRT_sv.counter.idlelast 	= 0;
	MRT_sv.counter.idlemax		= 1;
}

/*===========================================================================*
 *                                   MRT_idle 		                       *
 *===========================================================================*/
PUBLIC void MRT_idle(void)
{
	MRT_idle_init();
	while(TRUE)
		MRT_sv.idlecount++;
}

/*===========================================================================*
 *                               MRT_idle_handler	                       *
 *===========================================================================*/
PUBLIC int MRT_idle_handler(irq)
int irq;
{
	MRT_sv.counter.idlelast  = MRT_sv.idlecount; /* the last period count   */ 
	MRT_sv.idlecount =  0;			/* Reset the counter			*/
	MRT_sv.counter.idlemax 
		= MAX(MRT_sv.counter.idlelast,MRT_sv.counter.idlemax); /* maximum */
}

#endif /* MRT */

