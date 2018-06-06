/*==========================================================================*
 *					mrtvt.c						    *
 * This file contains essentially all of the Virtual Timers handling	    *
 *==========================================================================*/
#include "kernel.h"
#include <signal.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include "proc.h"

#ifdef MRT

#define CONTINUE	0
#define ONE_SHOT        0x30 /* ccaammmb, a = access, m = mode, b = BCD */
				/*   00 = chanel 0, 11 = LSB then MSB, x00 = one_shot, 0 = no_BCD */
#define SQUARE_WAVE     0x36	/* ccaammmb, a = access, m = mode, b = BCD */
				/*   11x11, 11 = LSB then MSB, x11 = sq wave */
#define SQUARE_WAVE2    0xB6	/* ccaammmb, a = access, m = mode, b = BCD */
				/*   11x11, 11 = LSB then MSB, x11 = sq wave */
#define LATCH_COUNT      0x00	/* cc00xxxx, c = channel, x = any */
#define LATCH_COUNT2     0x80	/* cc00xxxx, c = channel, x = any */


FORWARD _PROTOTYPE( int MRT_act_none, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_sched, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_msgown, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_msgwdog, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_irqtrig, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_sndto, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_rcvfr, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_wakeup, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_debug, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_sndrcv, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_stop, (MRT_vtimer_t *pvt) );
FORWARD _PROTOTYPE( int MRT_act_down, (MRT_vtimer_t *pvt) );

_PROTOTYPE (int (*MRT_vtaction[]),(MRT_vtimer_t *pvt)) = {
	MRT_act_none, 	/*MRT_ACT_NONE  	0 to exectute by a virtual timer: none 	*/
	MRT_act_sched, 	/*MRT_ACT_SCHED 	1 to exectute by a virtual timer: schedule*/
	MRT_act_msgown,	/*MRT_ACT_MSGOWN 	2 to send a message to the vtimer owner 	*/
	MRT_act_msgwdog,  /*MRT_ACT_MSGWDOG	3 to send a message to the process in param field */
	MRT_act_irqtrig,  /*MRT_ACT_IRQTRIG 4 to trigger and IRQ 				*/
	MRT_act_sndto, 	/*MRT_ACT_SNDTO 	5 Send time out					*/
	MRT_act_rcvfr,	/*MRT_ACT_RCVFR 	6 Receive time out				*/
	MRT_act_wakeup,	/*MRT_ACT_WAKEUP 	7 Wakeup a slept process 			*/
	MRT_act_debug, 	/*MRT_ACT_DEBUG 	8  Print the param field 			*/
	MRT_act_wakeup,   /*MRT_ACT_PERIODIC 9 Sched a periodic process idem WAKEUP	*/
	MRT_act_sndrcv,	/*MRT_ACT_SNDRCV 	10 Send Request/Receive reply time out	*/
	MRT_act_stop,	/*MRT_ACT_STOP 	11 Block the owner process			*/
	MRT_act_down,	/*MRT_ACT_DOWN 	12 Down timeout 					*/
	};

/*===========================================================================*
 *				MRT_timer_set						     *
 *===========================================================================*/
PUBLIC void MRT_timer_set(harmonic)
unsigned int	harmonic;
{
/* Initialize channel 0 of the 8253A timer to e.g. 60 Hz. */
  	MRT_reset_irq(CLOCK_IRQ);

	/* computes the value for the PIT latch counter */
	MRT_sv.harmonic = harmonic;
   	MRT_sv.tickrate = MRT_sv.harmonic * HZ;
  	MRT_sv.PIT_latch = TIMER_FREQ/MRT_sv.tickrate;
		
	/************** TIMER0 is used to generate Timer IRQs   *****************/
	out_byte(TIMER_MODE, SQUARE_WAVE);		/* set PIT to run continuously*/
      out_byte(TIMER0, MRT_sv.PIT_latch);		/* load PIT low byte 		*/
      out_byte(TIMER0, MRT_sv.PIT_latch >> 8);	/* load PIT high byte 		*/

  	MRT_set_irq(CLOCK_IRQ);			/* ready for clock interrupts 	*/
}

/*===========================================================================*
 *				MRT_timer_oneshot						     *
 * periods are in pseudo ticks units						     *
 *===========================================================================*/
