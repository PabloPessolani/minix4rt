/*==========================================================================*
 *					mrtsem.c 						    *
 * This file contains essentially all of the RT-semaphore handling	    *
 *==========================================================================*/

#include "kernel.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/type.h>
#include "proc.h"
#include "unistd.h"

#ifdef MRT

/*===========================================================================*
 *				MRT_spool_init	 					     *
 * Initialize the semaphore pool 							     * 
 *===========================================================================*/
PUBLIC void MRT_spool_init(void )
{
	int i;
	MRT_sem_t	*psem;

	MRT_list_rst(&MRT_ss.sfreeL);			/* resets the semaphore Free list	*/

	for ( i = 0;  i < NR_SEM ; i++)
		{ 
		psem 			= sem_addr(i);	
		psem->index 	= i;			/* assign the semaphore ID		*/
		MRT_sem_rst(psem);			/* reset all semaphore fields		*/
		MRT_list_app(&MRT_ss.sfreeL,&psem->alloclk); /* append into Free List	*/
		}
}

/*===========================================================================*
 *				MRT_sem_rst						     		*
 * Reset all fields of a semaphore						    		*
 *===========================================================================*/
PUBLIC void MRT_sem_rst(psem)
MRT_sem_t	*psem;
{
	psem->value 	= 0;
	psem->priority	= MRT_PRILOWEST;
	psem->flags		= 0;
	psem->owner		= HARDWARE;
	psem->ups		= 0;
	psem->downs		= 0;
	psem->carrier	= NULL;
	MRT_link_rst(&psem->alloclk,(void*)psem, MRT_OBJSEM);
	MRT_link_rst(&psem->locklk, (void*)psem, MRT_OBJSEM);
	strncpy(psem->name,"FREE",MAXPNAME-1);
	MRT_plist_rst(&psem->plist);		/* resets priority list fields	*/
}

/*===========================================================================*
 *				MRT_sem_alloc						     *
 * Alloc a RT Semaphore from the Semaphore Pool					     *
 *===========================================================================*/
PUBLIC MRT_sem_t *MRT_sem_alloc(rp)
MRT_proc_t *rp;
{
	MRT_sem_t *psem;
	link_t *plink;

	plink = MRT_ss.sfreeL.first;			/* The First from the Free List*/			
	if( plink != NULL)
		{
		MRT_list_rmv(&MRT_ss.sfreeL,plink);	/* Removes from Free List 	*/
		psem = (MRT_sem_t *)plink->this;
		psem->owner = rp->p_nr;			/* assing the semaphore owner	*/
		MRT_list_app(&rp->sallocL,plink);	/* Appends into proc Alloc List*/
		}
	else
		psem = NULL;				/* No free semaphore		*/

	return(psem);
}

/*===========================================================================*
 *				MRT_sem_free						     *
 * Frees a Semaphore to the Semaphore Pool					     *
 *===========================================================================*/
PUBLIC void MRT_sem_free(rp,psem)
MRT_proc_t *rp;
MRT_sem_t *psem;
{
	MRT_list_rmv(&rp->sallocL,&psem->alloclk);
	MRT_sem_rst(psem);				/* Resets semaphore fields	*/	
	MRT_list_app(&MRT_ss.sfreeL,&psem->alloclk); /* Appends into the Free List	*/
}

/*===========================================================================*
 *				MRT_sproc_app					     		*
 * Appends a waiting process  at the tail of the semaphore waiting queue 	*
 *===========================================================================*/
PUBLIC void MRT_sproc_app(psem, rp)
MRT_sem_t *psem;
MRT_proc_t *rp;
{
	int pty;

	if( TEST_BIT(psem->flags, SEM_PRTYORDER))	/* Priority order semaphore?	*/	
		pty = rp->rt.priority;			/* assing process priority	*/
	else
		pty = MRT_PRIHIGHEST;			/* only the highest priority	*/
								/* list is used			*/
	
	MRT_plist_app(&psem->plist,&rp->link,pty);/* appends the process into 	*/
								/* the semaphore priority list*/
}

/*===========================================================================*
 *				MRT_sproc_rmv					     		*
 * Removes a waiting process from the head of the semaphore waiting queue 	*
 *===========================================================================*/
