/*==========================================================================*
 *					mrttask.c						    *
 * This task handles the interface between user programs an the RTkernel    *
 * RT-System related services are obtained by sending a message specifying  *
 * what is needed.									    *  
 *==========================================================================*/

#include "kernel.h"
#include <signal.h>
#include <unistd.h>
#include <sys/sigcontext.h>
#include <sys/ptrace.h>
#include <minix/boot.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include "proc.h"
#if (CHIP == INTEL)
#include "protect.h"
#endif

#ifdef MRT

/* #define	MRT_TASKDBG	1		Remove comments for debugging			*/

PRIVATE message m;

FORWARD _PROTOTYPE( void MRT_sstat_init, (void ) );

FORWARD _PROTOTYPE( int MRT_getiattr, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getistat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getiint, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_setiattr, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getsstat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getsval, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_setsval, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_restart, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getpattr, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_setpattr, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getpstat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getpint, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_clrpstat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_RTstart, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_RTstop, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_exit, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_stdsignal, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_semalloc, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_semfree, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getsemstat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getsemint, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getsemid, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getsemattr, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_getmqstat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_clrsstat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_clristat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_clrsemstat, (message *m_ptr) );
FORWARD _PROTOTYPE( int MRT_clrmqstat, (message *m_ptr) );

 _PROTOTYPE (int (*MRT_vector[]), (message *msg) )= {
				MRT_getiattr,	/*  100 = mrt_getiattr	*/
				MRT_getistat,	/*  101 = mrt_getiarg	*/
				MRT_getiint,	/*  102 = mrt_getiarg	*/
				MRT_setiattr,	/*  103 = mrt_setiattr  */
				MRT_getsstat,	/*  104 = mrt_getsstat	*/
				MRT_getsval,	/*  105 = mrt_getsval	*/
				MRT_setsval,	/*  106 = mrt_setsval 	*/
				MRT_restart,	/*  107 = mrt_restart	*/
				MRT_getpattr,	/*  108 = mrt_getpattr	*/
				MRT_setpattr,	/*  109 = mrt_setpattr	*/
				MRT_getpstat,	/*  110 = mrt_getpstat	*/
				MRT_getpint,	/*  111 = mrt_getpint	*/
				MRT_clrpstat,	/*  112 = mrt_clrpstat	*/
				MRT_RTstart,	/*  113 = mrt_RTstart	*/
				MRT_RTstop,		/*  114 = mrt_RTstop	*/
				MRT_exit,		/*  115 = mrt_exit	*/
				MRT_stdsignal,	/*  116 = mrt_stdsignal */
				MRT_semalloc,	/*  117 = mrt_semalloc  */
				MRT_semfree,	/*  118 = mrt_semfree   */
				MRT_getsemstat,	/*  119 = mrt_getsemstat*/
				MRT_getsemint,	/*  120 = mrt_getsemint */
				MRT_getsemid,	/*  121 = mrt_getsemid  */
				MRT_getsemattr,  	/*  122 = mrt_getsemattr*/
				MRT_getmqstat,	/*  123 = mrt_getmqstat */
				MRT_clrsstat,	/*  124 = mrt_clrsstat	*/
				MRT_clristat,	/*  125 = mrt_clristat	*/
				MRT_clrsemstat,	/*  126 = mrt_clrsemstat*/
				MRT_clrmqstat,	/*  127 = mrt_clrmqstat */
				};

 int sys_run_mode[]= {
				MRT_RTMODE,	/*  100 = mrt_getiattr	*/
				MRT_RTMODE,	/*  101 = mrt_getiarg	*/
				MRT_RTMODE,	/*  102 = mrt_getiarg	*/
				MRT_RTMODE,	/*  103 = mrt_setiattr  */
				MRT_RTMODE,	/*  104 = mrt_getsstat	*/
				MRT_RTMODE,	/*  105 = mrt_getsval	*/
				0,		/*  106 = mrt_setsval 	*/
				MRT_RTMODE,	/*  107 = mrt_restart	*/
				MRT_RTMODE,	/*  108 = mrt_getpattr	*/
				MRT_RTMODE,	/*  109 = mrt_setpattr	*/
				MRT_RTMODE,	/*  110 = mrt_getpstat	*/
				MRT_RTMODE,	/*  111 = mrt_getpint	*/
				MRT_RTMODE,	/*  112 = mrt_clrpstat	*/
				0,		/*  113 = mrt_RTstart	*/
				MRT_RTMODE,	/*  114 = mrt_RTstop	*/
				MRT_RTMODE,	/*  115 = mrt_exit	*/
				MRT_RTMODE,	/*  116 = mrt_stdsignal */
				MRT_RTMODE,	/*  117 = mrt_semalloc  */
				MRT_RTMODE,	/*  118 = mrt_semfree   */
				MRT_RTMODE,	/*  119 = mrt_getsemstat*/
				MRT_RTMODE,	/*  120 = mrt_getsemint */
				MRT_RTMODE,	/*  121 = mrt_getsemid  */
				MRT_RTMODE, /*  122 = mrt_getsemattr*/
				MRT_RTMODE,	/*  123 = mrt_getmqstat */
				MRT_RTMODE,	/*  124 = mrt_clrsstat	*/
				MRT_RTMODE,	/*  125 = mrt_clristat	*/
				MRT_RTMODE,	/*  126 = mrt_clrsemstat*/
				MRT_RTMODE,	/*  127 = mrt_clrmqstat */
				};

