/******************************************************************************/
/*				MRTIRQ.C								*/
/******************************************************************************/

#include "kernel.h"

#ifdef	MRT

#include "glo.h"
#include <minix/com.h>
#include "proc.h"

#define READ_BACK     0xD2		/* Code to read PIT counter 0		*/	
#define READ_BACK2    0xD8		/* Code to read PIT counter 2		*/

FORWARD _PROTOTYPE( int MRT_irqd_flush,(int p)			);
FORWARD _PROTOTYPE( int MRT_vtimer_flush,(int p)		);

typedef char string[MAXPNAME];
string MRT_irqname[NR_IRQ_VECTORS]= {
	"TIMER",		/* IRQ 0  */
	"KEYBOARD",		/* IRQ 1  */
	"PIC-CASCADE",	/* IRQ 2  */
	"SERIAL-PORT-2",	/* IRQ 3  */
	"SERIAL-PORT-1",	/* IRQ 4  */
	"FREE",		/* IRQ 5  */
	"DISKETTE",		/* IRQ 6  */
	"PARALLEL-PORT",	/* IRQ 7  */
	"FREE",		/* IRQ 8  */
	"FREE",		/* IRQ 9  */
	"FREE",		/* IRQ 10 */
	"FREE",		/* IRQ 11 */
	"FREE",		/* IRQ 12 */
	"FREE",		/* IRQ 13 */
	"HARD-DISK",	/* IRQ 14 */
	"FREE"		/* IRQ 15 */
};

/*=========================================================================*
 *				MRT_IRQ_dispatch						   *
 * This is the common Hardware Interrupt Handler pointed by all IRQ vectors*
 * for Timer Interrupts:                                                   *
 *           increase MRT_sv.counter.ticks       				   *
 *           triggers the IRQ descriptor unserviced in the pending queue   *
 * for Event Driven Interrupts:                                            *
 *           runs the handler only if it's priority is greater than the	   *
 *           current interrupted process or                                *
 *           triggers the IRQ descriptor unserviced in the pending queue   *
 * for Timer Driven and Non Real Time Interrupts:                          *
 *           triggers the IRQ descriptor unserviced in the pending queue   *
 * INTERRUPTS COME DISABLED								   *
 *=========================================================================*/
