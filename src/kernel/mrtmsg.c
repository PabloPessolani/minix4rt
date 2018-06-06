/*==========================================================================*
 *					mrtmsg.c 						    *
 * This file contains essentially all of the RT-message handling 		    *
 *==========================================================================*/

#include "kernel.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/type.h>
#include "proc.h"
#include "unistd.h"

#ifdef MRT

/*===========================================================================*
 *				MRT_mpool_init	 					     *
 * Initialize the message pool 							     * 
 *===========================================================================*/
PUBLIC void MRT_mpool_init(void )
{
	int i;
	MRT_mqe_t	*pmqe;
	
	for ( i = 0;  i < NR_MESSAGES ; i++)
		{ 
		pmqe 			= mqe_addr(i);
		pmqe->index 	= i;
		MRT_mqe_rst(pmqe);			/* reset all mqe fields  	*/
		MRT_list_app(&MRT_sm.mqeL,&pmqe->link); /* append into free List	*/
		}
}

/*===========================================================================*
 *				MRT_mqe_rst						     		*
 * Reset all fields of a message queue entry				    		*
 *===========================================================================*/
PUBLIC void MRT_mqe_rst(pmqe)
MRT_mqe_t	*pmqe;
{
	mrt_mhdr_t 	*mhp;

	mhp 			= &pmqe->msgd.hdr;
	mhp->src.pid 	= HARDWARE;
	mhp->src.p_nr 	= HARDWARE;
	mhp->dst.pid	= HARDWARE;
	mhp->dst.p_nr	= HARDWARE;
	mhp->mtype 		= MT_MBFREE;
	mhp->mid 		= 0;
	mhp->seqno 		= 0;
	mhp->tstamp 	= 0;
	mhp->priority	= MRT_PRIHIGHEST;
	mhp->deadline	= 0;

	strcpy((char *)&pmqe->msgd.m_u.m_m7
		,(char *)"----|----|----|----|");

	pmqe->pvt	  	= NULL;
	MRT_link_rst(&pmqe->link,(void*)pmqe, MRT_OBJMQE);

}

/*===========================================================================*
 *				MRT_msgQ_init						     *
 * Initialize the all message Queues						     * 
 *===========================================================================*/
PUBLIC void MRT_msgQ_init(void )
{
	int i;
	MRT_msgQ_t *pmq;

	MRT_list_rst(&MRT_sm.mqeL);
	MRT_list_rst(&MRT_sm.msgQL);

	for ( i = 0;  i < NR_MSGQ ; i++) 
		{
		pmq = msgQ_addr(i);
		pmq->index   = i;
		MRT_msgQ_free(pmq);
		}
}

/*===========================================================================*
 *				MRT_msgQ_rst						     *
 * Reset all fields of a message queue 					    	     *
 *===========================================================================*/
PUBLIC void MRT_msgQ_rst(pmq)
MRT_msgQ_t *pmq;
{
	pmq->size   	= 0;
	pmq->flags  	= 0;
	pmq->owner  	= MRTMSGQFREE;
	pmq->delivered 	= 0;
	pmq->enqueued  	= 0;
/*
	pmq->inQ	 	= 0;
	pmq->maxinQ  	= 0;
*/
	MRT_link_rst(&pmq->link,(void*)pmq, MRT_OBJMQ);
	MRT_plist_rst(&pmq->plist);			/* resets priority list fields*/
}

/*===========================================================================*
 *				MRT_msgQ_alloc						     *
 * Alloc a Message Queue for a process						     *
 *===========================================================================*/
PUBLIC MRT_msgQ_t *MRT_msgQ_alloc(mq)
MRT_msgQ_t *mq;
{
	int i;
	MRT_msgQ_t	*pmq;
	link_t *plink;

	plink = MRT_sm.msgQL.first;
	if( plink != NULL )
		{
		MRT_list_rmv(&MRT_sm.msgQL,plink); 	/* Removes the item from List */
		pmq = (MRT_msgQ_t	*)plink->this;
		pmq->size   = mq->size;
		pmq->flags  = mq->flags;
		pmq->owner  = mq->owner;
		} 
	else
		pmq = NULL;

	return(pmq);
}

/*===========================================================================*
 *				MRT_msgQ_free						     *
 * Frees a Message Queue from a process						     *
 *===========================================================================*/
PUBLIC void MRT_msgQ_free(pmq)
MRT_msgQ_t *pmq;
{
	int i;
	MRT_msgQ_rst(pmq);				/* Clears all fields		*/
	MRT_list_app(&MRT_sm.msgQL,&pmq->link);	/* Appends item to the List 	*/
}


/*===========================================================================*
 *				MRT_mqe_alloc						     *
 * Alloc a Message Queue Entry from the Message Pool				     *
 *===========================================================================*/
PUBLIC MRT_mqe_t *MRT_mqe_alloc(void)
{
	MRT_mqe_t *pmqe;
	link_t *plink;

	plink = MRT_sm.mqeL.first;				
	if( plink != NULL)
		{
		MRT_list_rmv(&MRT_sm.mqeL,plink); /* Removes entry from List	*/
		pmqe = (MRT_mqe_t *)plink->this;
		}
	else
		pmqe = NULL;
		
	return(pmqe);
}