int proc_run_mode[]= {
				0,	/*  100 = mrt_getiattr	*/
				0,	/*  101 = mrt_getiarg	*/
				0,	/*  102 = mrt_getiarg	*/
				0,	/*  103 = mrt_setiattr  */
				0,	/*  104 = mrt_getsstat	*/
				0,	/*  105 = mrt_getsval	*/
				0,	/*  106 = mrt_setsval 	*/
				0,	/*  107 = mrt_restart	*/
				0,	/*  108 = mrt_getpattr	*/
				0,	/*  109 = mrt_setpattr	*/
				0,	/*  110 = mrt_getpstat	*/
				0,	/*  111 = mrt_getpint	*/
				0,	/*  112 = mrt_clrpstat	*/
				0,	/*  113 = mrt_RTstart	*/
				0,	/*  114 = mrt_RTstop	*/
				MRT_P_REALTIME,	/*  115 = mrt_exit	*/
				0,	/*  116 = mrt_stdsignal */
				0,	/*  117 = mrt_semalloc  */
				0,	/*  118 = mrt_semfree   */
				0,	/*  119 = mrt_getsemstat*/
				0,	/*  120 = mrt_getsemint */
				0,	/*  121 = mrt_getsemid  */
				0,  	/*  122 = mrt_getsemattr*/
				0,	/*  123 = mrt_getmqstat */
				0,	/*  124 = mrt_clrsstat	*/
				0,	/*  125 = mrt_clristat	*/
				0,	/*  126 = mrt_clrsemstat*/
				0,	/*  127 = mrt_clrmqstat */
				};

/* This task handles the interface between Real Time Process and kernel
 * System services are obtained by sending
 * MRT_sys_task() a message specifying what is needed. 
 * The message types and parameters are:
 *
 *   MRT_GETIATTR	 Gets the IRQ processing Attributes
 *   MRT_GETISTAT	 Gets the IRQ statistics
 *   MRT_GETIINT	 Gets the IRQ internal use fields
 *   MRT_SETIATTR	 Sets the IRQ processing Attributes
 *   MRT_GETSSTAT	 Gets System wide statistics
 *   MRT_GETSVAL	 Gets System wide Processing Values
 *   MRT_SETSVAL   Sets System wide Processing Flags
 *   MRT_RESTART   Restart all statistics and the timer frequency
 *   MRT_GETPATTR	 Gets Process Attributes
 *   MRT_SETPATTR	 Sets Process Attributes
 *   MRT_GETPSTAT	 Gets Process Statistics
 *   MRT_GETPINT	 Gets Process Internal variables
 *   MRT_CLRPSTAT	 Clears Process Statistics
 *   MRT_RTSTART	 Puts the system in RT processing mode
 *   MRT_RTSTOP  	 Puts the system in STANDARD processing mode
 *   MRT_EXIT  	 Change the process type to NRT - used by the _exit() system call
 *   MRT_STDSIGNAL Change the process type to NRT - used by the signal_RT() by the MM 
 *   MRT_SEMALLOC  Alloc a Semaphore 
 *   MRT_SEMINT	 Gets Semaphore internal use fields
 *   MRT_SEMSTAT	 Gets the Semaphore statistics
 *   MRT_SEMFREE 	 Frees a Semaphore
 *   MRT_GETSEMID	 Gets the ID of an allocated Semaphore
 *   MRT_SEMATTR	 Gets the Semaphore Processing Attributes
 *   MRT_MSGQSTAT	 Gets the Message Queue statistics
 *   MRT_CLRSSTAT	 Clears System Wide Statistics
 *   MRT_CLRISTAT	 Clears Interrupt Descriptor Statistics
 *   MRT_CLRSEMSTAT Clears Semaphore Statistics
 *   MRT_CLRMQSTAT Clears Message Queue Statistics
 *
 * Message types and parameters:
 *
 *    m_type          m1_i1     m1_i2     m1_i3       m1_p1
 * ----------------+---------+---------+---------+--------------
 * | MRT_GETIATTR  |  caller | irq_nbr |         |             |
 * |               |  CALLER | IRQNBR  |         |   PIRQATTR  |
 * ----------------+---------+---------+---------+--------------
 * | MRT_GETISTAT  |  caller | irq_nbr |         |             |
 * |               |  CALLER | IRQNBR  |         |   PIRQSTAT  |
 * ----------------+---------+---------+---------+--------------
 * | MRT_GETIINT   |  caller | irq_nbr |         |             |
 * |               |  CALLER | IRQNBR  |         |   PIRQINT   |
 * ----------------+---------+---------+---------+--------------
 * | MRT_SETIATTR  |  caller | irq_nbr |         |             |
 * |               |  CALLER | IRQNBR  |         |   PIRQATTR  |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETSSTAT  |  caller |         |         |             |
 * |               |  CALLER |         |         |   PSYSSTAT  |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETSVAL   |  caller |         |         |             |
 * |               |  CALLER |         |         |   PSYSVAL   |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_SETSVAL   |  caller |  flags  |         |             |
 * |               |  CALLER |  FLAGS  |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_RESTART   |  caller | harmonic| refresh |             |
 * |               |  CALLER | HARMONIC| REFRESH |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETPATTR  |  caller |  pid    |         |             |
 * |               |  CALLER | PIDNBR  |         |  PPROCATTR  |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_SETPATTR  |  caller |  pid    |         |             |
 * |               |  CALLER | PIDNBR  |         |  PPROCATTR  |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETPSTAT  |  caller |  pid    |         |             |
 * |               |  CALLER | PIDNBR  |         |  PPROCSTAT  |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETPINT   |  caller |  pid    |         |             |
 * |               |  CALLER | PIDNBR  |         |  PPROCINT   |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_CLRPSTAT  |  caller |  pid    |         |             |
 * |               |  CALLER | PIDNBR  |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_RTSTART   |  caller | harmonic| refresh |             |
 * |               |  CALLER | HARMONIC| REFRESH |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_EXIT      |  caller |         |         |             |
 * |               |  CALLER |         |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_STDSIGNAL |  caller |  pid    |         |             |
 * |               |  CALLER | PIDNBR  |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_SEMALLOC  |  caller |         |         |             |
 * |               |  CALLER |         |         |   SEMATTR   |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_SEMFREE   |  caller | semid   |         |             |
 * |               |  CALLER | SEMID   |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETSEMSTAT|  caller | semid   |         |             |
 * |               |  CALLER | SEMID   |         |   SEMSTAT   |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETSEMINT |  caller | semid   |         |             |
 * |               |  CALLER | SEMID   |         |   SEMINT    |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETSEMID  |  caller |         |         |             |
 * |               |  CALLER |         |         |   SEMNAME   |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETSEMATTR|  caller | semid   |         |             |
 * |               |  CALLER | SEMID   |         |   SEMATTR   |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_GETMQSTAT |  caller | msgqid  |         |             |
 * |               |  CALLER | MSGQID  |         |   MSGQSTAT  |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_CLRSSTAT  |  caller |         |         |             |
 * |               |  CALLER |         |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_CLRISTAT  |  caller | irq_nbr |         |             |
 * |               |  CALLER | IRQNBR  |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_CLRSEMSTAT|  caller | semid   |         |             |
 * |               |  CALLER | SEMID   |         |             |
 * |---------------+---------+---------+---------+-------------|
 * | MRT_CLRMQSTAT |  caller | msgqid  |         |             |
 * |               |  CALLER | MSGQID  |         |             |
 * |---------------+---------+---------+---------+-------------|
 */


