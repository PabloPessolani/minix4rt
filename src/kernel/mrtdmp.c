/* This file contains some dumping routines for debugging. */

#include "kernel.h"
#include <minix/com.h>
#include "proc.h"

char *vargv;

#ifdef MRT
#include "unistd.h"

/**************************************************************************
    F1:		MINIX process attributes
    SHIFT-F1: 	RT process attributes
    CTRL-F1:  	RT process statistics

    F2:		MINIX process memory map
    SHIFT-F2: 	RT system wide attributes and statistics
    CTRL-F2: 	Array o PIC Masks

    F4:		RT Hardware IRQ descriptors 
    SHIFT-F4:	RT Software IRQ descriptors 
    CTRL-F4:	Interrupt descriptors pending queues 

    F5: 	  	RT priority Queues

    F6:     	RT message pool (message attributtes)
    SF6:     	RT message pool (message contents)
    CF6:     	Per process message queue 

    F7:           Debug 232 switch
    SF7:          Latency computationi switch

    F8:		RT Virtual timer
    SF8:		RT Virtual timer Queues

    F9:		RT Semaphores
    SF9:		Allocated Semaphores

***************************************************************************/

/*========================================================================*
 *                           MRT_soft_dmp  (Shift F4 key)                 *
 *========================================================================*/
PUBLIC void MRT_soft_dmp()
{
	int i;
	MRT_irqd_t *pirqd;

	printf("\nIRQ TSK WDG -COUNT- MLAT -LAT RE PY TY VT -TSTAMP- MS MD PER FLAG  NAME\n");
	for (i = NR_IRQ_VECTORS ; i < (NR_IRQ_VECTORS+NR_IRQ_SOFT); i++)
		{
		pirqd = irqd_addr(i);
		printf("%3d %3d %3d %7d %4d %4d %2d %2d %2d %2d %8d %2d %2d %4d %4X %s\n",
			pirqd->irq,
			pirqd->task,
			pirqd->watchdog,
			pirqd->count,
			pirqd->maxlat,
			pirqd->latency,
			pirqd->reenter,
			pirqd->priority,
			pirqd->irqtype,
			(pirqd->pvt == NULL)?-1:pirqd->pvt->index,
			pirqd->timestamp,
			pirqd->maxshower,
			pirqd->mdl,
			pirqd->period,
			pirqd->flags,
			pirqd->name);
		}
}

/*========================================================================*
 *                           MRT_irq_dmp  (F4 key)                        *
 *========================================================================*/
PUBLIC void MRT_irq_dmp()
{
	int i;
 	MRT_irqd_t *pirqd;

	printf("\nIRQ TSK WDG -COUNT- MLAT -LAT RE PY TY VT -TSTAMP- MS MD PERD FLAG  NAME\n");
	for (i = 0; i < NR_IRQ_VECTORS; i++)
		{
		pirqd = irqd_addr(i);
		printf("%3d %3d %3d %7d %4d %4d %2d %2d %2d %2d %8d %2d %2d %4d %4X %s\n",
			pirqd->irq,
			pirqd->task,
			pirqd->watchdog,
			pirqd->count,
			pirqd->maxlat,
			pirqd->latency,
			pirqd->reenter,
			pirqd->priority,
			pirqd->irqtype,
			(pirqd->pvt == NULL)?-1:pirqd->pvt->index,
			pirqd->timestamp,
			pirqd->maxshower,
			pirqd->mdl,
			pirqd->period,
			pirqd->flags,
			pirqd->name);
		}
}

/*========================================================================*
 *                           MRT_test  (F9 key)                        *
 *========================================================================*/
PUBLIC void MRT_test()
{

}

/*========================================================================*
 *                           MRT_counter_dmp  (shift F2 key)            *
 *========================================================================*/