PUBLIC void MRT_timer_oneshot(period)
int 	period;	/* in virtual ticks  [vtick] = 1/(HZ*harmonic) */
{
	unsigned int latch;
	int error, realhz;

	MRT_st.T2curr = MRT_read_timer2();
	MRT_st.latch = (MRT_sv.tick2Hz * period);

	if(MRT_sv.counter.oneshots == 0) 
		error = 0;
	else
		{
		realhz  =  0xFFFF - MRT_st.T2curr;
		error   = (realhz  - MRT_st.latch); 
		if( MRT_st.latch < error) error = 0; 	/* overrun				*/
		}

	latch =  (MRT_st.latch - error)>>1; 		/* [Hz]				*/

	MRT_timer2_set(0xFFFF);
	out_byte(TIMER_MODE, ONE_SHOT);			/* set timer to run ONE SHOT 	*/
      out_byte(TIMER0, (char)(latch & 0x00FF));		/* load timer low byte 		*/
      out_byte(TIMER0, (char)((latch >> 8)& 0x00FF));	/* load timer high byte 	*/
	MRT_sv.counter.oneshots++;
	MRT_set_irq(CLOCK_IRQ);
}

/*===========================================================================*
 *				MRT_timer2_set						     *
 *===========================================================================*/
PUBLIC void MRT_timer2_set(latch)
unsigned int	latch;
{
	/************** TIMER2 is used to compute the latency *****************/
	out_byte(PORT_B, 0x01);

	out_byte(TIMER_MODE, SQUARE_WAVE2);	/* set TIMER2 to run continuously */
      out_byte(TIMER2, latch);		/* load timer low byte */
      out_byte(TIMER2, latch >> 8);		/* load timer high byte */
}

/*===========================================================================*
 *				MRT_read_timer2						     *
 * Reads the counter of the channel  of the Programmable Interrupt Timer     * 
 *===========================================================================*/
PUBLIC unsigned int MRT_read_timer2(void)
{
	unsigned int  count;

	out_byte(TIMER_MODE, LATCH_COUNT2);	
  	count = in_byte(TIMER2);		
  	count |=  (in_byte(TIMER2) << 8);

	return(count);
}

/*===========================================================================*
 *				MRT_clock_handler						     *
 * It is executed at INTERRUPT time only if at least VT has expired	     *
 * It changes the expired Vtimers from the Active Queue to the Expired Queue *
 * INTERRUPTS COME DISABLED								     * 
 *===========================================================================*/
PUBLIC int MRT_clock_handler( irq)
int  	irq;
{
	MRT_vtimer_t *vt;
	MRT_actQ_t	*tq;
	link_t	*plink;
	lcounter_t minexp;

	tq = &MRT_st.actQ;
	minexp = MRT_MAXPER;

	/* swap expired VTs from ActiveQ to ExpiredQ */
	for( plink = tq->list.first;  plink != NULL ; plink = tq->list.first)
		{
		if( tq->earliest != 0) break;

		vt = plink->this;
		if( TEST_BIT(MRT_sv.flags,MRT_ONESHOT))
			if( ((vt->expired+1) < vt->limit) || (vt->limit == 0))
				/* these expire VTs will be reinserted in the expiredQ 	*/
				/* after executing their actions, therefore, we need their  */
				/* periods to get the smallest among expired VTs and Active */
				/* VTs to set the PIT latch for the next Timer Interrupt	*/ 
				minexp = MIN(minexp,vt->period);

		MRT_vtimer_rmv(vt);				/* removes from Active Queue */
		MRT_plist_app(&MRT_st.expQ, plink, vt->priority); /* appends to expired*/
		}

	if( TEST_BIT(MRT_sv.flags,MRT_ONESHOT) )
		{
		if( plink != NULL) 		
			{
			vt = plink->this;
			minexp = MIN(minexp,vt->period); /* The smallest period		*/
			}
		MRT_timer_oneshot(minexp);	
		}

	/************* FALTA COMPUTAR LATENCIA ver MRT_do_handler  ******************/

	return(MRT_IRQ_ENABLED);
}


/*===========================================================================*
 *				MRT_timerQ_init						     *
 *===========================================================================*/