/*===========================================================================*
 *				MRT_mqe_free						     *
 * Frees a Message Queue Entry to Message Pool					     *
 *===========================================================================*/
PUBLIC void MRT_mqe_free(pmqe)
MRT_mqe_t *pmqe;
{
	MRT_mqe_rst(pmqe);
	MRT_list_app(&MRT_sm.mqeL, &pmqe->link);		/* Appends item to the Free List*/
}

/*===========================================================================*
 *				MRT_mqe_app						     		*
 * Appends  message into a Message Queue 	at the tail of the queue	     	*
 *===========================================================================*/
PUBLIC int MRT_mqe_app(pmq, mqe)
MRT_msgQ_t *pmq;
MRT_mqe_t *mqe;
{
	int pty;

	if(pmq->plist.inQ == pmq->size)
		return(E_MRT_MQFULL);

	if( TEST_BIT(pmq->flags, MQ_PRTYORDER))
		pty = mqe->msgd.hdr.priority;
	else
		pty = MRT_PRIHIGHEST;
	
	MRT_plist_app(&pmq->plist,&mqe->link,pty);

/*

	pmq->inQ++;
	pmq->maxinQ = MAX(pmq->maxinQ,pmq->inQ);
*/

	pmq->enqueued++;

	return(OK);
}

/*===========================================================================*
 *				MRT_mqe_rmv						     		*
 * Removes a message from a Message Queue 					     	*
 *===========================================================================*/
PUBLIC void MRT_mqe_rmv(pmq, pmqe)
MRT_msgQ_t	*pmq;
MRT_mqe_t *pmqe;
{
	int pty;
	
	if( TEST_BIT(pmq->flags, MQ_PRTYORDER) )
		pty = pmqe->msgd.hdr.priority;
	else
		pty = MRT_PRIHIGHEST;

/*
	if ( pmq->inQ == 0)
		panic("MRT_mqe_rmv, inQ = 0 for msgQ entry ",pmqe->index);
*/

	MRT_plist_rmv(&pmq->plist,&pmqe->link,pty);
/*
	pmq->inQ--;
*/
	pmq->delivered++;
}

/*===========================================================================*
 *				MRT_mqe_pick					     		*
 * Pick the highest priority message entry from a specified source	     	*
 * from a Message Queue										*
 *===========================================================================*/
PUBLIC MRT_mqe_t *MRT_mqe_pick(pmq, source)
MRT_msgQ_t	*pmq;
int source;
{
	int p;
	MRT_mqe_t *pmqe;
	unsigned int bm;
	link_t *plink;
	
	if( pmq->plist.inQ == 0) return(NULL); 
	
	for( p = 0; p < MRT_NR_PRTY; p++)			/* searches all Message queues*/
		{
		if(TEST_BIT(pmq->plist.bitmap,(1 << p))) /* Search the prty List bitmap */
			{
			for( plink = pmq->plist.list[p].first; plink != NULL; plink = plink->next)
				{
				pmqe = plink->this;
				if( (source == MRT_ANYPROC)	/* search for a msg that meets */
				|| (source == pmqe->msgd.hdr.src.p_nr))
					return(pmqe);
				}
			}
		}
	return(NULL);		/* There are not any message from the specifiedd source*/
}

/*===========================================================================*
 *				MRT_ksend							     *
 * Asynchronous send of a message. Used by kernel functions			     *
 *===========================================================================*/
PUBLIC int MRT_ksend(src, dest, prty, mtype)
int src;
int dest;
int prty;
int mtype;
{
	MRT_proc_t	 	*dest_ptr, *src_ptr;
	MRT_mqe_t 		*pmqe, mqe;
	int			waiting;
	phys_bytes user_phys;
  	static long int CPUflags;
	
	src_ptr = proc_addr(src);
	
	/* verify destination process */
	dest_ptr = proc_addr(dest);
	if(!MRT_is_process(dest_ptr)) 			return(E_BAD_PROC);
	if(!MRT_is_realtime(dest_ptr)) 			return(E_MRT_BADPTYPE);
	if( dest_ptr->pmq->plist.inQ == dest_ptr->pmq->size) 	return(E_MRT_MQFULL);

	/* Verify if the destination process is waiting for a message from this process */
	if( TEST_BIT(dest_ptr->rt.flags,MRT_MBX_RCV) && 
	    (dest_ptr->pfrom == proc_ptr || 
		dest_ptr->pfrom == dest_ptr ))
		{
		pmqe = &mqe;
		waiting = TRUE;
		}
	else
		{
		if( (pmqe = MRT_mqe_alloc()) == NULL ) 	return(E_MRT_NOMQENT);
		waiting = FALSE;
		}

	/* fill the header's fields */
	pmqe->msgd.hdr.src.p_nr	= src_ptr->p_nr;
	pmqe->msgd.hdr.src.pid  = src_ptr->p_pid;
	pmqe->msgd.hdr.dst.p_nr	= dest_ptr->p_nr;
	pmqe->msgd.hdr.dst.pid	= dest_ptr->p_pid;
	pmqe->msgd.hdr.mtype  	= mtype;
	pmqe->msgd.hdr.mid	= ++MRT_sv.counter.messages;	
	pmqe->msgd.hdr.seqno  	= ++src_ptr->st.msgsent;
	pmqe->msgd.hdr.tstamp 	= MRT_sv.counter.ticks;	
	pmqe->msgd.hdr.priority = prty;
	pmqe->msgd.hdr.deadline = 0;
	
	/* copy the msg hdr from the msg queue entry to the proc addr */
	KRN2USR(dest_ptr,dest_ptr->pmhdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t));

	/* Verify if the destination process is waiting for a message from this process */
	if( waiting == TRUE)
		{
		/* statistics update */
		dest_ptr->pmq->delivered++;
		dest_ptr->pmq->enqueued++;
		dest_ptr->st.msgrcvd++;

		/* unblock the destination process */
		dest_ptr->pfrom = NULL;
		CLR_BIT(dest_ptr->rt.flags,MRT_MBX_RCV);
		if(MRT_is_rtready(dest_ptr)) MRT_ready(dest_ptr);

		/* stop the destination timeout vtimer */
		MRT_vtimer_stop(dest_ptr->ipcvt);
		}			
	else	/* The destination process is not waiting for this message	*/
		{
		if( MRT_mqe_app(dest_ptr->pmq, pmqe) != 0) 
			return(E_MRT_MQFULL);
		}

	if(TEST_BIT(dest_ptr->pmq->flags, MQ_PRTYINHERIT))
		MRT_inherit(prty, dest_ptr); 
	return(OK);
}