PRIVATE message msg;

/*===========================================================================*
 *				MRT_sys_task						     *
 *===========================================================================*/
PUBLIC void MRT_sys_task()
{
/* Main entry point of MRT_sys_task.  Get the message and dispatch on type. */

  register int r;
  int	mrt_call, error;
  struct proc *caller;

  printf("Starting MRTTASK ..\n");

  MRT_sstat_init();					/* Initialize the IDLE process	*/
  MRT_sv.refresh 			= HZ;			/* IDLE refresh counter			*/
  MRT_idle_init();					/* Initialize the IDLE process	*/

  for( r=0; r < NR_IRQ_VECTORS; r++) 		/* Initialize the System Interrupt masks */
	MRT_si.mask[r] = 0;  

  MRT_rdyQ_init();					/* Initialize the Ready Queue		*/
  MRT_msgQ_init();					/* Initialize the Message Queues	*/
  MRT_mpool_init();					/* Initialize the message Pool	*/
  MRT_spool_init();					/* Initialize the semaphore Pool	*/
  
  while (TRUE) {
	receive(ANY, &msg);				/* Get Work					*/
	mrt_call =  msg.m_type - MRT_FIRSTCALL;	/* RT calls					*/
	caller = MRT_pid2pptr(msg.CALLER);

#ifdef MRT_TASKDBG
printf("mrttask: call nbr: %d\n",msg.m_type);
#endif

	if (mrt_call < 0 || mrt_call >= MRT_NCALLS)	/* Check the RT-syscall number*/
		error = EBADCALL;
	else if ( (MRT_sv.flags & MRT_RTMODE) != sys_run_mode[mrt_call] ) /* check system MODE */
		error = (sys_run_mode[mrt_call] == MRT_RTMODE)?E_MRT_NORTMODE:E_MRT_RTMODE;
	else if ( MRT_is_realtime(caller)     != proc_run_mode[mrt_call]) /* check process MODE */
		error = E_MRT_BADPTYPE;
	else
		error = (*MRT_vector[mrt_call])(&msg);

 	msg.m_type = error;				/* 'r' reports status of call 	*/
	send(msg.m_source, &msg);			/* send reply to caller 		*/
  }
}

/*===========================================================================*
 *				MRT_sstat_init()						     	*
 * Reset System wide Statistic fields							*
 *===========================================================================*/
PRIVATE void MRT_sstat_init(void)
{
  MRT_sv.counter.scheds		= 0;
  MRT_sv.counter.messages	= 0;
  MRT_sv.counter.interrupts	= 0;
  MRT_sv.counter.ticks		= 0;
  MRT_sv.counter.highticks	= 0;
  MRT_sv.counter.oneshots	= 0;
}

/*===========================================================================*
 *				MRT_getiattr						     *
 * Get the Functional Attributes of an IRQ descriptor				     *
 *===========================================================================*/
PRIVATE int MRT_getiattr(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_irqattr_t attr;
	MRT_irqd_t  *p;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/


	if( m_ptr->IRQNBR < 0 || m_ptr->IRQNBR >=  (NR_IRQ_VECTORS+NR_IRQ_SOFT))
		return(E_MRT_BADIRQ);

	p = irqd_addr(m_ptr->IRQNBR);

	attr.period		= p->period;
	attr.task 		= p->task;
	attr.watchdog	= p->watchdog;
	attr.priority	= p->priority;
	attr.irqtype	= p->irqtype;
	strncpy(attr.name,p->name,MAXPNAME);

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->PIRQATTR, &attr, sizeof(mrt_irqattr_t));
	return(OK);
}

/*===========================================================================*
 *				MRT_getistat						     *
 * Gets IRQ descriptor	staticstics							     *
 *===========================================================================*/
PRIVATE int MRT_getistat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_irqstat_t statd;
	MRT_irqd_t  *p;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	if( m_ptr->IRQNBR < 0 || m_ptr->IRQNBR >=  (NR_IRQ_VECTORS+NR_IRQ_SOFT))
		return(E_MRT_BADIRQ);

	p = irqd_addr(m_ptr->IRQNBR);

	statd.count		= p->count;
	statd.maxshower	= p->maxshower;
	statd.mdl  		= p->mdl;
	statd.timestamp   = p->timestamp;
	statd.maxlat	= p->maxlat;
	statd.reenter	= p->reenter;

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->PIRQSTAT, &statd, sizeof(mrt_irqstat_t));

	return(OK);
}