PUBLIC void MRT_timerQ_init(void )
{
	int i;

	MRT_st.actQ.earliest	= 0;
	MRT_st.actQ.maxper 	= 0;
	MRT_list_rst(&MRT_st.actQ.list);	/* Resets Active VT  list 		*/

	MRT_list_rst(&MRT_st.freeQ);		/* Resets Free VT list			*/
	MRT_plist_rst(&MRT_st.expQ);		/* Resets Expired VT priority list	*/
}

/*===========================================================================*
 *				MRT_vtimer_init						     *
 * Initialize all virtual timer descriptors					     * 
 *===========================================================================*/
PUBLIC void MRT_vtimer_init(void )
{
	int i;
	MRT_vtimer_t *pvt;

	for ( i = 0;  i < NR_VTIMERS ; i++) 
		{
		MRT_st.vtimer[i].index = i;		/* set the VT index		*/		
		pvt = vtimer_addr(i);			
		MRT_vtimer_rst(pvt);			/* resets all fields of a VT  */
		MRT_list_app(&MRT_st.freeQ,&pvt->link);/* appends VT into FreeQ 	*/
		}
}

/*===========================================================================*
 *				MRT_vtimer_rst						     *
 * Reset all fields of a virtual timer descriptor				     *
 *===========================================================================*/
PUBLIC void MRT_vtimer_rst(pvt)
MRT_vtimer_t *pvt;
{
	pvt->period 	= 0;
	pvt->limit 		= 0;
	pvt->action 	= MRT_ACT_NONE;			
	pvt->param 		= 0;

	pvt->owner 		= HARDWARE;
	pvt->priority 	= MRT_PRILOWEST;
	pvt->timestamp 	= 0;

	pvt->nextexp 	= 0;
	pvt->flags 		= VT_ALLOC;
	MRT_link_rst(&pvt->link, (void *)pvt, MRT_OBJVT);
}

/*===========================================================================*
 *				MRT_vtimer_free						     *
 * frees a virtual timer descriptor							     *
 *===========================================================================*/
PUBLIC void MRT_vtimer_free(pvt)
MRT_vtimer_t *pvt;
{
	MRT_vtimer_stop(pvt);			/* removes from Active/ExpiredQ 	*/
	MRT_vtimer_rst(pvt);			/* resets VT fields			*/
	pvt->flags 		= VT_FREEQ;
	MRT_list_app(&MRT_st.freeQ, &pvt->link);/* appends VT into FreeQ 		*/
}

/*=========================================================================== *
 *				MRT_vtimer_alloc						      *
 * Allocate a VT with the parameters passed by the function argument          *
 *===========================================================================*/
PUBLIC MRT_vtimer_t *MRT_vtimer_alloc( vt)
MRT_vtimer_t *vt;
{
	int i,p, vterr = 0;
	MRT_vtimer_t *vv;
	link_t *plink;
	lcounter_t	ticks;
	struct proc *rp;

	if( MRT_st.freeQ.inQ == 0) return(NULL);		/* check for a free VT		*/		
	if( vt->period <= 0 || vt->period > MRT_MAXPER) return(NULL);
	if( vt->limit  < 0)					return(NULL);
	if( vt->action < 0 || vt->action > MRT_NBRVTACT)return(NULL);

	plink = MRT_st.freeQ.first;				/* gets the first VT in FreeQ	*/
	MRT_list_rmv(&MRT_st.freeQ, plink);

	vv = plink->this;
	vv->owner = vt->owner;					/* assign the owner process	*/
	vv->priority = vt->priority;				/* specified priority for IRQs*/
	vv->period = vt->period;
	vv->limit  = vt->limit;
	vv->action = vt->action;
	vv->param  = vt->param;	
	vv->flags  = VT_ALLOC;					/* set the status of the VT	*/
	return(vv);
}

/*===========================================================================*
 *				MRT_vtimer_stop						     *
 * Removes a VT from the Active TimerQ/ExpiredQ and clears some VT's fields  *
 *===========================================================================*/
PUBLIC void MRT_vtimer_stop(pvt)
MRT_vtimer_t *pvt;
{
	switch(pvt->flags)
		{
		case VT_FREEQ:
			panic("MRT_vtimer_stop: VT is free",pvt->index);
			break;
		case VT_ACTIVEQ:					/* removes from the ActiveQ	*/
			MRT_vtimer_rmv(pvt);
			break;
		case VT_EXPIREDQ:					/* removes from the ExpiredQ	*/
			MRT_plist_rmv(&MRT_st.expQ, &pvt->link, pvt->priority);
			break;
		default:
			break;
		}
	pvt->nextexp 	= 0;
	pvt->flags 		= VT_ALLOC;
	MRT_link_rst(&pvt->link, (void *) pvt, MRT_OBJVT);
}