/*===========================================================================*
 *				rtk_receive							     *
 *===========================================================================*/
PUBLIC int rtk_receive(parm_ptr)
void *parm_ptr;
{
	MRT_mqe_t *pmqe;
	MRT_msgQ_t *pmq;
	struct proc *src_ptr;
	mrt_ipc_t 		*ipc;
	phys_bytes user_phys;
  	static long int CPUflags;
	
	ipc = (mrt_ipc_t*) parm_ptr;

	if( ipc->p_nr != MRT_ANYPROC)
		{
		src_ptr = proc_addr(ipc->p_nr);
		if(!MRT_is_process(src_ptr)) 		return(E_BAD_PROC);
		if(src_ptr->p_pid != ipc->pid) 		return(E_MRT_BADPTYPE);
		/* A RT-process could wait to receive a message from a current NRT-process */
		/* but a RT-process could not wait to send a message to a current NRT-process */
		}
	
	/* test if the queue is empty		*/
	if( (pmqe = MRT_mqe_pick(proc_ptr->pmq, ipc->p_nr)) == NULL)
		{
		if( ipc->timeout == MRT_NOWAIT) return(E_TRY_AGAIN);
		if( ipc->timeout != MRT_FOREVER) 
			VTSET(proc_ptr->ipcvt,ipc->timeout, 1, MRT_ACT_RCVFR, 
				proc_ptr->pmq->index,proc_ptr->rt.priority);

		SET_BIT(proc_ptr->rt.flags,MRT_MBX_RCV);
		proc_ptr->pfrom = (ipc->p_nr == MRT_ANYPROC)?proc_ptr:proc_addr(ipc->p_nr); 
		proc_ptr->pmsg 	= ipc->msg;
		proc_ptr->pmhdr 	= ipc->hdr;
 		MRT_unready(proc_ptr);
		return(OK);
		}

	/* copy the message content from MQE to USER*/
	KRN2USR(proc_ptr, ipc->msg,&pmqe->msgd.m_u,sizeof(mrt_msg_t));

	/* copy the message header */
	KRN2USR(proc_ptr, ipc->hdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t));

	/* unblock the sender if it was a Request for the receiver process */
	src_ptr = proc_addr(pmqe->msgd.hdr.src.p_nr);
	if( pmqe->msgd.hdr.mtype == MT_REQUEST && src_ptr->pto == proc_ptr )
		{
		/* stops the source timeout VT */
		MRT_vtimer_stop(src_ptr->ipcvt);
		CLR_BIT(src_ptr->rt.flags,MRT_MBX_SND);
		src_ptr->pto 	= NULL;
		if(MRT_is_rtready(src_ptr)) MRT_ready(src_ptr);
		}
	else if( pmqe->msgd.hdr.mtype == MT_RQRCV && src_ptr->pto == proc_ptr )
		{
		CLR_BIT(src_ptr->rt.flags,MRT_MBX_SND);
		src_ptr->pto = NULL;
		}

	MRT_mqe_rmv(proc_ptr->pmq, pmqe);
	MRT_mqe_rst(pmqe);
	MRT_mqe_free(pmqe);
	proc_ptr->st.msgrcvd++;
	return(OK);
}

/*===========================================================================*
 *				rtk_rqst							     *
 *===========================================================================*/