PUBLIC void MRT_counter_dmp()
{
	int i;
	char irquse_bin[17];

	printf("\n\nTotal number of interrupts = %-d\n",MRT_sv.counter.interrupts);
	printf("Total number of RT scheds  = %-d\n",MRT_sv.counter.scheds);
	printf("Total number of RT messages= %-d\n",MRT_sv.counter.messages);
	printf("Free messages desc. in Pool= %-d/%-d\n",MRT_sm.mqeL.inQ,NR_MESSAGES);
	printf("Free messages Queues       = %-d/%-d\n",MRT_sm.msgQL.inQ,NR_MSGQ);
   	printf("Total number of RT ticks   = %-d\n",MRT_sv.counter.ticks);


	printf("RT ticks/s                 = %-d\n",MRT_sv.tickrate);
	printf("Total number ONESHOT ticks = %-d\n",MRT_sv.counter.oneshots);
	printf("MINIX ticks/s              = %-d\n",HZ);
	printf("PIT Latency between 2 reads= %-d\n",MRT_sv.PIT_latency);
	printf("System Priority Level      = %-X\n",MRT_sv.prtylvl);

	int2bin(MRT_sv.flags,irquse_bin);
	printf("Processing Mode Flags      = %-s\n",irquse_bin);

	int2bin(MRT_si.hard_use,irquse_bin);
	printf("Bitmap of hard ints in use = %-s\n",irquse_bin);

	int2bin(MRT_si.soft_use,irquse_bin);
	printf("Bitmap of soft ints in use = %-s\n",irquse_bin);

	int2bin(MRT_sv.virtual_PIC,irquse_bin);
	printf("Bitmap of MINIX virtual PIC= %-s\n",irquse_bin);

	int2bin(MRT_sv.real_PIC,irquse_bin);
	printf("Bitmap of the REAL PIC     = %-s\n",irquse_bin);

	int2bin(MRT_get_PIC(),irquse_bin);
	printf("Bitmap of the HARDWARE PIC = %-s\n",irquse_bin);

	if( MRT_sv.counter.idlemax != 0)
	printf("MRT_sv.counter.idlelast    = %-d\n"
             "MRT_sv.counter.idlemax     = %-d\n"
             "CPU Load                   = %-d\%\n"
			,MRT_sv.counter.idlelast
			,MRT_sv.counter.idlemax
			,(100 - ((100*MRT_sv.counter.idlelast)/MRT_sv.counter.idlemax)));

	printf("Idle count refresh time    = %-d\n",MRT_sv.refresh);
	printf("Sizeof mrt_msg_t           = %-d\n",sizeof(mrt_msg_t));

}

/*========================================================================*
 *                           MRT_irqQ_dmp  (Control F4 key)               *
 *========================================================================*/
PUBLIC void MRT_irqQ_dmp()
{
	int i;
	char mask_bin[17];
	link_t		*plk;
	MRT_irqd_t  	*d;

	printf("PRI MASK 5432109876543210 INQ MAX Enqueued Interrupt Descriptors\n");
	for( i=0; i < MRT_NR_PRTY; i++)
		{
		int2bin(MRT_si.mask[i],mask_bin);
		printf("%3d %4X %16s %3d %3d",
			i,
			MRT_si.mask[i],
			mask_bin,
			MRT_si.irqQ.list[i].inQ,
			MRT_si.irqQ.list[i].maxinQ);
		if( MRT_si.irqQ.list[i].inQ == 0) printf("\n");
		else
			{
			for ( plk = MRT_si.irqQ.list[i].first; plk != NULL; plk = plk->next)
				{
				d = (MRT_irqd_t *) plk->this;
				printf("=>[%2d]",d->irq);
				}
			printf("\n");
			}
		}
	printf("MNX ---- ---------------- %3d %3d",
			MRT_si.mnxQ.inQ,
			MRT_si.mnxQ.maxinQ);
	if( MRT_si.mnxQ.inQ == 0) 
		printf("\n");
	else
		{
		for ( plk = MRT_si.mnxQ.first; plk != NULL; plk = plk->next)
			{
			d = (MRT_irqd_t *) plk->this;
			printf("=>[%2d]",d->irq);
			}
		printf("\n");
		}

}

/*========================================================================*
 *                           MRT_rdyQ_dmp  (F5 key)                       *
 *========================================================================*/
PUBLIC void MRT_rdyQ_dmp()
{
	int i;
	char rdyQ_bm_bin[17];
	MRT_proc_t *p;
	link_t	*plk;

	int2bin(MRT_sp.rdyQ.bitmap,rdyQ_bm_bin);
	printf("\nPriority Queues bitmap = %17s\n",rdyQ_bm_bin);

	printf("PRI MAXINQ INQ PROC-LIST\n");
	for( i=0; i < MRT_NR_PRTY; i++)
		{
		printf(" %2d %6d %3d ",
			i,
			MRT_sp.rdyQ.list[i].maxinQ,
			MRT_sp.rdyQ.list[i].inQ);
		if( MRT_sp.rdyQ.list[i].inQ != 0)
			for( plk = MRT_sp.rdyQ.list[i].first; plk != NULL; plk = plk->next)
				{
				p = (MRT_proc_t *) plk->this;
				printf("->[%d]",p->p_pid);
				}
		printf("\n");
		}
	printf("PRI MINIX-PROC-LIST\n");
	for( i=0; i < NQ; i++)
		{
		printf("%2d ",i);
		for( p = rdy_head[i]; p != NULL; p = p->p_nextready)
				printf("->[%d]",p->p_pid);
		printf("\n");
		}
}