/*===========================================================================*
 *				MRT_getiint							     *
 * Gets IRQ descriptor internal fields						     *
 *===========================================================================*/
PRIVATE int MRT_getiint(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_irqint_t intd;
	MRT_irqd_t  *pirqd;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	if( m_ptr->IRQNBR < 0 || m_ptr->IRQNBR >=  (NR_IRQ_VECTORS+NR_IRQ_SOFT))
		return(E_MRT_BADIRQ);

	pirqd = irqd_addr(m_ptr->IRQNBR);

	intd.irq		= pirqd->irq;
	intd.harmonic	= pirqd->harmonic;
	intd.vtimer		= pirqd->pvt->index;
	intd.flags		= pirqd->flags;

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->PIRQINT, &intd, sizeof(mrt_irqint_t));
	return(OK);
}

/*===========================================================================*
 *				MRT_setiattr						     *
 * sets de attributes of an IRQ descriptor					     *
 *===========================================================================*/
PRIVATE int MRT_setiattr(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_irqattr_t attr;
	MRT_irqd_t  *p;
	struct proc *caller, *rp;
	phys_bytes user_phys;			/* used by USR2KRN Macro		*/
	static long int CPUflags;		/* used by USR2KRN Macro		*/
	
	/*-------------- PARAMETERS VERIFICATION ------------------------------*/

	if( m_ptr->IRQNBR < 0 || m_ptr->IRQNBR >=  NR_IRQ_VECTORS)
		return(E_MRT_BADIRQ);
	
	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	USR2KRN(caller, &attr, m_ptr->PIRQATTR, sizeof(mrt_irqattr_t));

	/* check all parameters */
	if ( attr.task < (-NR_TASKS) )		/* check the task	number	*/	
		return(E_MRT_BADTASK);
	rp = proc_addr(attr.task);
	if( rp->p_flags == P_SLOT_FREE) 		
		return(E_MRT_BADTASK);

	if ( 	attr.priority  > MRT_PRILOWEST ||	/* check the priority		*/ 
		attr.priority <= MRT_PRIHIGHEST)
		return(E_MRT_BADPRTY);

	if( attr.watchdog != HARDWARE)		/* check watchdog process	*/
		{
		rp = proc_addr(attr.watchdog);
		if( rp->p_flags == P_SLOT_FREE)
			return(E_MRT_BADWDOG);
		}

	/*-------------- IRQ DESCRIPTOR SETTINGS  ------------------------------*/

	MRT_saveFlock(&CPUflags);			
	p = irqd_addr(m_ptr->IRQNBR);
	p->irqtype 		= attr.irqtype & (MRT_RTIRQ|MRT_TDIRQ|MRT_SOFTIRQ|MRT_ISTMSG);
	p->period		= attr.period;
	p->task 		= attr.task;
	p->watchdog		= attr.watchdog;
	p->priority		= attr.priority;
	strncpy(p->name,attr.name,MAXPNAME);
	MRT_restoreF(&CPUflags);		
	
	return(OK);
}

/*===========================================================================*
 *				MRT_getsstat 						     *
 * Get system wide statistics									*
 *===========================================================================*/
PRIVATE int MRT_getsstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_sysstat_t ss;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	ss.scheds		= MRT_sv.counter.scheds;
	ss.messages		= MRT_sv.counter.messages;
	ss.interrupts	= MRT_sv.counter.interrupts;
	ss.ticks		= MRT_sv.counter.ticks;
	ss.highticks	= MRT_sv.counter.highticks;
	ss.idlelast		= MRT_sv.counter.idlelast;
	ss.idlemax		= MRT_sv.counter.idlemax;

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	KRN2USR(caller, m_ptr->PSYSSTAT, &ss, sizeof(mrt_sysstat_t));

	return(OK);
}

/*===========================================================================*
 *				MRT_getsval 						     *
 * Get system wide processing values 				                 *
 *===========================================================================*/
PRIVATE int MRT_getsval(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_sysval_t sv;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	sv.flags		= MRT_sv.flags;
	sv.virtual_PIC	= MRT_sv.virtual_PIC;
	sv.PIT_latency	= MRT_sv.PIT_latency;
	sv.PIT_latch	= MRT_sv.PIT_latch;
	sv.tickrate		= MRT_sv.tickrate;
	sv.harmonic		= MRT_sv.harmonic;
	sv.refresh 		= MRT_sv.refresh;

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	KRN2USR(caller, m_ptr->PSYSVAL, &sv, sizeof(mrt_sysval_t));

	return(OK);
}

/*===========================================================================*
 *				MRT_setsval 						     *
 * Set system wide processing values 				                 *
 *===========================================================================*/
PRIVATE int MRT_setsval(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	unsigned int flags;
	struct proc *caller;
	phys_bytes user_phys;

	MRT_sv.flags = m_ptr->FLAGS & (MRT_DBG232|MRT_LATENCY|MRT_BPIP|MRT_ONESHOT|MRT_PRTYMASK);

	return(OK);
}

/*===========================================================================*
 *				MRT_restart							     *
 * Initializes:											*
 *        Interrupt descriptors statistics						*
 *        System wide statistics								*
 * 	    Process statistics									*
 *        Priority queues statistics							*
 *        Virtual timers									*
 *  	    Message queues and Pool								*
 *  	    Semaphore Pool									*
 * 	    Reconfigures the timer								*
 *===========================================================================*/