PUBLIC void MRT_sproc_rmv(psem, rp)
MRT_sem_t	*psem;
MRT_proc_t *rp;
{
	int pty;
	
	if( TEST_BIT(psem->flags, MQ_PRTYORDER))	/* Priority order semaphore?	*/
		pty = rp->rt.priority;			/* assing process priority	*/
	else
		pty = MRT_PRIHIGHEST;			/* only the highest priority	*/
								/* list is used			*/

	if ( psem->plist.inQ == 0)			/* Empty priority list???	*/
		panic("MRT_sproc_rmv, inQ = 0 for semaphore ",psem->index);

	MRT_plist_rmv(&psem->plist,&rp->link,pty);/* Removes the process from 	*/
								/* the semaphore priority list*/
}

/*===========================================================================*
 *				MRT_sproc_pick					     		*
 * Pick the highest priority waiting process from the semaphore waiting queue *
 *===========================================================================*/
PUBLIC MRT_proc_t *MRT_sproc_pick(psem)
MRT_sem_t	*psem;
{
	link_t *plink;
	MRT_proc_t	 	*rp;
	
	plink  = MRT_plist_get(&psem->plist);	/* get the address of the 	*/
								/* highest priority process	*/
	if( plink == NULL ) 				/* Empty priority list??	*/
		return(NULL); 				/* No waiting process		*/

	rp = (MRT_proc_t *)plink->this;		/* highest priority process	*/
	return(rp);
}

/*===========================================================================*
 *				MRT_semup							     *
 * Ups a RT semaphore 			 						     *
 *===========================================================================*/
PUBLIC int MRT_semup(psem)
MRT_sem_t *psem;
{
	MRT_proc_t	 	*rp;

	if( psem->owner == HARDWARE ) 		/* The semaphore is FREE??	*/
		return(E_MRT_BADSEM);

	if( psem->value < 0)				/* Are there waiting process 	*/
		{						/* to get the semaphore?	*/
		if((rp = MRT_sproc_pick(psem)) == NULL) /*gets the first process	*/
			panic("MRT_semup: value<0 but any process waiting on semaphore "
				,psem->index);

		MRT_sproc_rmv(psem, rp);		/* Removes the process from 	*/
								/* semaphore priority list	*/
		MRT_vtimer_stop(rp->ipcvt);		/* Stops the process' IPC VT	*/
		rp->psem = NULL;				/* The process does not wait	*/
								/* for the semaphore		*/

		if( TEST_BIT(psem->flags,SEM_MUTEX)) 
			{
			MRT_list_rmv(&proc_ptr->slockL,&psem->locklk);
								/* Removes the sem		*/
								/* from the older process sem	*/
								/* List				*/
			psem->carrier = rp;		/* Set the new semaphore's 	*/
								/* carrier process		*/
			MRT_list_app(&rp->slockL,&psem->locklk);/* appends the sem	*/
								/* into the carrier proc List	*/

			if (TEST_BIT(psem->flags, SEM_PRTYINHERIT)) /* use BPIP?	*/
				MRT_disinherit(proc_ptr);/* Disinherit priority of the*/
								/* older semaphore carrier proc*/
			}

		CLR_BIT(rp->rt.flags,MRT_SEM_DOWN);	/* Clear the flag indicating	*/
								/* that the process is waiting*/
								/* a Semaphore			*/
		if(MRT_is_rtready(rp)) 			/* all flags clear?		*/
			MRT_ready(rp);			/* Insert the proc into the 	*/
								/* RT-ready priority list	*/
		}
	else if(psem->value == 0	&&		/* there are not any process	*/
		TEST_BIT(psem->flags,SEM_MUTEX))	/* waiting for this semaphore	*/
		 	{
			MRT_list_rmv(&proc_ptr->slockL,&psem->locklk);
								/* Removes the sem		*/
								/* from the older process sem	*/
								/* List				*/
			psem->carrier = NULL;		/* The semaphore have no any	*/
								/* carrier				*/
			}

	psem->ups++;					/* statistics for the semaphore*/
	psem->value++;					/* increase the semaphore value*/
	return(OK);
}