/*===========================================================================*
 *				MRT_patt_dmp (Shift F1 Key )  			     *
 *===========================================================================*/
PUBLIC void MRT_patt_dmp()
{
/* Proc table dump Real Time Fields */

  register struct proc *rp;
  static struct proc *oldrp = BEG_PROC_ADDR;
  int n = 0;
  vir_clicks base, limit;

  printf(
         "\nPROC PID FLAGS PRTY BASE PERIOD LIMIT DEAD WDOG GETF SNDT NAME\n");

  for (rp = oldrp; rp < END_PROC_ADDR; rp++) {
	if (rp->p_flags & P_SLOT_FREE) continue;
	if (++n > 20) break;
	printf("%4d %3d %5X %4d %4d %6d %5d %4d %4d %4d %4d %s\n",
	       proc_number(rp),
	       rp->p_pid,
	       rp->rt.flags,
	       rp->rt.priority,
	       rp->rt.baseprty,
		 rp->rt.period,
		 rp->rt.limit,
		 rp->rt.deadline,
		 rp->rt.watchdog,
		 (rp->pfrom == NULL)?-1:rp->pfrom->p_nr,
		 (rp->pto   == NULL)?-1:rp->pto->p_nr,
		 rp->p_name);
  }
  if (rp == END_PROC_ADDR) rp = BEG_PROC_ADDR; else printf("--more--\r");
  oldrp = rp;
}

/*===========================================================================*
 *				MRT_pstat_dmp (Ctrl F1 Key )   			     *
 *===========================================================================*/
PUBLIC void MRT_pstat_dmp()
{
/* Proc table dump Real Time Fields */

  register struct proc *rp;
  static struct proc *oldrp = BEG_PROC_ADDR;
  int n = 0;
  vir_clicks base, limit;

  printf(
         "\nPROC -PID- SCHEDS MDL TSTAMP MSG_SENT MSG_RCVD PVT IPC MQ SEM COMMAND\n");
  for (rp = oldrp; rp < END_PROC_ADDR; rp++) {
	if (rp->p_flags & P_SLOT_FREE) continue;
	if (++n > 20) break;
	printf("%4d %5d %6d %3d %6d %8d %8d %3d %3d %2d %3d %s\n",
	       proc_number(rp),
	       rp->p_pid,
		 rp->st.scheds,
		 rp->st.mdl,
		 rp->st.timestamp,
		 rp->st.msgsent,
		 rp->st.msgrcvd,
		 (rp->pvt == NULL)?-1:rp->pvt->index,
		 (rp->ipcvt == NULL)?-1:rp->ipcvt->index,
		 (rp->pmq == NULL)?-1:rp->pmq->index,
		 rp->sallocL.inQ,
		 rp->p_name);
  }
  if (rp == END_PROC_ADDR) rp = BEG_PROC_ADDR; else printf("--more--\r");
  oldrp = rp;
}

/*===========================================================================*
 *				MRT_sem_dmp	(F9 Key )					     *
 *===========================================================================*/
PUBLIC void MRT_sem_dmp()
{
	/* semaphore table dump */
	static int i = 0;
	MRT_sem_t *psem;

	char vtimer_bin[17];
	printf("\nSEM --VALUE- PTY FLAG OWN CAR --#UPS-- -#DOWNS- INQ- MAXQ NAME \n");

	while ( i < NR_SEM)
		{
		psem = sem_addr(i);
		printf("%3d %8d %3d %4X %3d %3d %8d %8d %4d %4d %s\n",
			i,
			psem->value,
			psem->priority,
			psem->flags,
			psem->owner,
			(psem->carrier == NULL)?-1:psem->carrier->p_nr,
			psem->ups,
			psem->downs,
			psem->plist.inQ,
			psem->plist.maxinQ,
			psem->name
			);
		i++;
		if( i%16 == 0 ) break;
		}
	
	if( i != NR_SEM)
		printf("--more--\r");
	else
		{
		printf("\nFree Semaphores= %d\n",MRT_ss.sfreeL.inQ);
		i = 0;
		}
}

/*===========================================================================*
 *				MRT_psem_dmp (Shift F9 Key )   			     *
 *===========================================================================*/