PRIVATE int MRT_restart(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	int 				i, p;
	register struct proc 	*rp;
  	register int 		t;
	lcounter_t 			before,period;
	MRT_irqd_t  		*d;
	MRT_vtimer_t 		vt,*pvt;
	static long int 		CPUflags;

	/*============== CHECK ARGUMENTS =======================================*/

	if( m_ptr->HARMONIC <= 0 || m_ptr->HARMONIC >  MAXHARMONIC)
		return(E_MRT_BADHARM);

	if( m_ptr->REFRESH <= 0 || m_ptr->REFRESH >  MAXREFRESH)
		return(E_MRT_BADRFSH);

	MRT_saveFlock(&CPUflags); 		/* ENTER CRITICAL SECTION		*/
	
	/*============== CHECK IF A RT-PROCESS IS ACTIVE =======================*/
	for (rp = BEG_PROC_ADDR, t = -NR_TASKS; rp < END_PROC_ADDR; ++rp, ++t) 
		if (MRT_is_realtime(rp))
			{
			MRT_restoreF(&CPUflags); 
			return(E_MRT_RTACTIVE);
			}

	/*============== RESET SYSTEM WIDE STATISTICS AND COUNTERS =============*/
	MRT_sstat_init();	 

	/*============== RESET PROCESS DESCRIPTOR STATISTICS AND COUNTERS =======*/
	MRT_proc_init();	 

	/*============== UPDATE IDLE REFRESH COUNTER VTIMER PARAMETERS =========*/
	d = irqd_addr(MRT_sv.IDLE_soft);		/* gets the irqd address	*/
	if( MRT_sv.refresh != m_ptr->REFRESH)
		{
		d->period = m_ptr->REFRESH;
		d->period *=  MRT_sv.harmonic;
		d->period /=  m_ptr->HARMONIC;  
		}

	MRT_sv.refresh	= m_ptr->REFRESH; 
	MRT_idle_init();

	/*============== RESET READY QUEUES =====================================*/
	MRT_rdyQ_init();

	/*============== RESET VIRTUAL TIMERS DESCRIPTORS AND TIMERQ   ==========*/
	MRT_timerQ_init();
	MRT_vtimer_init();

	/*============== RESET INTERRUPT QUEUES  ================================*/
	MRT_irqQ_init();
	MRT_sv.real_PIC    = MRT_get_PIC();
	MRT_sv.virtual_PIC = MRT_sv.real_PIC;

	/*============== RESET INTERRUPT DESCRIPTOR STATISTICS AND COUNTERS =====*/
 	for(i = 0; i < (NR_IRQ_VECTORS+NR_IRQ_SOFT); i++)
      	{
		d = irqd_addr(i);

		if( i < NR_IRQ_VECTORS)					/* Hardware IRQ		*/
			{
			if ( !TEST_BIT(~MRT_sv.real_PIC,(1 << i)))
				continue;

			/* build the array of Interrupt Masks */
			for( p = MRT_PRIHIGHEST; p < d->priority ; p++) 
				MRT_si.mask[d->priority] |= MRT_si.mask[p]; 
			for( p = d->priority ; p <= MRT_PRILOWEST; p++)
				{	
				SET_BIT(MRT_si.mask[p],(1 << i));
				SET_BIT(MRT_si.mask[p],(1 << CASCADE_IRQ));
				}
			}

		if( (i >= NR_IRQ_VECTORS) 
		&& !TEST_BIT(MRT_si.soft_use,(1 << (i-NR_IRQ_VECTORS)))) 			
			continue;

		MRT_irqd_clr(d);	

		if( TEST_BIT(d->irqtype,MRT_TDIRQ))		/* Timer Driven IRQ?	*/		
			{
			period = d->period;
			if(!TEST_BIT(d->irqtype,MRT_RELATPER))/* Convert the period	*/
				{
				period *= m_ptr->HARMONIC;		
				period /= d->harmonic;  
				}
			d->period = period;  			/* update the period	*/
			d->harmonic= m_ptr->HARMONIC;  	/* update the harmonic	*/
			VTALLOC(pvt, vt, period, 0, MRT_ACT_IRQTRIG, i
				,HARDWARE, d->priority);
			if( pvt == NULL)
				panic("MRT_restart: cannot create a VT for TD IRQ ",i);
			MRT_vtimer_ins(pvt);			/* activate the VT	*/
			d->pvt = pvt;
			}
       	}

	/*============== COMPUTES PIT LATENCY =================================*/
	before = MRT_read_timer2();
	MRT_sv.PIT_latency = before - MRT_read_timer2();/* compute PIT latency */

	/*============== SET THE PRIORITY LEVEL ======================================*/
	SET_BIT(MRT_sv.flags,MRT_RTMODE); 
	MRT_prtylvl_set(MRT_PRILOWEST); 

	/*============== INITIALIZE THE PIT FOR ONESHOT OR PERIODIC MODE =============*/
	MRT_reset_irq(CLOCK_IRQ);
	if( TEST_BIT(MRT_sv.flags,MRT_ONESHOT))
		{
		MRT_sv.harmonic = m_ptr->HARMONIC;
		MRT_sv.tick2Hz = (TIMER_FREQ/HZ);					
		MRT_sv.tick2Hz /= MRT_sv.harmonic;		/* convert [vtick]->[Hz]	*/
		}
	else
		{
	 	MRT_timer_set(m_ptr->HARMONIC);
		MRT_timer2_set(0xFFFF);
		}
	MRT_set_irq(CLOCK_IRQ);

	/*============== RESET THE MESSAGE POOL AND MESSAGE QUEUES ===========*/
	MRT_msgQ_init();
	MRT_mpool_init();

	/*============== RESET THE SEMAPHORE POOL ============================*/
	MRT_spool_init();

	MRT_restoreF(&CPUflags); 		/* LEAVE CRITICAL SECTION		*/
	return(OK);
}

/*===========================================================================*
 *				MRT_getpattr						     *
 * get process attributes									*
 *===========================================================================*/