/*===========================================================================*
 *				MRT_vtimer_ins						     *
 * Inserts an active VT into the Active Queue				 	     *
 *===========================================================================*/
PUBLIC void MRT_vtimer_ins(pvt)
MRT_vtimer_t *pvt;
{
	MRT_vtimer_t *pvtprev, *pvtnext;
	link_t	*plnext,*plprev;
	long ticksum;
	MRT_actQ_t *ptq;

	ptq = &MRT_st.actQ;
	pvt->flags = VT_ACTIVEQ;			/* Sets the VT status flag		*/

	if(ptq->list.first == NULL)			/* The active Queue is empty		*/
		{
		ptq->earliest 	= pvt->period;
		ptq->maxper		= 0;
		pvt->nextexp 	= 0;
		MRT_list_ins(&ptq->list, &pvt->link);
		}
	else if (ptq->earliest > pvt->period)	/* The new VT is the first 		*/
		{
		pvt->nextexp 	= ptq->earliest - pvt->period;
		ptq->maxper		+= pvt->nextexp;
		ptq->earliest 	= pvt->period;
		MRT_list_ins(&ptq->list, &pvt->link);
		}
	else if ((ptq->maxper + ptq->earliest) <= pvt->period)/* The new VT is the last */
		{
		plprev = ptq->list.last;
		pvtprev = (MRT_vtimer_t *) plprev->this;
		pvtprev->nextexp = pvt->period - (ptq->maxper + ptq->earliest);
		ptq->maxper 	= pvt->period - ptq->earliest;
		pvt->nextexp 	= 0;
		MRT_list_app(&ptq->list, &pvt->link);
		}
	else							/* in the middle of the queue 	*/
		{					
		for ( ticksum = ptq->earliest, plnext = ptq->list.first;
		      ticksum <= pvt->period && plnext->next != NULL;
		      plnext = plnext->next)
				{
				pvtnext = (MRT_vtimer_t *) plnext->this;
				ticksum += pvtnext->nextexp;		/* computes cumulated deadline*/
				}

		pvt->nextexp 	= ticksum - pvt->period;	/* computes next exp for VT 	*/

		plprev  = plnext->prev;				
		pvtprev = (MRT_vtimer_t *) plprev->this;		/* recomputes the next exp	*/
		pvtprev->nextexp -= pvt->nextexp;			/* for the prev VT in the list*/
										
		pvt->link.next    = plnext;				/* current VT next link 	*/
		pvt->link.prev 	= plprev;				/* current VT prev link		*/

		plprev->next = &pvt->link;				/* prev VT next link		*/
		plnext->prev = &pvt->link;				/* next VT prev link		*/

		ptq->list.inQ++;				
		ptq->list.maxinQ = MAX(ptq->list.inQ,ptq->list.maxinQ);
		}


/* OJO!!!!!!!!!!!!!: SI ESTA EN ONE-SHOT, ENTONCES HAY QUE RECALCULAR EL TIEMPO 	*/
/* SI ES EL PRIMERO DE LOS TIMERS!!!!								*/
}	 

/*===========================================================================*
 *				MRT_vtimer_rmv						     *
 * Removes a VT from the Active Queue 						     *
 *===========================================================================*/