PUBLIC int MRT_IRQ_dispatch(irq)
int irq;
{
	int retval, prtylvl;
	MRT_irqd_t 	*d;

	d = irqd_addr(irq);				/* get the descriptor address			*/
	MRT_reset_irq(irq);				/* reset the irq bit in the PIC and system*/
								/* MRT_sv.real_PIC variable			*/

	MRT_enable_PIC(irq);				/* reenable MASTER and SLAVE PICs		*/

	retval = MRT_IRQ_ENABLED;			/* return to restart() 				*/
	MRT_sv.counter.interrupts++;			/* update the global interrupt counter 	*/
	d->count++;						/* update the irqd interrupt counter 	*/
	d->reenter = MAX(d->reenter,k_reenter);	/* maximun reentrancy level 			*/

	if( TEST_BIT(d->flags,MRT_DISABLED))	/* Is the descriptor disabled?		*/
		return(retval);				/* return to restart 				*/

	if(TEST_BIT(MRT_sv.flags,MRT_LATENCY)  	/* System Latency computation?		*/
	&& TEST_BIT(d->flags,MRT_LATCALC) )		/* Descriptor Latency computation?		*/
		d->before = MRT_read_timer2();	

	
	if ( irq == CLOCK_IRQ)					/* for TIMER Interrupts... 		*/
		/*----------------------------------------------------------------------------*/
		/* 					TIMER IRQ							*/
		/*----------------------------------------------------------------------------*/
		{
		if ( TEST_BIT(MRT_sv.flags,MRT_ONESHOT))	/* ONE_SHOT Mode?				*/
			{
			MRT_st.actQ.earliest = 0;
			MRT_clock_handler(irq);			/* run the handler 			*/
			}
		else
			{
			MRT_sv.counter.ticks++;			/* update the timer int. counter 	*/
			if( MRT_st.actQ.list.first != NULL )/* check if there are Active VTs	*/
				if(--MRT_st.actQ.earliest == 0)/* check for a expired VT		*/
					MRT_clock_handler(irq);	/* run the handler 			*/
			}
		d->timestamp = MRT_sv.counter.ticks;	/* stamp tick counter in the irqd 	*/
		}
	else							
		/*----------------------------------------------------------------------------*/
		/* 					IRQ != TIMER						*/
		/*----------------------------------------------------------------------------*/
		{
		d->timestamp = MRT_sv.counter.ticks;	/* stamp the ticks into the irqd 	*/
		if ( TEST_BIT(d->irqtype,MRT_TDIRQ))
			{
			SET_BIT(d->flags,MRT_TDTRIGGER);	/* Set Timer handler to be triggered*/
			d->shower++;				/* increase the shower counter	*/
			}
		else							/* Event Driven IRQ			*/
			{
			if( d->priority > MRT_sv.prtylvl )	/* lower prty than running prty 	*/
				{					/* enqueue the descrip for flushing */
				MRT_irqd_enQ(d);
				retval = MRT_IRQ_DISABLED; 	/* keep the PIC disabled		*/
				}
			else 
				{
				if( !TEST_BIT(d->irqtype,MRT_RTIRQ) 
				&&   TEST_BIT(MRT_sv.flags,MRT_MINIXCLI))
					{
					MRT_irqd_enQ(d);
					retval = MRT_IRQ_DISABLED; 	/* keep PIC disabled		*/
					}
				else
					{
					/* Is the IRQ descriptor already enqueued? yes = missed deadline */
					if(TEST_BIT(d->flags,MRT_ENQUEUED)) 		
						{						
						d->mdl++;
						if(d->watchdog != HARDWARE )
							MRT_ksend(HARDWARE, d->watchdog
								,d->priority,MT_KRNDLINE); 
						}

					/* Test for a Task locking the descriptor to use a Critical Section */
					if(TEST_BIT(d->flags,MRT_TASKLOCKED)) 
						{
						SET_BIT(d->flags,MRT_HANDWAIT);
						SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);
						retval = MRT_IRQ_DISABLED; 	/* keep PIC disabled		*/
						}
					else
						MRT_do_handler(d);		/* run the handler and more	*/

					if( MRT_sv.flags & MRT_NEEDSCHED )
						lock_pick_proc();		/* call the Scheduler		*/
					}
				}
			}
		}

	if( retval == MRT_IRQ_ENABLED)			/* Does it need to reenable IRQ?	*/
		MRT_set_irq(irq);

	return(retval);						/* return to restart 			*/
}

/*=====================================================================*
 *                          MRT_flush_int    	                       *
 * This code is invoked on return of system calls and int. handlers    *
 * It scans the interrupt pending queue for HARD and SOFT interrupts   *
 * to be run and the expired timer Queue for timer Actions to execute  * 
 * The handler/action is executed only if its priority is greater than *
 * the current system priority level					     *
 *=====================================================================*/
PUBLIC void MRT_flush_int(void)
{
	int p;

	SET_BIT(MRT_sv.flags,MRT_FLUSHLCK);			/* lock multiple access			*/

	p = MRT_PRIHIGHEST + 1;					/* priority one more than TIMER	*/

	CLR_BIT(MRT_sv.flags,MRT_NEWINT);	
						
	while( p <= MRT_sv.prtylvl 				/* run up to the the current system */
		&& (MRT_st.expQ.bitmap != 0 ||		/* priority level or if there aren't*/ 
		    MRT_si.irqQ.bitmap != 0 )) 		/* any VT actions or deferred IRQs  */
		{
		if(TEST_BIT(MRT_st.expQ.bitmap,(1 << p)))	/* Test for a expired VT to run	*/	
			MRT_vtimer_flush(p); 			

		if(TEST_BIT(MRT_si.irqQ.bitmap,(1 << p)))	/* Test for a trigger IRQ to run	*/
			MRT_irqd_flush(p);

		if(TEST_BIT(MRT_sv.flags,MRT_NEEDSCHED))	/* Need to reschedule?			*/
			lock_pick_proc();				/* run the scheduler 			*/

		if(TEST_BIT(MRT_sv.flags,MRT_NEWINT))	/* New Ints have occurred while	*/
			{ 						/* running the current handler?	*/
			p = MRT_PRIHIGHEST + 1; 		/* Reset the priority to restart	*/
			CLR_BIT(MRT_sv.flags,MRT_NEWINT);	/* the loop and clear the bit		*/
			}
		else 	
			p++;						/* next priority				*/
		}

	CLR_BIT(MRT_sv.flags,MRT_FLUSHLCK);			/* unlock multiple access		*/
}