PRIVATE int MRT_getpattr(m_ptr)
message *m_ptr;			/* pointer to request message */
{
      register struct proc *proc, *caller;
	mrt_pattr_t pattrib;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	if ( (proc = MRT_pid2pptr(m_ptr->PIDNBR))== NULL) 		
			return(E_BAD_PROC);

	pattrib.flags 	= proc->rt.flags;
	pattrib.baseprty 	= proc->rt.baseprty;
	pattrib.period  	= proc->rt.period;
	pattrib.limit  	= proc->rt.limit;
	pattrib.deadline 	= proc->rt.deadline;
	pattrib.watchdog  = proc->rt.watchdog;
	pattrib.mq_size   = proc->pmq->size;
	pattrib.mq_flags  = proc->pmq->flags;

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->PPROCATTR, &pattrib, sizeof(mrt_pattr_t)); 

	return(OK);
}

/*===========================================================================*
 *				MRT_setpattr						     *
 * Sets process attributes									*
 *===========================================================================*/
PRIVATE int MRT_setpattr(m_ptr)
message *m_ptr;			/* pointer to request message */
{
      register struct proc *wd, *caller;
	mrt_pattr_t pattrib;
	phys_bytes user_phys;			/* used by USR2KRN Macro		*/
	static long int CPUflags;		/* used by USR2KRN Macro		*/

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* Copy from user memory to kernel memory	*/
	USR2KRN(caller, &pattrib, m_ptr->PPROCATTR, sizeof(mrt_pattr_t));

	if(TEST_BIT(pattrib.flags,MRT_P_REALTIME)) 	/* a new Real-Time Process */
		{						/* copy attributes to the process descriptor */  
		if ( pattrib.baseprty <= MRT_PRIHIGHEST || pattrib.baseprty > MRT_PRILOWEST)
			return(E_MRT_BADPRTY);

		if ( pattrib.watchdog != HARDWARE)		
			{
			wd = MRT_pid2pptr(pattrib.watchdog);
			if ( wd == NULL || 			/* Check for a running process */
			  !(MRT_is_realtime(wd)) ||		/* Chech for a RT process type */
			   (wd->p_nr == caller->p_nr))		/* Check for other process */
				return(E_MRT_BADWDOG);
			}

		caller->rt.priority   = pattrib.baseprty;
		caller->rt.flags 	    = (pattrib.flags & (MRT_P_PERIODIC | MRT_P_REALTIME));
		caller->rt.baseprty   = pattrib.baseprty;
		caller->rt.period     = pattrib.period;
		caller->rt.limit      = pattrib.limit;
		caller->rt.deadline   = pattrib.deadline;
		caller->rt.watchdog   = pattrib.watchdog;
		caller->mq.size	    = pattrib.mq_size;
		caller->mq.flags 	    = pattrib.mq_flags;

		if(TEST_BIT(pattrib.flags,MRT_P_PERIODIC))
			caller->rt.limit  = 0;
		else
			caller->rt.limit  = pattrib.limit;
		
		MRT_link_rst(&caller->link,(void *)caller,MRT_OBJPROC);		
		}
	return(OK);
}

/*===========================================================================*
 *				MRT_getpstat						     *
 * Gets process statistics									*
 *===========================================================================*/
PRIVATE int MRT_getpstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
      register struct proc *rp, *caller;
	mrt_pstat_t pstats;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	if ( (rp = MRT_pid2pptr(m_ptr->PIDNBR))== NULL) 		
			return(E_BAD_PROC);

	pstats.scheds  	= rp->st.scheds;
	pstats.timestamp  = rp->st.timestamp;
	pstats.maxlat     = rp->st.maxlat;
	pstats.minlax     = rp->st.minlax;
	pstats.msgsent    = rp->st.msgsent;
	pstats.msgrcvd    = rp->st.msgrcvd;

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->PPROCSTAT, &pstats, sizeof(mrt_pstat_t)); 
	
	return(OK);

}

/*===========================================================================*
 *				MRT_getpint 						     *
 * Gets process internall variables								*
 *===========================================================================*/
PRIVATE int MRT_getpint(m_ptr)
message *m_ptr;			/* pointer to request message */
{
      register struct proc *rp, *caller;
	mrt_pint_t pint;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	if ( (rp = MRT_pid2pptr(m_ptr->PIDNBR))== NULL) 		
			return(E_BAD_PROC);

	pint.vt	  	= rp->pvt->index;
	pint.priority     = rp->rt.priority;
	pint.mqID 	      = rp->pmq->index;
	pint.p_nr		= rp->p_nr;

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->PPROCINT, &pint, sizeof(mrt_pint_t)); 
	
	return(OK);
}

/*===========================================================================*
 *				MRT_clrpstat						     *
 * Clears process statistics									*
 *===========================================================================*/
PRIVATE int MRT_clrpstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
      register struct proc *rp;

	if ( (rp = MRT_pid2pptr(m_ptr->PIDNBR))== NULL) 		
			return(E_BAD_PROC);

	rp->st.scheds	= 0;
	rp->st.timestamp 	= 0;
	rp->st.maxlat	= 0;
	rp->st.minlax 	= 0;
	rp->st.msgsent 	= 0;
	rp->st.msgrcvd 	= 0;
	
	return(OK);
}


/*===========================================================================*
 *				MRT_RTstart							     *
 * Start Real Time processing mode								*
 *===========================================================================*/
PRIVATE int MRT_RTstart (m_ptr)
message *m_ptr;			/* pointer to request message */
{
	return(MRT_restart(m_ptr));
}

/*===========================================================================*
 *				MRT_RTstop							     *
 * Stop Real Time processing mode								*
 *===========================================================================*/