PUBLIC void MRT_vtimer_rmv(pvt)
MRT_vtimer_t	*pvt;
{
	MRT_vtimer_t *pvtprev;
	link_t	*plprev;
	MRT_actQ_t  *ptq;

	ptq = &MRT_st.actQ;

	/* remove the vtimer descriptor from the timerQ */

	if( ptq->list.first == &pvt->link 
		&& ptq->list.last == &pvt->link)			/* the only			*/
		{
		ptq->earliest = 0;
		ptq->maxper = 0;
		}
	else if (ptq->list.first == &pvt->link)			/* the first		*/
		{
		ptq->earliest += pvt->nextexp;
		ptq->maxper -= pvt->nextexp;
		}
	else if (ptq->list.last == &pvt->link)			/* the last			*/
		{
		plprev = pvt->link.prev;
		pvtprev = (MRT_vtimer_t *) plprev->this;
		ptq->maxper -= pvtprev->nextexp;
		}
	else									/* in the middle  	*/
		{
		plprev = pvt->link.prev;
		pvtprev = (MRT_vtimer_t *) plprev->this;
		pvtprev->nextexp += pvt->nextexp;
		}

	MRT_list_rmv(&ptq->list, &pvt->link);			/* remove object from ActQ*/ 

	pvt->flags = VT_ALLOC;						/* sets the VT's status	*/
	MRT_link_rst(&pvt->link, (void *)pvt, MRT_OBJVT);	/* resets VT fields	*/

/* OJO!!!!!!!!!!!!!: SI ESTA EN ONE-SHOT, ENTONCES HAY QUE RECALCULAR EL TIEMPO 	*/
/* SI ES EL PRIMERO DE LOS TIMERS!!!!								*/
}

/*===========================================================================*
 *				MRT_vtimer_run					     	     *
 * A virtual timer action must be executed					     *
 * Must be executed with INTERRUPT DISABLED					     *
 *===========================================================================*/
PUBLIC void MRT_vtimer_run(pvt)
MRT_vtimer_t *pvt;
{

	pvt->timestamp = MRT_sv.counter.ticks;	/* set its timestamp				*/

	if( pvt->action < 0 || pvt->action >= MRT_NBRVTACT)
		panic("MRT_vtimer_run: bad action type for vtimer\n",pvt->index);
	
	if( (*MRT_vtaction[pvt->action])(pvt) == CONTINUE)
		{
		MRT_plist_rmv(&MRT_st.expQ, &pvt->link, pvt->priority);
		pvt->expired++;
 		}

	if( (pvt->expired < pvt->limit) || (pvt->limit == 0))/* expirations limit 		*/
		MRT_vtimer_ins(pvt);		/* inserts the VT into the Active TimerQ		*/
}


/*============================================================================*
 *					MRT_act_none						*
 *		MRT_ACT_NONE  0 to exectute by a virtual timer: none 			*
 *============================================================================*/