PUBLIC int rtk_rqst(parm_ptr)
void *parm_ptr;
{
	MRT_proc_t	 	*dest_ptr;
	MRT_mqe_t 		*pmqe, mqe;
	int			waiting;
	mrt_ipc_t 		*ipc;
	phys_bytes 		user_phys, user_phys1;
  	static long int CPUflags;
	
	ipc = (mrt_ipc_t*) parm_ptr;

	/* verify destination process */
	dest_ptr = proc_addr(ipc->p_nr);
	if(!MRT_is_process(dest_ptr)) 			return(E_BAD_PROC);
	if( dest_ptr->p_pid != ipc->pid) 			return(E_BAD_DEST);
	if(!MRT_is_realtime(dest_ptr)) 			return(E_MRT_BADPTYPE);
	if( dest_ptr->pmq->plist.inQ == dest_ptr->pmq->size) 	return(E_MRT_MQFULL);

	/* Verify if the destination process is waiting for a message from this process */
	if( TEST_BIT(dest_ptr->rt.flags,MRT_MBX_RCV) && 
	    (dest_ptr->pfrom == proc_ptr || 
		dest_ptr->pfrom == dest_ptr ))
		{
		pmqe = &mqe;
		waiting = TRUE;
		}
	else
		{
		if( ipc->timeout == MRT_NOWAIT)		return(E_TRY_AGAIN);
		if( (pmqe = MRT_mqe_alloc()) == NULL ) 	return(E_MRT_NOMQENT);
		waiting = FALSE;
		}

	/* fill the header's fields */
	pmqe->msgd.hdr.src.p_nr	= proc_ptr->p_nr;
	pmqe->msgd.hdr.src.pid  = proc_ptr->p_pid;
	pmqe->msgd.hdr.dst.p_nr	= dest_ptr->p_nr;
	pmqe->msgd.hdr.dst.pid	= dest_ptr->p_pid;
	pmqe->msgd.hdr.mtype  	= MT_REQUEST;
	pmqe->msgd.hdr.mid	= ++MRT_sv.counter.messages;	
	pmqe->msgd.hdr.seqno  	= ++proc_ptr->st.msgsent;
	pmqe->msgd.hdr.tstamp 	= MRT_sv.counter.ticks;	
	pmqe->msgd.hdr.priority = proc_ptr->rt.priority;
	pmqe->msgd.hdr.deadline = proc_ptr->rt.deadline;
	
	/* Verify if the destination process is waiting for a message from this process */
	if( waiting == TRUE)
		{
		/* copy the msg from src to dest */
		USR2USR(dest_ptr, proc_ptr,  dest_ptr->pmsg, ipc->msg, sizeof(mrt_msg_t));
	
		/* copy the msg hdr from the msg queue entry to the proc addr */
		KRN2USR(dest_ptr, dest_ptr->pmhdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t)); 
	
		/* statistics update */
		dest_ptr->pmq->delivered++;
		dest_ptr->pmq->enqueued++;
		dest_ptr->pmq->plist.maxinQ = MAX(dest_ptr->pmq->plist.maxinQ,(dest_ptr->pmq->plist.inQ)+1);

		dest_ptr->st.msgrcvd++;

		/* unblock the destination process */
		dest_ptr->pfrom = NULL;
		CLR_BIT(dest_ptr->rt.flags,MRT_MBX_RCV);
		if(MRT_is_rtready(dest_ptr)) MRT_ready(dest_ptr);

		/* stop the destination timeout vtimer */
		MRT_vtimer_stop(dest_ptr->ipcvt);
		}			
	else	/* The destination process is not waiting for this message	*/
		{
		/* copy the message to the message queue entry */
		USR2KRN( proc_ptr, &pmqe->msgd.m_u, ipc->msg,sizeof(mrt_msg_t));

		if( MRT_mqe_app(dest_ptr->pmq, pmqe) != 0) 
			panic("rtk_rqst: the msgq must not be full for proc !!",dest_ptr->p_nr);

		if( ipc->timeout != MRT_FOREVER)
			VTSET(proc_ptr->ipcvt, ipc->timeout, 1, MRT_ACT_SNDTO, 
				pmqe->index,proc_ptr->rt.priority);

		/* block the current process waiting to send the msg */
		SET_BIT(proc_ptr->rt.flags,MRT_MBX_SND);
		proc_ptr->pto = dest_ptr;
		MRT_unready(proc_ptr);
		}

	if(TEST_BIT(dest_ptr->pmq->flags, MQ_PRTYINHERIT))
		MRT_inherit(proc_ptr->rt.priority, dest_ptr); 
	return(OK);
}

/*===========================================================================*
 *				rtk_arqst							     *
 *===========================================================================*/
