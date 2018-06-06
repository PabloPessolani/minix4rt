/*==========================================================================*
 *					mrtlib.c 						    *
 * This file contains essentially library functions		 		    *
 *==========================================================================*/

#include "kernel.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/type.h>
#include "proc.h"
#include "unistd.h"

#ifdef MRT


/*===========================================================================*
 *				MRT_plist_rst						     *
 * Reset all fields of a Priority List					    	     *
 *===========================================================================*/
PUBLIC void MRT_plist_rst(pl)
plist_t *pl;
{
	int i;

	for( i = 0; i < MRT_NR_PRTY; i++)
		MRT_list_rst(&pl->list[i]);
	pl->bitmap = 0;
	pl->inQ    = 0;
	pl->maxinQ = 0;
}

/*===========================================================================*
 *				MRT_list_rst						     *
 * Reset all fields of a double linked list 				    	     *
 *===========================================================================*/
PUBLIC void MRT_list_rst(pl)
list_t *pl;
{
	pl->inQ 	= 0;
	pl->maxinQ 	= 0;
	pl->first 	= NULL;
	pl->last 	= NULL;
}

/*===========================================================================*
 *				MRT_link_rst						     *
 * Reset all fields of a double linked list 				    	     *
 *===========================================================================*/
PUBLIC void MRT_link_rst(plink,this,object)
link_t *plink;
void *this;
int object;
{
	plink->next 	= NULL;
	plink->prev 	= NULL;
	plink->this 	= this;
	plink->object 	= object;
}

/*===========================================================================*
 *				MRT_list_app						     *
 * Appends an object in a double linked list 					     *
 *===========================================================================*/
PUBLIC void MRT_list_app(plist, plink)
list_t *plist;
link_t *plink;
{
	if(plist->first == NULL)			/* The list is empty			*/
		{
		plink->prev		= NULL;
		plist->first 	= plink;
		}
	else							/* inserts the object into the Queue tail*/
		{
		plink->prev 	= plist->last;
		plist->last->next	= plink;
		}

	plink->next 	= NULL;
	plist->last 	= plink;

	plist->inQ++;						/* increase the number objects in list*/
	plist->maxinQ = MAX(plist->inQ,plist->maxinQ); 	/* computes maximum number of objects*/
}

/*===========================================================================*
 *				MRT_list_ins						     *
 * Inserts an object in the head of a double linked list 			     *
 *===========================================================================*/
PUBLIC void MRT_list_ins(plist, plink)
list_t *plist;
link_t *plink;
{
	plink->next 	= plist->first;
	plink->prev		= NULL;

	if(plist->first == NULL)			/* The list is empty			*/
		plist->last 	= plink;
	else							
		plist->first->prev = plink;

	plist->first	= plink;

	plist->inQ++;						/* increase the number objects in list*/
	plist->maxinQ = MAX(plist->inQ,plist->maxinQ); 	/* computes maximum number of objects */
}

/*===========================================================================*
 *				MRT_list_rmv						     *
 * Removes an object from a double linked list 				    	     *
 *===========================================================================*/
PUBLIC void MRT_list_rmv(plist, plink)
list_t *plist;
link_t *plink;
{
	if( plink->prev == NULL)				
		plist->first = plink->next;
	else
		plink->prev->next = plink->next;

	if( plink->next == NULL)				
		plist->last = plink->prev;
	else
		plink->next->prev = plink->prev;

	plink->next = NULL;	
	plink->prev = NULL;
	plist->inQ--;		
}

/*===========================================================================*
 *				MRT_plist_app						     *
 * Appends an object in a Priority List	 					     *
 *===========================================================================*/
PUBLIC void MRT_plist_app(plist, plink, pty)
plist_t *plist;
link_t *plink;
int  pty;
{
	MRT_list_app(&plist->list[pty],plink);

	plist->inQ++;						/* increase the number objects in list*/
	plist->maxinQ = MAX(plist->inQ,plist->maxinQ); 	/* computes maximum number of objects*/

	SET_BIT(plist->bitmap,(1 << pty)); 
}

/*===========================================================================*
 *				MRT_plist_ins						     *
 * Appends an object in a Priority List 						     *
 *===========================================================================*/
PUBLIC void MRT_plist_ins(plist, plink, pty)
plist_t *plist;
link_t *plink;
int  pty;
{
	MRT_list_ins(&plist->list[pty],plink);
	plist->inQ++;						/* increase the number objects in list*/
	plist->maxinQ = MAX(plist->inQ,plist->maxinQ); 	/* computes maximum number of objects*/
	SET_BIT(plist->bitmap,(1 << pty)); 
}

/*===========================================================================*
 *				MRT_plist_rmv						     *
 * Removes an object from a Priority List					    	     *
 *===========================================================================*/
PUBLIC void MRT_plist_rmv(plist, plink, pty)
plist_t *plist;
link_t *plink;
int  pty;
{
	MRT_list_rmv(&plist->list[pty],plink);
	if( plist->list[pty].inQ == 0)
		CLR_BIT(plist->bitmap,(1 << pty)); 
	plist->inQ--;
}

/*===========================================================================*
 *				MRT_plist_get						     *
 * Gets the highest priority object in a Priority List		    	     *
 *===========================================================================*/