PRIVATE int MRT_RTstop (m_ptr)
message *m_ptr;			/* pointer to request message */
{
	register struct proc *rp;
  	register int t;
	static long int CPUflags;

	MRT_saveFlock(&CPUflags);

	/*============== CHECK IF A RT PROCESS IS ACTIVE =======================*/
	for (rp = BEG_PROC_ADDR, t = -NR_TASKS; rp < END_PROC_ADDR; ++rp, ++t) 
		if (MRT_is_realtime(rp))
			{
			MRT_restoreF(&CPUflags);
			return(E_MRT_RTACTIVE);
			}

 	MRT_timer_set(1);
	CLR_BIT(MRT_sv.flags,MRT_RTMODE);

	MRT_restoreF(&CPUflags); 
	return(OK);
}

/*===========================================================================*
*				MRT_exit 							     *
*===========================================================================*/
PRIVATE int MRT_exit(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	static long int CPUflags;
      register struct proc *rp;
	MRT_sem_t	*psem;
	link_t *lk;

	if ( (rp = proc_addr(m_ptr->CALLER))== NULL) 		
			return(E_BAD_PROC);

	MRT_saveFlock(&CPUflags);
	MRT_set2nrt(rp);

	for( lk = rp->sallocL.first; lk != NULL; lk = rp->sallocL.first)
		{
		psem = (MRT_sem_t	*)lk->this;
		MRT_sem_free(rp,psem);
		}

	MRT_proc_rst(rp);
	MRT_restoreF(&CPUflags); 
	return(OK);
}

/*===========================================================================*
*				MRT_stdsignal						     *
* Sets the state of a process to MRT_P_STANDARD - used by the MM before     
*
*===========================================================================*/
PRIVATE int MRT_stdsignal(m_ptr)
message *m_ptr;					/* pointer to request message 	*/
{
      register struct proc *rp;

	if ( (rp = MRT_pid2pptr(m_ptr->PIDNBR))== NULL)
			return(E_BAD_PROC);

	return(MRT_set2nrt(rp));
}


/*===========================================================================*
 *				MRT_semalloc						     *
 * Allocs a semaphore with the specified attributes 				     *
 *===========================================================================*/
PRIVATE int MRT_semalloc(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	int i;
	mrt_semattr_t sattr;
	struct proc *caller;
	MRT_sem_t 	*psem;
	phys_bytes user_phys;			/* used by USR2KRN Macro		*/
	static long int CPUflags;		/* used by USR2KRN Macro		*/
	
	/*-------------- PARAMETERS VERIFICATION ------------------------------*/
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from user memory to kernel memory */
	USR2KRN(caller, &sattr, m_ptr->SEMATTR, sizeof(mrt_semattr_t));

	for (i = 0; i < NR_SEM; i++)		/* Test if the semaphore name exists	*/
		if(strcmp(MRT_ss.sem[i].name, sattr.name) == 0)
			return(E_MRT_BADNAME);

	if( TEST_BIT(sattr.flags,SEM_MUTEX) && sattr.value > 1)
		{
		MRT_restoreF(&CPUflags);		/* LEAVE CRITICAL SECTION		*/
		return(E_MRT_BADVALUE);
		}

	if( TEST_BIT(sattr.flags,SEM_PRTYINHERIT) )
		{
		if (!TEST_BIT(sattr.flags,(SEM_MUTEX | SEM_PRTYORDER)))
			{
			MRT_restoreF(&CPUflags);		/* LEAVE CRITICAL SECTION		*/
			return(E_MRT_BADFLAGS);
			}
		}

	if( sattr.priority > MRT_PRILOWEST || sattr.priority < MRT_PRIHIGHEST ) 
		{
		MRT_restoreF(&CPUflags);		/* LEAVE CRITICAL SECTION		*/
		return(E_MRT_BADPRTY);
		}

	/*-------------- SEM DESCRIPTOR SETTINGS  ------------------------------*/

	MRT_saveFlock(&CPUflags);			/* ENTER CRITICAL SECTION		*/
	
	if( (psem = MRT_sem_alloc(caller)) == NULL)
		{
		MRT_restoreF(&CPUflags);		/* LEAVE CRITICAL SECTION		*/
		return(E_MRT_NOSEM);
		}

	psem->flags = sattr.flags & (SEM_PRTYORDER|SEM_PRTYINHERIT|SEM_MUTEX);
	psem->value = sattr.value;
	psem->priority = sattr.priority ;
	strncpy(psem->name,sattr.name,MAXPNAME);

	MRT_restoreF(&CPUflags);			/* LEAVE CRITICAL SECTION		*/
	return(psem->index);
}

/*===========================================================================*
 *				MRT_getsemstat						     *
 * Get the Statistics of a specified Semaphore 					     *
 *===========================================================================*/
PRIVATE int MRT_getsemstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_semstat_t sstat;
	MRT_sem_t  *psem;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	if( m_ptr->SEMID < 0 || m_ptr->SEMID >=  NR_SEM )
		return(E_MRT_BADSEM);

	psem = sem_addr(m_ptr->SEMID);

	sstat.ups 		= psem->ups;
	sstat.downs		= psem->downs;
	sstat.maxinQ	= psem->plist.maxinQ;

	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->SEMSTAT, &sstat, sizeof(mrt_semstat_t));    

	return(OK);
}

/*===========================================================================*
 *				MRT_getsemint						     *
 * Get the Internal data of a specified Semaphore 				     *
 *===========================================================================*/
PRIVATE int MRT_getsemint(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_semint_t sint;
	MRT_sem_t  *psem;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/


	if( m_ptr->SEMID < 0 || m_ptr->SEMID >=  NR_SEM )
		return(E_MRT_BADSEM);

	psem = sem_addr(m_ptr->SEMID);

	sint.index 		= psem->index;
	sint.owner		= psem->owner;
	sint.inQ		= psem->plist.inQ;

	caller = MRT_pid2pptr(m_ptr->CALLER);
	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->SEMINT, &sint, sizeof(mrt_semint_t));    

	return(OK);
}