PUBLIC void MRT_psem_dmp()
{
/* Proc table dump Real Time Semaphores */

  register struct proc *rp;
  static struct proc *oldrp = BEG_PROC_ADDR;
  MRT_sem_t	*psem;
  link_t *lk;
  int n = 0;

  printf(
         "\nPROC -PID- #SEM Allocated_Semaphore_List\n");
  for (rp = oldrp; rp < END_PROC_ADDR; rp++) {
	if (rp->p_flags & P_SLOT_FREE) continue;
	if (++n > 20) break;
	printf("%4d %5d %4d ->",
	       proc_number(rp),
	       rp->p_pid,
		 rp->sallocL.inQ);
	for(lk = rp->sallocL.first; lk != NULL; lk = lk->next)
		{
		psem = (MRT_sem_t	*)lk->this;
		printf("[%d:%s]->",psem->index,psem->name);
		}
	printf("[NULL]\n");
  	}
  if (rp == END_PROC_ADDR) rp = BEG_PROC_ADDR; else printf("--more--\r");
  oldrp = rp;
}


/*===========================================================================*
 *				MRT_vtimer_dmp	(F8 Key )				     *
 *===========================================================================*/
PUBLIC void MRT_vtimer_dmp()
{
/* vtimer table dump */
	static int i = 0;
	MRT_vtimer_t *pvt;
	char vtimer_bin[17];
	printf("\n-VT --PERIOD-- -NEXTEXP-- LIMIT ACT PAR FLAG NR OWN PRTY EXPIRE TIMESTAMP\n");

	while ( i < NR_VTIMERS)
		{
		pvt = vtimer_addr(i);
		printf("%3d %10d %10d %5d %3d %3d %4X %2d %3d %4d %6d %9d\n",
			i,
			pvt->period,		/* Period in ticks		*/
			pvt->nextexp,		/* ticks to next expiration	*/
	
			pvt->limit,			/* expirations to be removed  */
			pvt->action,		/* Action				*/
			pvt->param,			/* Action parameter		*/
			pvt->flags,			/* VT status flags		*/

			pvt->index,			/* VT number 			*/
			pvt->owner,			/* nbr of the owner process	*/
			pvt->priority,		/* VT	priority		 	*/

			pvt->expired,		/* number of vtimer expirations */
			pvt->timestamp		/* timestamp of the last expire */
			);
		i++;
		if( i%16 == 0 ) break;
		}
	
	if( i != NR_VTIMERS)
		printf("--more--\r");
	else
		{
		int2bin(MRT_st.expQ.bitmap,vtimer_bin);
		printf("\nPriority Bitmap of Expired Virtual Timers= %17s\n",vtimer_bin);
		i = 0;
		}
}

/*===========================================================================*
 *				MRT_vtQ_dmp	(Shift F8 Key )				     *
 *===========================================================================*/
PUBLIC void MRT_vtQ_dmp()
{
	int i;
	link_t *plk;
	MRT_vtimer_t *pvt;

	printf("\n\nTYPE PRTY INQ MAXINQ\n");

	/*  Active timer queue dump */
	printf("ACTV  ALL %3d %6d --->",MRT_st.actQ.list.inQ,MRT_st.actQ.list.maxinQ);
	for ( plk = MRT_st.actQ.list.first; plk != NULL; plk = plk->next )
			{
			pvt = (MRT_vtimer_t *) plk->this;
			printf("[%d]->",pvt->index);
			}
	printf("[NULL]\n");

	/*  Expired timer queues dump */
	for( i = 0; i < MRT_NR_PRTY; i++)
		{
		printf("EXPD %4d %3d %6d --->",
			i,MRT_st.expQ.list[i].inQ,MRT_st.expQ.list[i].maxinQ);
		for ( plk = MRT_st.expQ.list[i].first; plk != NULL; plk = plk->next )
			{
			pvt = (MRT_vtimer_t *) plk->this;
			printf("[%d]->",pvt->index);
			}
		printf("[NULL]\n");
		}

	/*  Free  timer queue dump */
	printf("FREE NONE %3d %6d --->",MRT_st.freeQ.inQ,MRT_st.freeQ.maxinQ);
	for ( plk = MRT_st.freeQ.first; plk != NULL; plk = plk->next)
		{
		pvt = (MRT_vtimer_t *) plk->this;
		printf("[%d]->",pvt->index);
		}
	printf("[NULL]\n");
}

/*===========================================================================*
 *				MRT_mpool_dmp	( F6 Key )	   		            *
 *===========================================================================*/