PUBLIC link_t *MRT_plist_get(plist)
plist_t *plist;
{
	int p;

	if( plist->inQ == 0) return(NULL);

	for( p = 0 ; p < MRT_NR_PRTY ; p++)
		if(TEST_BIT(plist->bitmap,(1 << p))) /* Search the prty List bitmap */
			return(plist->list[p].first);
	return(NULL);
}

/*===========================================================================*
 *				MRT_pid2pptr						     *
 * Converts a PID into a process pointer						     *
 * This function can be enhanced but this task is for MINIX developers	     *
 *===========================================================================*/
PUBLIC struct proc *MRT_pid2pptr(pid )
pid_t	pid;
{
      register struct proc *rp;

	/* convert PID into process Number	*/
	for (rp = BEG_PROC_ADDR; rp < END_PROC_ADDR; ++rp) 
		if( rp->p_pid == pid) break;

	if( rp == END_PROC_ADDR) 		return(NULL);
	if( rp->p_flags == P_SLOT_FREE) 	return(NULL);

	return(rp);
}

/*===========================================================================*
 *				int2bin 			   				     *
 * Convert an integer into a string of "0" and "1" representing the binary   *
 * value of the number									     *
 *===========================================================================*/
PUBLIC void int2bin( number, bin)
int number;
char *bin;
{
	int sreg;

	for( sreg = 0x8000; sreg != 0 ; sreg = sreg >> 1, bin++)
		*bin = (number & sreg)?'1':'0';

	*bin = '\0';
}

/*===========================================================================*
 *				MRT_inherit							     *
 * Transfer the priority from the sender to the destination process 	     *
 * or from a process that downs a mutex semaphore to the semaphore carrier   *
 * Its a transitive process (recurrent function)				     *
 * OJO: Aquellos objetos que un proceso tiene (mensajes, timers, etc)        *
 * NO CAMBIAN DE PRIORIDAD								     *
 *===========================================================================*/
PUBLIC void MRT_inherit(prty, pptr)
int prty;
MRT_proc_t *pptr;
{
	struct proc *p;

	if(!TEST_BIT(MRT_sv.flags,MRT_BPIP)) return;

	if( prty < pptr->rt.priority )		/* only if it has lower prty	*/
		{
		if(MRT_is_rtready(pptr))		/* if it is in ready state	*/
			{
			MRT_rdyQ_rmv(pptr);		/* removes from its readyQ	*/
			pptr->rt.priority = prty;	/* changes its priority		*/
			MRT_rdyQ_ins(pptr);		/* inserts into the new readyQ*/
			}
		else						/* the process is BLOCKED!!	*/
			{	
			pptr->rt.priority = prty;	/* changes its priority		*/
			if( TEST_BIT(pptr->rt.flags,MRT_MBX_SND) )
				p = pptr->pto;
			else if( TEST_BIT(pptr->rt.flags,MRT_MBX_RCV) && 
				(pptr->pfrom != pptr))
				p = pptr->pfrom;
			else if( TEST_BIT(pptr->rt.flags,MRT_SEM_DOWN))
				{
				if( pptr->psem->carrier != NULL)
					p = pptr->psem->carrier;
				else
					return;
				}
			else
				return;
			MRT_inherit(prty, p);
			}
		}
}

/*===========================================================================*
 *				MRT_disinherit						     *
 * Changes (or not) the priority of the proccess to the highest priority 	*
 * (lower value) among the:									*
 *      	- highest mqe									* 
 * 		- highest priority proccess waiting on a locked semaphore 	 	*
 *       	- The process base priority							* 
 *===========================================================================*/
PUBLIC void MRT_disinherit(pptr)
struct proc *pptr;
{
	MRT_mqe_t *pmqe;
	MRT_sem_t	*psem;
	struct proc *rp, *rpp;
	link_t *lk;
	priority_t	prty, sem_prty, mq_prty;

	if(!TEST_BIT(MRT_sv.flags,MRT_BPIP)) return;

	sem_prty = mq_prty = MRT_PRILOWEST;

	/* Searches the highest priority proccess waiting in all locked semaphores	*/
	for(lk = rp->slockL.first; lk != NULL; lk = lk->next)
		{
		psem = (MRT_sem_t	*)lk->this;
		if((rpp = MRT_sproc_pick(psem)) != NULL)
			sem_prty = MIN(sem_prty, rpp->rt.priority);
		}

	/* Gets the highest priority message waiting in the process message Queue	*/
	if( (pmqe = MRT_mqe_pick(pptr->pmq, MRT_ANYPROC)) != NULL) /* Message waiting?*/
		mq_prty = pmqe->msgd.hdr.priority;		/* highest prty message		*/

	prty = MIN(sem_prty,mq_prty);				/* highest prty (lower value)	*/  
	prty = MIN(prty,pptr->rt.baseprty);			

	if( prty != pptr->rt.baseprty)			/* base priority 			*/
		{
		if( MRT_is_rtready(pptr))			/* the process is ready		*/
			{
			MRT_rdyQ_rmv(pptr);
			pptr->rt.priority = prty;
			MRT_rdyQ_ins(pptr);
			}
		else							/* the process is not ready	*/
			{
			pptr->rt.priority = prty;
			}
		}  
}

#endif /* MRT */