/*=====================================================================*
 *                          MRT_irqd_flush	                       *
 * Runs a irq lists of pending interrupt descriptors of priority pty   *
 *=====================================================================*/
PRIVATE int MRT_irqd_flush(pty)
int pty;
{
	link_t		*plk;
	list_t		*pls;
	MRT_irqd_t  	*d;

	pls = irqQ_addr(pty); 
	for (plk = pls->first; plk != NULL; plk = pls->first)
		{
		d = (MRT_irqd_t *) plk->this;
		if(TEST_BIT(d->flags,MRT_TASKLOCKED)) 
			{
			SET_BIT(d->flags,MRT_HANDWAIT);
			SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);
			}
		else
			{
			MRT_do_handler(d);	/* runs the handler			*/
			MRT_irqd_deQ(d);		/* dequeue the serviced descriptor  */
			}
		}
}

/*=====================================================================*
 *                          MRT_vtimer_flush	                       *
 * INTERRUPTS COME DISABLED							     *
 *=====================================================================*/
PRIVATE int MRT_vtimer_flush(pty)
int pty;
{
	list_t  *pls;
	link_t  *plk;
	MRT_vtimer_t *pvt;

	pls = expQ_addr(pty);
	for (plk = pls->first; plk != NULL; plk = pls->first)
		{
		pvt = (MRT_vtimer_t *)plk->this;					
		MRT_vtimer_run(pvt);			/* also removes the VT from the expQ*/
		}
}

/*=========================================================================*
 *				MRT_do_handler						   *
 * Execute the handler and gets statistics					   *
 * WARNING: The handler is executed with INTERRUPTS ENABLED			   *	
 * return value: 										   *
 *              	MRT_IRQ_DISABLED: keep the IRQ disabled			   *
 *          	MRT_IRQ_ENABLED : enable the IRQ			         *
 * INTERRUPTS COME DISABLED							         *
 *=========================================================================*/