/*===========================================================================*
 *				MRT_semfree							     *
 * Frees a specified Semaphore 							     *
 *===========================================================================*/
PRIVATE int MRT_semfree(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	MRT_sem_t  *psem;
	struct proc *caller;
	static long int CPUflags;

	caller = MRT_pid2pptr(m_ptr->CALLER);

	if( m_ptr->SEMID < 0 || m_ptr->SEMID >=  NR_SEM )
		return(E_MRT_BADSEM);
		
	psem = sem_addr(m_ptr->SEMID);

	if( psem->owner == HARDWARE) return(E_MRT_FREESEM);
	if( psem->owner != caller->p_nr) return(EPERM);

	MRT_saveFlock(&CPUflags);		/* Start Critical Section */
	MRT_sem_free(caller, psem);
	MRT_restoreF(&CPUflags);		/* End   Critical Section */

	return(OK);
}

/*===========================================================================*
 *				MRT_getsemid						     *
 * Gets the semaphore ID of an allocated semaphore 				     *
 *===========================================================================*/
PRIVATE int MRT_getsemid(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	int i;
	struct proc *caller;
	char sname[MAXPNAME];
	phys_bytes user_phys;			/* used by USR2KRN Macro		*/
	static long int CPUflags;		/* used by USR2KRN Macro		*/
	
	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	/* copy from kernel memory to user memory */
	USR2KRN(caller, &sname, m_ptr->SEMNAME, MAXPNAME);    

	for (i = 0; i < NR_SEM; i++)		/* Test if the semaphore name exists	*/
		if(strcmp(MRT_ss.sem[i].name, sname) == 0)
			return(MRT_ss.sem[i].index);

	return(E_MRT_BADSEM);
}

/*===========================================================================*
 *				MRT_getsemattr						     *
 * Get the processing Attributes of a specified Semaphore 			     *
 *===========================================================================*/
PRIVATE int MRT_getsemattr(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_semattr_t sattr;
	MRT_sem_t  *psem;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	if( m_ptr->SEMID < 0 || m_ptr->SEMID >=  NR_SEM )
		return(E_MRT_BADSEM);

	psem = sem_addr(m_ptr->SEMID);

	if( psem->owner == HARDWARE) return(E_MRT_FREESEM);

	sattr.value 	= psem->value;
	sattr.flags 	= psem->flags;
	sattr.priority 	= psem->priority;
	strncpy(sattr.name,psem->name,MAXPNAME);

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->SEMATTR, &sattr, sizeof(mrt_semattr_t));    

	return(OK);
}

/*===========================================================================*
 *				MRT_getmqstat						     *
 * Get the Statistics of a specified Message Queue				     *
 *===========================================================================*/
PRIVATE int MRT_getmqstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	mrt_msgqstat_t mqstat;
	MRT_msgQ_t  *pmq;
	struct proc *caller;
	phys_bytes user_phys;			/* used by KRN2USR Macro		*/
	static long int CPUflags;		/* used by KRN2USR Macro		*/

	/* get the process pointer */
	caller = MRT_pid2pptr(m_ptr->CALLER);

	if( m_ptr->MSGQID < 0 || m_ptr->MSGQID >=  NR_MSGQ )
		return(E_MRT_BADMSGQ);

	pmq = msgQ_addr(m_ptr->MSGQID);

	mqstat.delivered		= pmq->delivered;
	mqstat.enqueued		= pmq->enqueued;
	mqstat.maxinQ		= pmq->plist.maxinQ;

	/* copy from kernel memory to user memory */
	KRN2USR(caller, m_ptr->MSGQSTAT, &mqstat, sizeof(mrt_msgqstat_t));    

	return(OK);
}

/*===========================================================================*
 *				MRT_clrsstat						     *
 * Clears System Wide statistics							     *
 *===========================================================================*/
PRIVATE int MRT_clrsstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	MRT_sstat_init();
	return(OK);
}

/*===========================================================================*
 *				MRT_clristat						     *
 * Clears IRQ descriptor statistics							     *
 *===========================================================================*/
PRIVATE int MRT_clristat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	MRT_irqd_t  *d;

	if( m_ptr->IRQNBR < 0 || m_ptr->IRQNBR >=  (NR_IRQ_VECTORS+NR_IRQ_SOFT))
		return(E_MRT_BADIRQ);

	d = irqd_addr(m_ptr->IRQNBR);
	irqd_clrstat(d);

	return(OK);
}

/*===========================================================================*
 *				MRT_clrsemstat						     *
 * Clears the Statistics of a specified Semaphore 				     *
 *===========================================================================*/
PRIVATE int MRT_clrsemstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	MRT_sem_t  *psem;

	if( m_ptr->SEMID < 0 || m_ptr->SEMID >=  NR_SEM )
		return(E_MRT_BADSEM);

	psem = sem_addr(m_ptr->SEMID);
	if( psem->owner == HARDWARE) return(E_MRT_FREESEM);

	psem->ups			= 0;
	psem->downs			= 0;
	psem->plist.maxinQ	= 0;

	return(OK);
}

/*===========================================================================*
 *				MRT_clrmqstat						     *
 * Clears the Statistics of a specified Message Queue				     *
 *===========================================================================*/
PRIVATE int MRT_clrmqstat(m_ptr)
message *m_ptr;			/* pointer to request message */
{
	MRT_msgQ_t  *pmq;

	if( m_ptr->MSGQID < 0 || m_ptr->MSGQID >=  NR_MSGQ )
		return(E_MRT_BADMSGQ);

	pmq = msgQ_addr(m_ptr->MSGQID);
	if( pmq->owner == MRTMSGQFREE) return(E_MRT_FREEMSGQ);

	pmq->delivered		= 0;
	pmq->enqueued		= 0;
	pmq->plist.maxinQ		= 0;

	return(OK);
}

#endif /* MRT */