PUBLIC int rtk_arqst(parm_ptr)
void *parm_ptr;
{
	MRT_proc_t	 	*dest_ptr;
	MRT_mqe_t 		*pmqe, mqe;
	int			waiting;
	mrt_ipc_t 		*ipc;
	phys_bytes 		user_phys, user_phys1;
  	static long int CPUflags;
	
	ipc = (mrt_ipc_t*) parm_ptr;

	/* verify destination process */
	dest_ptr = proc_addr(ipc->p_nr);
	if(!MRT_is_process(dest_ptr)) 			return(E_BAD_PROC);
	if( dest_ptr->p_pid != ipc->pid) 			return(E_BAD_DEST);
	if(!MRT_is_realtime(dest_ptr)) 			return(E_MRT_BADPTYPE);
	if( dest_ptr->pmq->plist.inQ == dest_ptr->pmq->size) 	return(E_MRT_MQFULL);

	/* Verify if the destination process is waiting for a message from this process */
	if( TEST_BIT(dest_ptr->rt.flags,MRT_MBX_RCV) && 
	    (dest_ptr->pfrom == proc_ptr || 
		dest_ptr->pfrom == dest_ptr ))
		{
		pmqe = &mqe;
		waiting = TRUE;
		}
	else
		{
		if( (pmqe = MRT_mqe_alloc()) == NULL ) 	return(E_MRT_NOMQENT);
		waiting = FALSE;
		}

	/* fill the header's fields */
	pmqe->msgd.hdr.src.p_nr	= proc_ptr->p_nr;
	pmqe->msgd.hdr.src.pid  = proc_ptr->p_pid;
	pmqe->msgd.hdr.dst.p_nr	= dest_ptr->p_nr;
	pmqe->msgd.hdr.dst.pid	= dest_ptr->p_pid;
	pmqe->msgd.hdr.mtype  	= MT_AREQUEST;
	pmqe->msgd.hdr.mid	= ++MRT_sv.counter.messages;	
	pmqe->msgd.hdr.seqno  	= ++proc_ptr->st.msgsent;
	pmqe->msgd.hdr.tstamp 	= MRT_sv.counter.ticks;	
	pmqe->msgd.hdr.priority = proc_ptr->rt.priority;
	pmqe->msgd.hdr.deadline = proc_ptr->rt.deadline;
	
	/* Verify if the destination process is waiting for a message from this process */
	if( waiting == TRUE)
		{
		/* copy the msg from src to dest */
		USR2USR(dest_ptr, proc_ptr,  dest_ptr->pmsg, ipc->msg, sizeof(mrt_msg_t));
	
		/* copy the msg hdr from the msg queue entry to the proc addr */
		KRN2USR(dest_ptr, dest_ptr->pmhdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t)); 

		/* statistics update */
		dest_ptr->pmq->delivered++;
		dest_ptr->pmq->enqueued++;
		dest_ptr->pmq->plist.maxinQ = MAX(dest_ptr->pmq->plist.maxinQ,(dest_ptr->pmq->plist.inQ)+1);
		dest_ptr->st.msgrcvd++;

		/* unblock the destination process */
		dest_ptr->pfrom = NULL;
		CLR_BIT(dest_ptr->rt.flags,MRT_MBX_RCV);
		if(MRT_is_rtready(dest_ptr)) MRT_ready(dest_ptr);

		/* stop the destination timeout vtimer */
		MRT_vtimer_stop(dest_ptr->ipcvt);

		}			
	else	/* The destination process is not waiting for this message	*/
		{
		/* copy the message to the message queue entry */
		USR2KRN( proc_ptr, &pmqe->msgd.m_u, ipc->msg,sizeof(mrt_msg_t));

		if( MRT_mqe_app(dest_ptr->pmq, pmqe) != 0) 
			panic("rtk_arqst: the msgq must not be full for proc !!",dest_ptr->p_nr);
		}

	if(TEST_BIT(dest_ptr->pmq->flags, MQ_PRTYINHERIT))
		MRT_inherit(proc_ptr->rt.priority, dest_ptr); 
	return(OK);
}

/*===========================================================================*
 *				rtk_reply							     *
 *===========================================================================*/
PUBLIC int rtk_reply(parm_ptr)
void *parm_ptr;
{
	MRT_proc_t	 	*dest_ptr;
	MRT_mqe_t 		*pmqe, mqe;
	int			rcode = 0,waiting;
	mrt_ipc_t 		*ipc;
	phys_bytes 		user_phys, user_phys1;
  	static long int CPUflags;
	
	ipc = (mrt_ipc_t*) parm_ptr;

	/* verify destination process */
	dest_ptr = proc_addr(ipc->p_nr);
	if(!MRT_is_process(dest_ptr))
		rcode = E_BAD_PROC;
	else if(dest_ptr->p_pid != ipc->pid) 
		rcode = E_BAD_DEST;
	else if(!MRT_is_realtime(dest_ptr)) 
		rcode = E_MRT_BADPTYPE;
	else if(dest_ptr->pmq->plist.inQ == dest_ptr->pmq->size)
		rcode = E_MRT_MQFULL;
	if ( rcode != 0) 
		{
		if(TEST_BIT(proc_ptr->pmq->flags, MQ_PRTYINHERIT))
			MRT_disinherit(proc_ptr); 
		return(rcode);
		}		

	/* Verify if the destination process is waiting for a message from this process */
	if( TEST_BIT(dest_ptr->rt.flags,MRT_MBX_RCV) && 
	    (dest_ptr->pfrom == proc_ptr))
		{
		pmqe = &mqe;
		waiting = TRUE;
		}
	else
		{
		if( (pmqe = MRT_mqe_alloc()) == NULL ) 	return(E_MRT_NOMQENT);
		waiting = FALSE;
		}

	/* fill the header's fields */
	pmqe->msgd.hdr.src.p_nr	= proc_ptr->p_nr;
	pmqe->msgd.hdr.src.pid  = proc_ptr->p_pid;
	pmqe->msgd.hdr.dst.p_nr	= dest_ptr->p_nr;
	pmqe->msgd.hdr.dst.pid	= dest_ptr->p_pid;
	pmqe->msgd.hdr.mtype  	= MT_REPLY;
	pmqe->msgd.hdr.mid	= ++MRT_sv.counter.messages;	
	pmqe->msgd.hdr.seqno  	= ++proc_ptr->st.msgsent;
	pmqe->msgd.hdr.tstamp 	= MRT_sv.counter.ticks;	
	pmqe->msgd.hdr.priority = proc_ptr->rt.priority;
	pmqe->msgd.hdr.deadline = proc_ptr->rt.deadline;
	
	/* Verify if the destination process is waiting for a message from this process */
	if( waiting == TRUE)
		{
		/* copy the msg from src to dest */
		USR2USR(dest_ptr, proc_ptr,  dest_ptr->pmsg, ipc->msg, sizeof(mrt_msg_t));
	
		/* copy the msg hdr from the msg queue entry to the proc addr */
		KRN2USR(dest_ptr, dest_ptr->pmhdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t)); 

		/* statistics update */
		dest_ptr->pmq->delivered++;
		dest_ptr->pmq->enqueued++;
		dest_ptr->pmq->plist.maxinQ = MAX(dest_ptr->pmq->plist.maxinQ,(dest_ptr->pmq->plist.inQ)+1);
		dest_ptr->st.msgrcvd++;

		/* unblock the destination process */
		dest_ptr->pfrom = NULL;
		CLR_BIT(dest_ptr->rt.flags,MRT_MBX_RCV);
		if(MRT_is_rtready(dest_ptr)) MRT_ready(dest_ptr);

		/* stop the destination timeout vtimer */
		MRT_vtimer_stop(dest_ptr->ipcvt);
		}			
	else	/* The destination process is not waiting for this message	*/
		{
		/* copy the message to the message queue entry */
		USR2KRN( proc_ptr, &pmqe->msgd.m_u, ipc->msg,sizeof(mrt_msg_t));

		if( MRT_mqe_app(dest_ptr->pmq, pmqe) != 0) 
			panic("rtk_rply: the msgq must not be full for proc !!",dest_ptr->p_nr);
		}

	if(TEST_BIT(proc_ptr->pmq->flags, MQ_PRTYINHERIT))
		MRT_disinherit(proc_ptr); 
	return(OK);
}