PUBLIC void MRT_mpool_dmp()
{
/* message pool dump */
	static int i = 0;
	MRT_mqe_t *pmqe;
	link_t	*plk;

	printf("MSG SPID/SNBR DPID/DNBR TYPE -MID- SEQNO TIMESTAMP PRI DLINE LAXTY\n");

	while ( i < NR_MESSAGES)
		{
		pmqe = mqe_addr(i);
		printf("%3d %4d/%4d %4d/%4d %4d %5d %5d %9d %3X %5d %5d \n",
			i,
			pmqe->msgd.hdr.src.p_nr,
			pmqe->msgd.hdr.src.pid,
			pmqe->msgd.hdr.dst.p_nr,
			pmqe->msgd.hdr.dst.pid,
			pmqe->msgd.hdr.mtype,

			pmqe->msgd.hdr.mid,
			pmqe->msgd.hdr.seqno,

			pmqe->msgd.hdr.tstamp,

			pmqe->msgd.hdr.priority,
			pmqe->msgd.hdr.deadline,
			pmqe->msgd.hdr.laxity 
			);
		i++;
		if( i%20 == 0 ) break;
		}
	if( i != NR_MESSAGES)
		printf("--more--\r");
	else
		{
		i = 0;
		/*  message pool free slots dump */
 		printf("\nMessage Free slots= %d \n",MRT_sm.mqeL.inQ);
		for ( plk = MRT_sm.mqeL.first; plk != NULL; plk = plk->next)
			{
			pmqe = (MRT_mqe_t*)plk->this;
			printf("[%d]->",pmqe->index);
			}
		printf("[NULL]\n");
		}
}

/*===========================================================================*
 *				MRT_mpool2_dmp	( Shift F6 Key )		            *
 *===========================================================================*/
PUBLIC void MRT_mpool2_dmp()
{
/* message pool dump */
	static int i = 0;
	MRT_mqe_t *pmqe;
	link_t	*plk;

	printf("MSG SPID/SNBR DPID/DNBR TYPE -MID- VT 01234567890123456789\n");

	while ( i < NR_MESSAGES)
		{
		pmqe = mqe_addr(i);
		printf("%3d %4d/%4d %4d/%4d %4d %5d %2d %s\n",
			i,
			pmqe->msgd.hdr.src.pid,
			pmqe->msgd.hdr.src.p_nr,
			pmqe->msgd.hdr.dst.pid,
			pmqe->msgd.hdr.dst.p_nr,
			pmqe->msgd.hdr.mtype,
			pmqe->msgd.hdr.mid,
			(pmqe->pvt == NULL)?-1:pmqe->pvt->index,
			&pmqe->msgd.m_u.m_m7 
			);
		i++;
		if( i%20 == 0 ) break;
		}
	if( i != NR_MESSAGES)
		printf("--more--\r");
	else
		{
		i = 0;
		/*  message pool free slots dump */
 		printf("\nMessage Free slots= %d \n",MRT_sm.mqeL.inQ);
		for ( plk = MRT_sm.mqeL.first; plk != NULL; plk = plk->next)
			{
			pmqe = (MRT_mqe_t*)plk->this;
			printf("[%d]->",pmqe->index);
			}
		printf("[NULL]\n");
		}
}

/*===========================================================================*
 *				MRT_msgQ_dmp (Ctrl F6 Key )   			     *
 *===========================================================================*/
PUBLIC void MRT_msgQ_dmp()
{
/* message queue dump */
	int i;
	MRT_msgQ_t	*mqp;
	link_t	*plk;


	printf("\n MESSAGE QUEUES STATUS AND STATISTICS\n");
	printf("ID SZ FLAG BITM INQ MAX OWN --DLVD-- --ENQD--\n");
	for( i=0; i < NR_MSGQ; i++)
		{
		mqp = msgQ_addr(i);	
		printf("%2d %2d %4X %4X %3d %3d %3d %8d %8d\n",
			mqp->index,
			mqp->size,
			mqp->flags,
			mqp->plist.bitmap,
			mqp->plist.inQ,
			mqp->plist.maxinQ,
			mqp->owner,
			mqp->delivered,
			mqp->enqueued);
		}
	printf("\nFree Message Queue List= %d \n",MRT_sm.msgQL.inQ);
	for ( plk = MRT_sm.msgQL.first; plk != NULL; plk = plk->next)
		{
		mqp = (MRT_msgQ_t*)plk->this;
		printf("[%d]->",mqp->index);
		}
	printf("[NULL]\n");
	printf("\n\n");
}

/*===========================================================================*
 *				MRT_latency_dmp	( Shift F7 Key )		            *
 *===========================================================================*/
PUBLIC void MRT_latency_dmp()
{
	if ( MRT_sv.flags & MRT_LATENCY)
		{
		printf("Latency computation OFF\n");
		CLR_BIT(MRT_sv.flags,MRT_LATENCY);
		}
	else
		{
		printf("Latency computation ON\n");
		SET_BIT(MRT_sv.flags,MRT_LATENCY);
		}
}


#endif /* MRT */