PUBLIC int MRT_do_handler(d)
MRT_irqd_t  	*d;
{
	scounter_t after;
	MRT_proc_t	*rp;
	int retval, prtylvl, PICmask;
	static long int CPUflags;

	/*---------------------------- SAVE PRIORITY AND PIC MASK --------------------------*/
	prtylvl = MRT_sv.prtylvl;			/* Save the current priority level 		*/
	PICmask = MRT_sv.real_PIC;			/* Save the current PIC mask	 		*/
	MRT_prtylvl_set(d->priority);			/* change the current priority level 	*/

	/*---------------------------------- RUN THE HANDLER -------------------------------*/
	SET_BIT(d->flags,MRT_HANDLOCKED);
	MRT_saveFunlock(&CPUflags);	      	/* Save Flags and do STI - enable ints.	*/	
	if (!TEST_BIT(d->irqtype,MRT_RTIRQ))	/* Is is a NRT-interrupt handler?		*/
		retval = d->nrthandler(d->irq);
	else							/* It is a RT-interrupt handler		*/
		{
		retval = d->rthandler(d->irq);	
		if(TEST_BIT(d->irqtype,MRT_ISTMSG))	/* Does the kernel need to send a message */
			MRT_ksend(HARDWARE, d->task,d->priority,MT_KRNIRQ);/* to the  Task?	*/
		}
	MRT_restoreF(&CPUflags);		/* restore flags => CLI 			*/
      CLR_BIT(d->flags,MRT_HANDLOCKED);

	/*------------------------- RESTORE PRIORITY AND PIC MASK --------------------------*/
	MRT_sv.real_PIC=PICmask;

	if( retval == MRT_IRQ_ENABLED)
		{
		CLR_BIT(MRT_sv.real_PIC,(1 << d->irq));
		if(!TEST_BIT(MRT_sv.flags,MRT_PRTYMASK))
			MRT_set_PIC(MRT_sv.real_PIC);				
		}

	MRT_prtylvl_set(prtylvl);	  	/* Restore the priority level and set PIC	*/

	/*------------------------- IRQ  LATENCY COMPUTATION -------------------------------*/
	if(TEST_BIT(MRT_sv.flags,MRT_LATENCY) 	/* Computes the latency ?			*/
	&& TEST_BIT(d->flags,MRT_LATCALC ))		/* for this descriptor  ?  			*/
		{
		out_byte(TIMER_MODE, READ_BACK2);	/* Get PIT timer2 counter			*/
  		after = in_byte(TIMER2);		
		after |= (in_byte(TIMER2) << 8);	
		d->latency = (d->before > after)? 	/* Computes the latency				*/
			(d->before - after): 
			(0xFFFF - after + d->before);
		d->maxlat = MAX(d->latency,d->maxlat); /* Computes the highest latency		*/
		}

	/*------------------------- check for the task waiting the lock --------------------*/
	if(TEST_BIT(d->flags,MRT_TASKWAIT))
		{
		CLR_BIT(d->flags,MRT_TASKWAIT);
		SET_BIT(d->flags,MRT_TASKLOCKED);
		rp = proc_addr(d->task);
		CLR_BIT(rp->rt.flags,MRT_LOCK_WAIT); 
		MRT_ready(rp);
		SET_BIT(MRT_sv.flags,MRT_NEEDSCHED);
		}

	/*------------------------- UPDATE SOFTIRQ STATISTICS ------------------------------*/
	if(TEST_BIT(d->irqtype,MRT_SOFTIRQ))	/* Is it a soft IRQ Descriptor?		*/
		{
		d->count++;					/* update de counter				*/
		d->timestamp = MRT_sv.counter.ticks;/* stamp the time 				*/
		return;
		}

	/*------------------- RESET TIMER DRIVEN IRQ INTERNAL FIELDS  ----------------------*/
	if(TEST_BIT(d->irqtype,MRT_TDIRQ))		/* Is it a Timer Driven Int Descriptor?	*/
		{
		CLR_BIT(d->flags,MRT_TDTRIGGER);	/* reset the TD triggered flag		*/
		d->maxshower = MAX(d->maxshower, d->shower); /* maximun shower computation	*/
		d->shower = 0;				/* reset the shower counter			*/
		}	
}

/*=========================================================================*
 *				MRT_irqd_enQ						   *
 * enqueue the interrupt descriptor to be flushed 		 		   *
 *=========================================================================*/
PUBLIC void MRT_irqd_enQ(d)
MRT_irqd_t  	*d;
{
	scounter_t 		before;

	if(TEST_BIT(d->flags,MRT_ENQUEUED)) 		
		{						
		d->mdl++;
		if(d->watchdog != HARDWARE )
			MRT_ksend(HARDWARE, d->watchdog,d->priority,MT_KRNDLINE); 
		return; 		
		}

	if(TEST_BIT(d->flags,MRT_DISABLED))		/* Is the descriptor disabled?		*/
		return;

	SET_BIT(MRT_sv.flags,MRT_NEWINT);		/* to detect new ints while flushing 	*/

	if( !TEST_BIT(d->irqtype,MRT_RTIRQ)) 	/* if MINIX has disabled interrupts		*/
		{
		if(TEST_BIT(MRT_sv.virtual_PIC,(1 << d->irq))
		|| TEST_BIT(MRT_sv.flags,MRT_MINIXCLI))	
			{
			MRT_list_app(&MRT_si.mnxQ, &d->link);/* enqueue irqd in MINIX pendingQ*/
			return;
			}
		}

	if(TEST_BIT(d->irqtype,MRT_SOFTIRQ)  	/* Is it a soft IRQ descriptor?		*/
	&& TEST_BIT(MRT_sv.flags,MRT_LATENCY) 	/* Want system wide latency computation?	*/
	&& TEST_BIT(d->flags, MRT_LATCALC) ) 	/* want descriptor Latency Computation ?  */
		{
		out_byte(TIMER_MODE, READ_BACK2);	/* Computes the latency				*/
  		before = in_byte(TIMER2);		
		d->before = before | in_byte(TIMER2) << 8;
		}

	SET_BIT(d->flags,MRT_ENQUEUED);		/* Sets the descriptor as enqueued 		*/
	MRT_plist_app(&MRT_si.irqQ, &d->link, d->priority);
}