/*===========================================================================*
 *				MRT_semdown							     *
 * Downs a RT semaphore 		 						     *
 *===========================================================================*/
PUBLIC int MRT_semdown(psem, timeout)
MRT_sem_t *psem;
lcounter_t	timeout;
{

	if( psem->owner == HARDWARE ) 		/* The semaphore is FREE??	*/
		return(E_MRT_BADSEM);

	if( psem->value <= 0)				/* Is the semaphore locked?	*/
		{
		if( timeout == MRT_NOWAIT)		/* No wait 				*/ 
			return(E_TRY_AGAIN);

		if( timeout != MRT_FOREVER) 		/* Was a timeout specified	*/
			VTSET(proc_ptr->ipcvt, timeout, 1, 
				MRT_ACT_DOWN, psem->index, proc_ptr->rt.priority);

		SET_BIT(proc_ptr->rt.flags,MRT_SEM_DOWN); /* Set a process flag	*/
								/* indicating that the process*/
								/* is waiting for a semaphore	*/
		proc_ptr->psem = psem;			/* indicate from which sem	*/

 		MRT_unready(proc_ptr);			/* removes the process from 	*/
								/* from the RT-ready list	*/

		MRT_sproc_app(psem, proc_ptr);	/* appends the process into 	*/
								/* the semaphore list of 	*/
								/* waiting processes		*/

		if ( TEST_BIT(psem->flags, SEM_PRTYINHERIT)) /* use BPIP on MUTEX?*/
			MRT_inherit(proc_ptr->rt.priority, psem->carrier);
		}
	else if( psem->value == 1 && TEST_BIT(psem->flags,SEM_MUTEX) )
		{						/* Does the process lock the 	*/
								/* sem and be the carrier	*/
		psem->carrier = proc_ptr;		/* the process is the carrier	*/
		MRT_list_app(&proc_ptr->slockL,&psem->locklk);/* sem into proc List	*/
		}


	psem->downs++;					/* semaphore statistics		*/
	psem->value--;					/* decrease semaphore value	*/
	return(OK);
}

/*===========================================================================*
 *				MRT_sem_unlock						     *
 * Unlock process' locked semaphores - USED by mrt_set2nrt()		     *
 *===========================================================================*/
PUBLIC void MRT_sem_unlock(rp)
MRT_proc_t *rp;
{
	MRT_sem_t	*psem;
	link_t *lk;

	/*  Unlock process' locked Semaphores	*/
	for(lk = rp->slockL.first; lk != NULL; lk = rp->slockL.first)
		{
		psem = (MRT_sem_t	*)lk->this;
		MRT_semup(psem);				/* unlock locked semaphores	*/
		}
}

/*===========================================================================*
 *				MRT_sem_empty						     *
 * Wakeup waiting processes in allocated semaphores- USED by mrt_set2nrt()   *
 *===========================================================================*/
PUBLIC void MRT_sem_empty(rp)
MRT_proc_t *rp;
{
	struct proc *rpp;
	MRT_sem_t	*psem;
	link_t *lk;


	/*  Unlock waiting processes in allocated Semaphores	*/
	for(lk = rp->sallocL.first; lk != NULL; lk = lk->next)
		{
		psem = (MRT_sem_t	*)lk->this;
		while( (rpp = MRT_sproc_pick(psem)) != NULL)
			{
			MRT_sproc_rmv(psem, rpp);		/* removes from the sem's list */
			if( rpp->ipcvt != NULL)
				MRT_vtimer_stop(rpp->ipcvt);	/* stop the process timeout VT */

			rpp->psem = NULL;				/* the process does not wait for*/
									/* the semaphore			*/
			CLR_BIT(rpp->rt.flags,MRT_SEM_DOWN);/* clears the flag for semaphore*/
									/* waiting				*/
			if(MRT_is_rtready(rpp)) 		/* Are there all flags cleared? */
				MRT_ready(rpp);			/* Inserts the process into the*/
									/* RT-ready list			*/
			rpp->p_reg.retreg = E_MRT_EXITING;	/* Return code for the process*/
			}
		}
}

#endif /* MRT */