PRIVATE int MRT_act_none(pvt)
MRT_vtimer_t *pvt;
{
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_sched						*
 *		MRT_ACT_SCHED 1 to exectute by a virtual timer: schedule 		*
 *============================================================================*/
PRIVATE  int MRT_act_sched(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*dp;
	if( pvt->param != HARDWARE)
		{
		dp = proc_addr(pvt->param);
		if( MRT_wakeup(dp) == OK) 	/* also stops and remove the VT !!  */
			SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);
		return(!CONTINUE);
		}
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_msgown						*
 *		MRT_ACT_MSGOWN 2 to send a message to the vtimer owner 		*
 *============================================================================*/
PRIVATE  int MRT_act_msgown(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp;
	rp = proc_addr(pvt->owner);			/* gets the owner process	*/
	MRT_ksend(HARDWARE, pvt->owner,rp->rt.priority,MT_KRNTOUT);
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_msgwdog						*
 *  		MRT_ACT_MSGWDOG 3 to send a message to the watchdog process  	*
 *============================================================================*/
PRIVATE  int MRT_act_msgwdog(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp;
	rp = proc_addr(pvt->owner);			/* gets the owner process	*/
	MRT_ksend(pvt->owner, rp->rt.watchdog,rp->rt.priority,MT_KRNTOUT);
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_irqtrig						*     *		MRT_ACT_IRQTRIG 4 to trigger and IRQ 					*
 *============================================================================*/
PRIVATE  int MRT_act_irqtrig(pvt)
MRT_vtimer_t *pvt;
{
	MRT_irqd_t 	*d;

	d = irqd_addr(pvt->param);			/* gets the irqd address	*/
	if(!TEST_BIT(d->irqtype,MRT_TDIRQ))		/* Is it a TD interrupt?	*/
		MRT_irqd_enQ(d);				/* ED-Interrupt			*/
	else							/* TD-Interrupt			*/	
		if( TEST_BIT(d->flags,MRT_TDTRIGGER)
		||  TEST_BIT(d->irqtype,MRT_SOFTIRQ))
			MRT_irqd_enQ(d);			/* Has been triggered or SOFT	*/
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_sndto						* 
 *		MRT_ACT_SNDTO 5 Send time out							*
 *============================================================================*/
PRIVATE  int MRT_act_sndto(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp, *dp;
	MRT_mqe_t *pmqe;

	rp = proc_addr(pvt->owner);	/* gets the VT owner				*/
	pmqe = mqe_addr(pvt->param);	/* gets msg queue entry address		*/	
	dp = rp->pto;

	MRT_mqe_rmv(dp->pmq, pmqe);	/* removes the message queue entry		*/
	MRT_mqe_free(pmqe);		/* frees the message queue entry		*/

	MRT_disinherit(dp);		/* rollback destination priority		*/

	CLR_BIT(rp->rt.flags,MRT_MBX_SND);	/* the sender does not wait anymore	*/

	rp->pto    = NULL;	
	if(MRT_is_rtready(rp))		/* Does the sender need to be blocked?	*/
		{ 
		MRT_ready(rp);		/* inserts the sender into a ready queue	*/
		SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);/* signal the kernel to sched	*/
		}

	rp->p_reg.retreg = E_MRT_TIMEOUT;	/* set the sender return code		*/
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_rcvfr						*
 *		MRT_ACT_RCVFR 6	Receive time out						*
 *============================================================================*/
PRIVATE  int MRT_act_rcvfr(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp;

	rp = proc_addr(pvt->owner);			/* gets the  owner		*/

	CLR_BIT(rp->rt.flags,MRT_MBX_RCV);		/* the receiver does not wait */

	rp->pfrom   = NULL;
	if(MRT_is_rtready(rp))		/* Does the sender need to be blocked?	*/	
		{ 
		MRT_ready(rp);		/* inserts the sender into a ready queue	*/
		SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);/* signal the kernel to sched	*/
		}

	rp->p_reg.retreg = E_MRT_TIMEOUT;		/* set the sender return code	*/
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_wakeup						*
 *		MRT_ACT_WAKEUP 7 Wakeup a slept process 					*
 *============================================================================*/
PRIVATE  int MRT_act_wakeup(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp;

	rp = proc_addr(pvt->owner);			/* gets the  owner		*/
	if( MRT_wakeup(rp) == OK) 	/* also stops and remove the VT !!        */
		SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);
	return(!CONTINUE);
}

/*============================================================================*
 *					MRT_act_debug						*
 *		MRT_ACT_DEBUG 8  Print the param field 					*
 *============================================================================*/
PRIVATE  int MRT_act_debug(pvt)
MRT_vtimer_t *pvt;
{
	printf("[%d] ",pvt->param); 
	return(CONTINUE);
}


/*============================================================================*
 *					MRT_act_sndrcv						*
 *		MRT_ACT_SNDRCV 10 Send Request/Receive reply time out			*
 *============================================================================*/
PRIVATE  int MRT_act_sndrcv(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp, *dp;
	MRT_mqe_t *pmqe;

	rp = proc_addr(pvt->owner);			/* gets the  owner			*/

	if(TEST_BIT(rp->rt.flags,MRT_MBX_SND))	/* the process is waiting to send?	*/
		{	
		pmqe = mqe_addr(pvt->param);	
		dp = rp->pto;
		MRT_mqe_rmv(dp->pmq, pmqe); 		/* removes the message queue entry	*/
		MRT_mqe_free(pmqe);			/* frees the message queue entry 	*/
		rp->pto    = NULL;
		CLR_BIT(rp->rt.flags,MRT_MBX_SND);	/* the sender does not wait more	*/
		}

	dp = rp->pfrom;
	MRT_disinherit(dp);				/* rollback destination priority	*/

	CLR_BIT(rp->rt.flags,MRT_MBX_RCV); 		/* the receiver does not wait more	*/

	rp->pfrom   = NULL;
	if(MRT_is_rtready(rp))			/* Does the process need to be blocked?	*/
		{ 
		MRT_ready(rp);			/* inserts the sender into a ready queue	*/
		SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);	/* signal the kernel to sched	*/
		}

	rp->p_reg.retreg = E_MRT_TIMEOUT;		/* set the sender return code		*/
 	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_stop						*
 *		MRT_ACT_STOP 11 to stop a specified process				*
 *============================================================================*/
PRIVATE  int MRT_act_stop(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp;

	rp = proc_addr(pvt->param);
	if( MRT_is_realtime(rp))
		{
		SET_BIT(rp->p_flags,P_STOP);  
		SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);
		}
	return(CONTINUE);
}

/*============================================================================*
 *					MRT_act_down						*
 *		MRT_ACT_DOWN 12	Down time out						*
 *============================================================================*/
PRIVATE  int MRT_act_down(pvt)
MRT_vtimer_t *pvt;
{
	MRT_proc_t	*rp;
	MRT_sem_t *psem;

	rp = proc_addr(pvt->owner);			/* gets the  owner		*/
	psem = sem_addr(pvt->param);			/* gets semaphore address	*/

	MRT_sproc_rmv(psem, rp);			/* removes the process from  	*/
								/* semaphore priority list	*/

	CLR_BIT(rp->rt.flags,MRT_SEM_DOWN);		/* the receiver does not wait */
	if(MRT_is_rtready(rp))		/* Does the sender need to be blocked?	*/	
		{ 
		MRT_ready(rp);		/* inserts the sender into a ready queue	*/
		SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);/* signal the kernel to sched	*/
		}

	rp->p_reg.retreg = E_MRT_TIMEOUT;		/* set the sender return code	*/
	return(CONTINUE);
}


/*============================================================================*
 *				MRT_vtimer_MINIX						     	*
 * Assigns a Virtual Timer to emulate a Real Clock for MINIX kernel		*
 *============================================================================*/
PUBLIC void MRT_vtimer_MINIX(per)
lcounter_t per;
{
	MRT_irqd_t mnxd; 

	/* software IRQ descriptor preparation for virtual MINIX Timer Interrupt 		*/
 	mnxd.rthandler 	= clock_handler;		/* The RT-handler					*/
 	mnxd.nrthandler	= clock_handler;		/* The NRT-handler				*/
 	mnxd.task 		= HARDWARE;			/* The task number to send an int msg 	*/ 
 	mnxd.priority 	= MRT_PRILOWEST;		/* The SOFT IRQ priority			*/
 	mnxd.irqtype 	= (MRT_RTIRQ | MRT_TDIRQ | MRT_SOFTIRQ);	/* IRQ type			*/
 	mnxd.period 	= per;			/* Period 						*/
 	strncpy(mnxd.name, "MINIX-CLOCK",MRT_PRI15);	/* The name of the soft IRQ		*/
 	if ((MRT_sv.MINIX_soft = MRT_softirq_alloc()) < 0)	/* assign a SOFT IRQ		*/
		panic("clock_task: could not set MINIX CLOCK software IRQ", MRT_sv.MINIX_soft);
 	MRT_irqd_set(MRT_sv.MINIX_soft, &mnxd);	/* set the int descriptor 			*/

}

/*============================================================================*
 *				MRT_vtimer_IDLE						     	*
 * Assigns a Virtual Timer Refresh IDLE counters					*
 *============================================================================*/
PUBLIC void MRT_vtimer_IDLE(prty)
int prty;
{
	MRT_irqd_t idled; 

	/* software IRQ descriptor preparation for virtual MINIX Timer Interrupt 		*/
 	idled.rthandler 	= MRT_idle_handler;	/* The RT-handler					*/
 	idled.nrthandler	= MRT_idle_handler;	/* The NRT-handler				*/
 	idled.task 		= IDLE;			/* The task number to send an int msg 	*/ 
 	idled.priority 	= prty;			/* The SOFT IRQ priority			*/
 	idled.irqtype 	= (MRT_RTIRQ | MRT_TDIRQ | MRT_SOFTIRQ);	/* IRQ type			*/
 	idled.period 	= MRT_sv.refresh;		/* Refresh Period					*/
 	strncpy(idled.name, "IDLE-REFRESH",MRT_PRI15);	/* The name of the soft IRQ		*/
 	if ((MRT_sv.IDLE_soft = MRT_softirq_alloc()) < 0)	/* assign a SOFT IRQ		*/
		panic("MRT_vtimer_IDLE: could not set IDEL Refresh soft IRQ", MRT_sv.IDLE_soft);
 	MRT_irqd_set(MRT_sv.IDLE_soft, &idled);	/* set the int descriptor 			*/

}

#endif /* MRT */