/*=========================================================================*
 *				MRT_irqd_deQ						   *
 * dequeue the interrupt descriptor when it was serviced 			   *
 *=========================================================================*/
PUBLIC void MRT_irqd_deQ(d)
MRT_irqd_t  	*d;
{
	if( !TEST_BIT(d->flags,MRT_ENQUEUED))
		panic("MRT_irqd_deQ: interrupt descriptor was not enqueued ",d->irq); 

	MRT_plist_rmv(&MRT_si.irqQ, &d->link, d->priority);
	CLR_BIT(d->flags,MRT_ENQUEUED);		/* the descriptor is not enqueued 		*/
	return;				
}

/*=========================================================================*
 *				MRT_softirq_alloc						   *
 * Alloc a software interrupt descriptor                                   *
 *=========================================================================*/
PUBLIC int MRT_softirq_alloc(void)
{
	int i,m;

	for( i = 0; i < NR_IRQ_SOFT; i++)		/* searchs the bitmap of SOFT IRQ in use	*/
		{
		m = 1 << i;					
		if( !TEST_BIT(m,MRT_si.soft_use))
			break;				/* for a FREE descriptor			*/
		}

	if( i == NR_IRQ_SOFT) 
		return(E_MRT_NOSOFTIRQ); 		/* NO FREE DESCRIPTOR 				*/
	
	SET_BIT(MRT_si.soft_use,m);			/* register the soft irq into the bitmap  */

	return(i+NR_IRQ_VECTORS);
}

/*=========================================================================*
 *				MRT_softirq_free						   *
 * Free the software IRQ descriptor                                  	   *
 *=========================================================================*/
PUBLIC void MRT_softirq_free( irq)
int irq;
{
	int i;

	i = irq - NR_IRQ_VECTORS;
	
      if (i < 0 || i > NR_IRQ_SOFT)
	    	panic("invalid soft irq number in MRT_softirq_free",irq);

	CLR_BIT(MRT_si.soft_use,(1<<i));		/* deregister the soft int 			*/

}

/*=========================================================================*
 *				MRT_irqd_set						   *
 * Set the parameters of an interrupt descriptor (HARD and SOFT)           *
 *=========================================================================*/