/*===========================================================================*
 *				rtk_uprqst							     *
 *===========================================================================*/
PUBLIC int rtk_uprqst(parm_ptr)
void *parm_ptr;
{
	MRT_proc_t	 	*dest_ptr;
	MRT_mqe_t 		*pmqe, mqe;
	int			waiting;
	mrt_ipc_t 		*ipc;
	phys_bytes 		user_phys, user_phys1;
  	static long int CPUflags;
	
	ipc = (mrt_ipc_t*) parm_ptr;

	/* verify destination process */
	dest_ptr = proc_addr(ipc->p_nr);
	if(!MRT_is_process(dest_ptr)) 			return(E_BAD_PROC);
	if( dest_ptr->p_pid != ipc->pid) 			return(E_BAD_DEST);
	if(!MRT_is_realtime(dest_ptr)) 			return(E_MRT_BADPTYPE);
	if( dest_ptr->pmq->plist.inQ == dest_ptr->pmq->size) 	return(E_MRT_MQFULL);

	/* Verify if the destination process is waiting for a message from this process */
	if( TEST_BIT(dest_ptr->rt.flags,MRT_MBX_RCV) && 
	    (dest_ptr->pfrom == proc_ptr || 
		dest_ptr->pfrom == dest_ptr ))
		{
		pmqe = &mqe;
		waiting = TRUE;
		}
	else
		{
		if( (pmqe = MRT_mqe_alloc()) == NULL ) 	return(E_MRT_NOMQENT);
		waiting = FALSE;
		}

	/* fill the header's fields */
	pmqe->msgd.hdr.src.p_nr	= proc_ptr->p_nr;
	pmqe->msgd.hdr.src.pid  = proc_ptr->p_pid;
	pmqe->msgd.hdr.dst.p_nr	= dest_ptr->p_nr;
	pmqe->msgd.hdr.dst.pid	= dest_ptr->p_pid;
	pmqe->msgd.hdr.mtype  	= MT_UPRQST;
	pmqe->msgd.hdr.mid	= ++MRT_sv.counter.messages;	
	pmqe->msgd.hdr.seqno  	= ++proc_ptr->st.msgsent;
	pmqe->msgd.hdr.tstamp 	= MRT_sv.counter.ticks;	
	pmqe->msgd.hdr.priority = ipc->priority;
	pmqe->msgd.hdr.deadline = 0;
	
	/* Verify if the destination process is waiting for a message from this process */
	if( waiting == TRUE)
		{
		/* copy the msg from src to dest */
		USR2USR(dest_ptr, proc_ptr,  dest_ptr->pmsg, ipc->msg, sizeof(mrt_msg_t));
	
		/* copy the msg hdr from the msg queue entry to the proc addr */
		KRN2USR(dest_ptr, dest_ptr->pmhdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t)); 

		/* statistics update */
		dest_ptr->pmq->delivered++;
		dest_ptr->pmq->enqueued++;
		dest_ptr->pmq->plist.maxinQ = MAX(dest_ptr->pmq->plist.maxinQ,(dest_ptr->pmq->plist.inQ)+1);
		dest_ptr->st.msgrcvd++;

		/* unblock the destination process */
		dest_ptr->pfrom = NULL;
		CLR_BIT(dest_ptr->rt.flags,MRT_MBX_RCV);
		if(MRT_is_rtready(dest_ptr)) MRT_ready(dest_ptr);

		/* stop the destination timeout vtimer */
		MRT_vtimer_stop(dest_ptr->ipcvt);
		}			
	else	/* The destination process is not waiting for this message	*/
		{
		/* copy the message to the message queue entry */
		USR2KRN( proc_ptr, &pmqe->msgd.m_u, ipc->msg,sizeof(mrt_msg_t));

		if( MRT_mqe_app(dest_ptr->pmq, pmqe) != 0) 
			panic("rtk_uprqst: the msgq must not be full for proc !!",dest_ptr->p_nr);
		}

	if(TEST_BIT(dest_ptr->pmq->flags, MQ_PRTYINHERIT))
		MRT_inherit(ipc->priority, dest_ptr); 
	return(OK);
}