PUBLIC void MRT_irqd_set(irq, p_irqd)
int irq;
MRT_irqd_t *p_irqd; 
{
	priority_t 		p;
	MRT_irqd_t  	*d;
	static long int CPUflags;
	MRT_proc_t	 	*task_ptr
	
	if(TEST_BIT(p_irqd->irqtype,MRT_SOFTIRQ))
		{
		if (irq < NR_IRQ_VECTORS || irq >= (NR_IRQ_VECTORS+NR_IRQ_SOFT)) 
			panic("MRT_irqd_set: invalid SOFTIRQ number", irq);
		}
	else
		{
		if (irq < 0 || irq >= NR_IRQ_VECTORS) 
			panic("MRT_irqd_set: invalid HARDIRQ number", irq);
		}
	
	if ( irq != 0 && p_irqd->priority <= MRT_PRIHIGHEST)
		{
		printf("MRT_irqd_set: IRQ %d prty (%d) must be > than MRT_PRIHIGHEST\n"
		,irq,p_irqd->priority);
		return;
		}

	if ( p_irqd->priority > MRT_PRILOWEST)
		{
		printf("MRT_irqd_set: IRQ %d prty (%d) must be <= to MRT_PRILOWEST\n"
		,irq,p_irqd->priority);
		return;
		}

	d = irqd_addr(irq);
  
	MRT_saveFlock(&CPUflags);			/* ENTER_CRITICAL_SECTION */

  	if(TEST_BIT(p_irqd->irqtype,MRT_RTIRQ))
		d->rthandler  = p_irqd->rthandler;
  	else
		d->nrthandler = p_irqd->nrthandler;

  	d->flags   	= 0;			
  	d->irq 	= irq;
  	if(p_irqd->task != HARDWARE)		/* if a task is specied, a msg will be sent 	*/
		{					/* to the Interrrupt Service Task on each int 	*/
		task_ptr = proc_addr(p_irqd->task);
		if(!MRT_is_process(taskt_ptr))
			panic("MRT_irqd_set: invalid TASK number ", p_irqd->task);
		if(!MRT_is_realtime(task_ptr))
			panic("MRT_irqd_set: task is not realtime ", p_irqd->task);
		d->task	= p_irqd->task;	
		SET_BIT( p_irqd->irqtype, (p_irqd->irqtype & MRT_ISTMSG));
		}
	else
		CLR_BIT(p_irqd->irqtype, (p_irqd->irqtype & MRT_ISTMSG));


  	d->priority = p_irqd->priority;

	if( irq < NR_IRQ_VECTORS) 		/* Hardware IRQ?						*/
		{
		for( p = MRT_PRIHIGHEST; p < d->priority ; p++)		/* set priority array	*/
			MRT_si.mask[d->priority] |= MRT_si.mask[p]; 	/* of higher prty irqs 	*/
 
		for( p = d->priority ; p <= MRT_PRILOWEST; p++)		/* set priority array  	*/
			SET_BIT(MRT_si.mask[p],(1 << irq));			/* of lower prty irqs	*/
		}

  	d->irqtype  = p_irqd->irqtype;
  	d->period 	= p_irqd->period;
  	strncpy(d->name, p_irqd->name, 15);
  
  	if( irq < NR_IRQ_VECTORS)
		SET_BIT(irq_use,(1 << irq)); 	/* only for Hardware Interrupts 			*/

  	MRT_restoreF(&CPUflags);		/* LEAVE_CRITICAL_SECTION 				*/
}

/*=========================================================================*
 *				MRT_irqd_free						   *
 * Removes a interrupt descriptor from the interrupt queue                 *
 *=========================================================================*/
PUBLIC void MRT_irqd_free(d)
MRT_irqd_t *d; 
{
	static long int CPUflags;

	MRT_saveFlock(&CPUflags);
	if( d->irq >= NR_IRQ_VECTORS)			/* Software irq descriptors 			*/
		MRT_softirq_free(d->irq);
	MRT_irqd_rst(d);
  	MRT_restoreF(&CPUflags);			/* LEAVE_CRITICAL_SECTION 			*/
}

/*===========================================================================*
 *                     MRT_irqQ_init()	                                   *
 * Initialize the IRQ Queues								     * 
 *===========================================================================*/
PUBLIC void MRT_irqQ_init(void)
{
	int i;
	/* initialize the interrupt queues  */

	MRT_plist_rst(&MRT_si.irqQ);
	MRT_list_rst(&MRT_si.mnxQ);

	for (i = 0; i < MRT_NR_PRTY; i++)		/* clear interrupts masks */
		MRT_si.mask[i]		= 0;

}

/*===========================================================================*
 *                     MINIX virtual lock()                                  *
 * Virtual CLI for MINIX (NRT) descriptors					     * 
 *===========================================================================*/
PUBLIC void lock(void)
{
	static long int CPUflags;

	MRT_saveFlock(&CPUflags);		/* ENTER_CRITICAL_SECTION 				*/

	SET_BIT(MRT_sv.flags,MRT_MINIXCLI);	/* This flag avoid flushing NRT int descriptors */

	if( !TEST_BIT(MRT_sv.flags,MRT_RTMODE))/* if the system is running in NRT Mode	*/
		{
		MRT_restoreF(&CPUflags);	/* LEAVE_CRITICAL_SECTION 				*/
		MRT_lock();				/* do the REAL CLI					*/
		}
	else
		MRT_restoreF(&CPUflags);	/* LEAVE_CRITICAL_SECTION 				*/
}

/*===========================================================================*
 *                    MINIX virtual unlock	                             *
 * Virtual STI for MINIX (NRT) descriptors					     * 
 *===========================================================================*/
PUBLIC void unlock(void)
{
	link_t		*plk;
	static long int CPUflags;

	MRT_saveFlock(&CPUflags);		/* ENTER_CRITICAL_SECTION 				*/

	CLR_BIT(MRT_sv.flags,MRT_MINIXCLI);	/* Clears the Virtual CLI bit				*/

	if( TEST_BIT(MRT_sv.flags,MRT_RTMODE))/* Real Time MODE ?					*/
		{
		for( plk = MRT_si.mnxQ.first; plk != NULL; plk = MRT_si.mnxQ.first)
			{
			MRT_list_rmv(&MRT_si.mnxQ, plk); /* get from MINIX queue			*/
			MRT_do_handler(plk->this);	/* run the handler and more			*/
			}
		MRT_restoreF(&CPUflags);	/* LEAVE_CRITICAL_SECTION			   	*/
		}
	else						/* NON REAL TIME MODE					*/
		{
		MRT_restoreF(&CPUflags);	/* LEAVE_CRITICAL_SECTION  				*/
		MRT_unlock();			/* Real CLI							*/
		}
}

/*===========================================================================*
 *                     MINIX virtual enable_irq()                            *
 *===========================================================================*/
PUBLIC void enable_irq(irq)
unsigned irq;
{
	static long int CPUflags;
	MRT_irqd_t  	*d;
	int i;

	MRT_saveFlock(&CPUflags);		/* ENTER_CRITICAL_SECTION 				*/

	CLR_BIT(MRT_sv.virtual_PIC,(1 << irq));	/* Enable the IRQ in the virtual PIC	*/

	if(!TEST_BIT(MRT_sv.flags,MRT_RTMODE) 			/* NON REAL TIME MODE?		*/
	|| !TEST_BIT(MRT_si.irqtab[irq].irqtype,MRT_RTIRQ)) 	/* NRT IRQ Descriptor?		*/
		MRT_set_irq(irq);						/* enable the REAL PIC		*/

	MRT_restoreF(&CPUflags);		/* LEAVE_CRITICAL_SECTION  				*/
}

/*===========================================================================*
 *                    MINIX virtual disable_irq()                            *
 *===========================================================================*/
PUBLIC int disable_irq(irq)
unsigned irq;
{
	static long int CPUflags;

	MRT_saveFlock(&CPUflags);		/* ENTER_CRITICAL_SECTION 				*/

	SET_BIT(MRT_sv.virtual_PIC,(1 << irq));	/* Disable the IRQ in the virtual PIC 	*/

	if(!TEST_BIT(MRT_sv.flags,MRT_RTMODE)			/* NON REAL TIME MODE?		*/
	|| !TEST_BIT(MRT_si.irqtab[irq].irqtype,MRT_RTIRQ))	/* NRT IRQ Descriptor ?		*/
		MRT_set_irq(irq);			 			/* Disable the IRQ in PIC	*/

	MRT_restoreF(&CPUflags);		/* LEAVE_CRITICAL_SECTION  				*/
	return(1);
}

/*===========================================================================*
 *                   MRT_irqd_rst			                             *
 * Reset all fields of an interrupt descriptor					     *
 *===========================================================================*/
PUBLIC void MRT_irqd_rst(d)
MRT_irqd_t  	*d;
{
	int i;

	d->rthandler 	= spurious_irq;
      d->nrthandler	= spurious_irq;
	i = d->irq;

	d->harmonic		= 1;
	d->period 		= 0;
      d->task 		= HARDWARE;
      d->watchdog		= HARDWARE;
      d->priority 	= MRT_PRILOWEST;
      d->pvt 		= NULL;

	MRT_irqd_clr(d);	
	
	if( i <  NR_IRQ_VECTORS)			/* Hardware irq descriptors */
		{
	      d->irqtype  = 0;
      	strncpy(d->name, MRT_irqname[i],15);
       	}
	 else							/* Software irq descriptors */
		{
	      d->irqtype  = MRT_SOFTIRQ;
      	strncpy(d->name, "SOFTIRQ",15);
       	}
}