/*===========================================================================*
 *				rtk_sign							     *
 *===========================================================================*/
PUBLIC int rtk_sign(parm_ptr)
void *parm_ptr;
{
	MRT_proc_t	 	*dest_ptr;
	MRT_mqe_t 		*pmqe, mqe;
	int			waiting;
	mrt_ipc_t 		*ipc;
	phys_bytes 		user_phys, user_phys1;
  	static long int CPUflags;
	
	ipc = (mrt_ipc_t*) parm_ptr;

	/* verify destination process */
	dest_ptr = proc_addr(ipc->p_nr);
	if(!MRT_is_process(dest_ptr)) 			return(E_BAD_PROC);
	if( dest_ptr->p_pid != ipc->pid) 			return(E_BAD_DEST);
	if(!MRT_is_realtime(dest_ptr)) 			return(E_MRT_BADPTYPE);
	if( dest_ptr->pmq->plist.inQ == dest_ptr->pmq->size) 	return(E_MRT_MQFULL);

	/* Verify if the destination process is waiting for a message from this process */
	if( TEST_BIT(dest_ptr->rt.flags,MRT_MBX_RCV) && 
	    (dest_ptr->pfrom == proc_ptr || 
		dest_ptr->pfrom == dest_ptr ))
		{
		pmqe = &mqe;
		waiting = TRUE;
		}
	else
		{
		if( (pmqe = MRT_mqe_alloc()) == NULL ) 	return(E_MRT_NOMQENT);
		waiting = FALSE;
		}

	/* fill the header's fields */
	pmqe->msgd.hdr.src.p_nr	= proc_ptr->p_nr;
	pmqe->msgd.hdr.src.pid  = proc_ptr->p_pid;
	pmqe->msgd.hdr.dst.p_nr	= dest_ptr->p_nr;
	pmqe->msgd.hdr.dst.pid	= dest_ptr->p_pid;
	pmqe->msgd.hdr.mtype  	= MT_SIGNAL;
	pmqe->msgd.hdr.mid	= ++MRT_sv.counter.messages;	
	pmqe->msgd.hdr.seqno  	= ++proc_ptr->st.msgsent;
	pmqe->msgd.hdr.tstamp 	= MRT_sv.counter.ticks;	
	pmqe->msgd.hdr.priority = dest_ptr->rt.priority;
	pmqe->msgd.hdr.deadline = dest_ptr->rt.deadline;
	
	/* Verify if the destination process is waiting for a message from this process */
	if( waiting == TRUE)
		{
		/* copy the msg from src to dest */
		USR2USR(dest_ptr, proc_ptr,  dest_ptr->pmsg, ipc->msg, sizeof(mrt_msg_t));
	
		/* copy the msg hdr from the msg queue entry to the proc addr */
		KRN2USR(dest_ptr, dest_ptr->pmhdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t)); 

		/* statistics update */
		dest_ptr->pmq->delivered++;
		dest_ptr->pmq->enqueued++;
		dest_ptr->pmq->plist.maxinQ = MAX(dest_ptr->pmq->plist.maxinQ,(dest_ptr->pmq->plist.inQ)+1);
		dest_ptr->st.msgrcvd++;

		/* unblock the destination process */
		dest_ptr->pfrom = NULL;
		CLR_BIT(dest_ptr->rt.flags,MRT_MBX_RCV);
		if(MRT_is_rtready(dest_ptr)) MRT_ready(dest_ptr);

		/* stop the destination timeout vtimer */
		MRT_vtimer_stop(dest_ptr->ipcvt);
		}			
	else	/* The destination process is not waiting for this message	*/
		{
		/* copy the message to the message queue entry */
		USR2KRN( proc_ptr, &pmqe->msgd.m_u, ipc->msg,sizeof(mrt_msg_t));

		if( MRT_mqe_app(dest_ptr->pmq, pmqe) != 0) 
			panic("rtk_sign: the msgq must not be full for proc !!",dest_ptr->p_nr);
		}
	return(OK);
}

/*===========================================================================*
 *				rtk_rqrcv							     *
 *===========================================================================*/