/*===========================================================================*
 *                   irqd_clrstat		                		           *
 * Clear statistical fields of an interrupt descriptor			     *
 *===========================================================================*/
PUBLIC void irqd_clrstat(d)
MRT_irqd_t  	*d;
{
	 d->count 		= 0;
	 d->maxshower 	= 0;
	 d->mdl	 	= 0;
	 d->timestamp 	= 0;
	 d->maxlat 		= 0;
	 d->reenter		= 0;

}

/*===========================================================================*
 *                   irqd_clrstat			                             *
 * Clear some fields of an interrupt descriptor					     *
 *===========================================================================*/
PUBLIC void MRT_irqd_clr(d)
MRT_irqd_t  	*d;
{
	 irqd_clrstat(d);

	 d->latency 	= 0;
	 d->shower  	= 0;
	 d->before 		= 0;

       d->flags   	= 0;
	 MRT_link_rst(&d->link,d, MRT_OBJIRQD);
}

/*===========================================================================*
 *                   MRT_set_irq			                             *
 *===========================================================================*/
PUBLIC void MRT_set_irq(irq)
unsigned int irq;
{
	CLR_BIT(MRT_sv.real_PIC,(1 << irq));
	MRT_enable_irq(irq);
}

/*===========================================================================*
 *                   MRT_reset_irq			                             *
 *===========================================================================*/
PUBLIC void MRT_reset_irq(irq)
unsigned int irq;
{
	SET_BIT(MRT_sv.real_PIC,(1 << irq));
	MRT_disable_irq(irq);
}

/*=========================================================================*
 *				MRT_irqd_lock						   *
 * Lock the execution of an interrupt handler to allow enter into a 	   *
 * Critical Section     								   *
 *=========================================================================*/
PUBLIC void MRT_irqd_lock(irq)
unsigned int irq;
{
	MRT_irqd_t  	*d;
	static long int CPUflags;
	
	if (irq < NR_IRQ_VECTORS || irq >= (NR_IRQ_VECTORS+NR_IRQ_SOFT)) 
			panic("MRT_irqd_lock: invalid IRQ number", irq);

	d = irqd_addr(irq);			/* get the descriptor address		*/

	MRT_saveFlock(&CPUflags);		/* ENTER_CRITICAL_SECTION 		*/
	if(TEST_BIT(d->flags,MRT_HANDLOCKED)) /* the irqd is locked by the HANDLER */
		{
		SET_BIT(d->flags,MRT_TASKWAIT);
		SET_BIT(proc_ptr->rt.flags,MRT_LOCK_WAIT); /* Set a process flag	*/
								/* indicating that the process*/
								/* is waiting to lock an IRQ	*/
 		MRT_unready(proc_ptr);			/* removes the process from 	*/
								/* from the RT-ready list	*/
		}
	MRT_restoreF(&CPUflags);		/* LEAVE_CRITICAL_SECTION	*/
}

/*=========================================================================*
 *				MRT_irqd_unlock						   *
 * UnLock the execution of an interrupt handler 				   *
 *=========================================================================*/
PUBLIC void MRT_irqd_unlock(irq)
unsigned int irq;
{
	MRT_irqd_t  	*d;
	static long int CPUflags;
	
	if (irq < NR_IRQ_VECTORS || irq >= (NR_IRQ_VECTORS+NR_IRQ_SOFT)) 
			panic("MRT_irqd_lock: invalid IRQ number", irq);

	d = irqd_addr(irq);			/* get the descriptor address		*/

	MRT_saveFlock(&CPUflags);		/* ENTER_CRITICAL_SECTION 		*/
	CLR_BIT(d->flags,MRT_TASKLOCKED);	/* unlock the IRQ				*/
	if(TEST_BIT(d->flags,MRT_HANDWAIT)) /* the irqd handler is waiting 	 */
		{
		CLR_BIT(d->flags,MRT_HANDWAIT); /* clears the waiting flag		*/
		SET_BIT(d->flags,MRT_HANDLOCKED); /* sets the locked flag		*/
		MRT_do_handler(d);		/* run the delayed handler		*/
		}
	MRT_restoreF(&CPUflags);		/* LEAVE_CRITICAL_SECTION	*/
}

#endif	/* MRT */