PUBLIC int rtk_rqrcv(parm_ptr)
void *parm_ptr;
{
	MRT_proc_t	 	*dest_ptr;
	MRT_mqe_t 		*pmqe, mqe;
	int			waiting;
	mrt_ipc_t 		*ipc;
	phys_bytes 		user_phys, user_phys1;
  	static long int CPUflags;
	
	ipc = (mrt_ipc_t*) parm_ptr;
	if( ipc->timeout == MRT_NOWAIT)			return(EINVAL);

	/* verify destination process */
	dest_ptr = proc_addr(ipc->p_nr);
	if(!MRT_is_process(dest_ptr)) 			return(E_BAD_PROC);
	if( dest_ptr->p_pid != ipc->pid) 		return(E_BAD_DEST);
	if(!MRT_is_realtime(dest_ptr)) 			return(E_MRT_BADPTYPE);
	if( dest_ptr->pmq->plist.inQ == dest_ptr->pmq->size) 	return(E_MRT_MQFULL);

	/* Verify if the destination process is waiting for a message from this process */
	if( TEST_BIT(dest_ptr->rt.flags,MRT_MBX_RCV) && 
	    (dest_ptr->pfrom == proc_ptr || 
		dest_ptr->pfrom == dest_ptr ))
		{
		pmqe = &mqe;
		waiting = TRUE;
		}
	else
		{
		/*
		if( ipc->timeout != MRT_FOREVER && 
			MRT_st.freeQ.inQ == 0) 				return(E_MRT_NOVTIMER);
		*/
		if( (pmqe = MRT_mqe_alloc()) == NULL ) 	return(E_MRT_NOMQENT);
		waiting = FALSE;
		}

	/* fill the header's fields */
	pmqe->msgd.hdr.src.p_nr	= proc_ptr->p_nr;
	pmqe->msgd.hdr.src.pid  = proc_ptr->p_pid;
	pmqe->msgd.hdr.dst.p_nr	= dest_ptr->p_nr;
	pmqe->msgd.hdr.dst.pid	= dest_ptr->p_pid;
	pmqe->msgd.hdr.mtype  	= MT_RQRCV;
	pmqe->msgd.hdr.mid	= ++MRT_sv.counter.messages;	
	pmqe->msgd.hdr.seqno  	= ++proc_ptr->st.msgsent;
	pmqe->msgd.hdr.tstamp 	= MRT_sv.counter.ticks;	
	pmqe->msgd.hdr.priority = proc_ptr->rt.priority;
	pmqe->msgd.hdr.deadline = proc_ptr->rt.deadline;
	
	/* Verify if the destination process is waiting for a message from this process */
	if( waiting == TRUE)
		{
		/*............................ RQST SECTION .............................*/

		/* copy the msg from src to dest */
		USR2USR(dest_ptr, proc_ptr,  dest_ptr->pmsg, ipc->msg, sizeof(mrt_msg_t));
	
		/* copy the msg hdr from the msg queue entry to the proc addr */
		KRN2USR(dest_ptr, dest_ptr->pmhdr,&pmqe->msgd.hdr,sizeof(mrt_mhdr_t)); 

		/* statistics update */
		dest_ptr->pmq->delivered++;
		dest_ptr->pmq->enqueued++;
		dest_ptr->pmq->plist.maxinQ = MAX(dest_ptr->pmq->plist.maxinQ,(dest_ptr->pmq->plist.inQ)+1);
		dest_ptr->st.msgrcvd++;

		/* unblock the destination process */
		dest_ptr->pfrom = NULL;
		CLR_BIT(dest_ptr->rt.flags,MRT_MBX_RCV);

		if(MRT_is_rtready(dest_ptr)) MRT_ready(dest_ptr);

		/* stop the destination timeout vtimer */
		MRT_vtimer_stop(dest_ptr->ipcvt);
		proc_ptr->ipcvt->param  = -1;
		}			
	else	/* The destination process is not waiting for this message	*/
		{
		/*............................ RQST SECTION .............................*/
		/* copy the message to the message queue entry */
		USR2KRN( proc_ptr, &pmqe->msgd.m_u, ipc->msg,sizeof(mrt_msg_t));

		if( MRT_mqe_app(dest_ptr->pmq, pmqe) != 0) 
			panic("rtk_rqst: the msgq must not be full for proc !!",dest_ptr->p_nr);

		SET_BIT(proc_ptr->rt.flags,MRT_MBX_SND);
		proc_ptr->pto  = proc_addr(ipc->p_nr);
		proc_ptr->ipcvt->param  = pmqe->index;
		}

	/*............................ RCV SECTION ............................*/

	if( ipc->timeout != MRT_FOREVER) 
		VTSET(proc_ptr->ipcvt, ipc->timeout, 1, MRT_ACT_SNDRCV, 0, proc_ptr->rt.priority);
	
	SET_BIT(proc_ptr->rt.flags,MRT_MBX_RCV);
	proc_ptr->pfrom   = dest_ptr; 
	proc_ptr->pmsg 	= ipc->msg2;
	proc_ptr->pmhdr 	= ipc->hdr;
	MRT_unready(proc_ptr);
	if(TEST_BIT(dest_ptr->pmq->flags, MQ_PRTYINHERIT))
		MRT_inherit(proc_ptr->rt.priority, dest_ptr); 
	return(OK);
}

#endif /* MRT */

