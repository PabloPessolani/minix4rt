#ifndef PROC_H
#define PROC_H

/* Here is the declaration of the process table.  It contains the process'
 * registers, memory map, accounting, and message send/receive information.
 * Many assembly code routines reference fields in it.  The offsets to these
 * fields are defined in the assembler include file sconst.h.  When changing
 * 'proc', be sure to change sconst.h to match.
 */
#ifdef MRT
typedef struct {
	int 		flags;	/* Real Time Flags				*/
	priority_t	priority;	/* EFECTIVE Real Time priority		*/
	priority_t	baseprty;	/* BASE Real Time priority			*/
  	lcounter_t	period;	/* period in RT-ticks				*/
	scounter_t	limit;	/* maximun number of process schedulings  */
  	lcounter_t	deadline;   /* process deadline				*/
	pid_t 	watchdog;   /* Watchdog process				*/
	} rtattrib_t;

typedef struct {
  	lcounter_t	scheds;	/* number of RT schedules			*/
  	lcounter_t	mdl;		/* Missed DeadLines				*/
  	lcounter_t	timestamp;	/* Last schedule timestamp in ticks 	*/
  	lcounter_t	maxlat;	/* Maximun latency in timer Hz 		*/
  	lcounter_t	minlax;	/* Minimun laxity in timer Hz 		*/
  	lcounter_t	msgsent;	/* messages sent by the process		*/
  	lcounter_t	msgrcvd;	/* messages received by the process 	*/
	} rtstats_t;

typedef struct {
	scounter_t	size;		/* Message Queue Size				*/
   	unsigned int flags;	/* Message Queue Policy Flags			*/
 	} mqattrib_t;

#endif /* MRT */

struct proc {
  struct stackframe_s p_reg;	/* process' registers saved in stack frame */

#if (CHIP == INTEL)
  reg_t p_ldt_sel;		/* selector in gdt giving ldt base and limit*/
  struct segdesc_s p_ldt[2];	/* local descriptors for code and data 	*/
				/* 2 is LDT_SIZE - avoid include protect.h 	*/
#endif /* (CHIP == INTEL) */

#if (CHIP == M68000)
  reg_t p_splow;			/* lowest observed stack value 		*/
  int p_trap;			/* trap type (only low byte) 			*/
#if (SHADOWING == 0)
  char *p_crp;			/* mmu table pointer (really struct _rpr *)*/
#else
  phys_clicks p_shadow;		/* set if shadowed process image 		*/
  int align;			/* make the struct size a multiple of 4 	*/
#endif
  int p_nflips;			/* statistics 					*/
  char p_physio;			/* cannot be (un)shadowed now if set 	*/
#if defined(FPP)
  struct fsave p_fsave;		/* FPP state frame and registers 		*/
  int align2;			/* make the struct size a multiple of 4 	*/
#endif
#endif /* (CHIP == M68000) */

  reg_t *p_stguard;		/* stack guard word 				*/

  int p_nr;				/* number of this process (for fast access)*/

  int p_int_blocked;		/* nonzero if int msg blocked by busy task */
  int p_int_held;		/* nonzero if int msg held by busy syscall */
  struct proc *p_nextheld;	/* next in chain of held-up int processes */

  int p_flags;			/* P_SLOT_FREE, SENDING, RECEIVING, etc. */
  struct mem_map p_map[NR_SEGS];/* memory map */
  pid_t p_pid;			/* process id passed in from MM */

  clock_t user_time;		/* user time in ticks */
  clock_t sys_time;		/* sys time in ticks */
  clock_t child_utime;		/* cumulative user time of children 	*/
  clock_t child_stime;		/* cumulative sys time of children 		*/
  clock_t p_alarm;		/* time of next alarm in ticks, or 0 	*/

  struct proc *p_callerq;	/* head of list of procs wishing to send 	*/
  struct proc *p_sendlink;	/* link to next proc wishing to send 	*/
  message *p_messbuf;		/* pointer to message buffer 			*/
  int p_getfrom;		/* from whom does process want to receive? 	*/
  int p_sendto;

  struct proc *p_nextready;	/* pointer to next ready process 		*/
  sigset_t p_pending;		/* bit map for pending signals 		*/
  unsigned p_pendcount;		/* count of pending and unfinished signals */

  char p_name[16];		/* name of the process 				*/

#ifdef MRT
  rtattrib_t	rt;		/* Real Time Attributes 			*/
  rtstats_t		st;		/* Real Time Statistics  			*/
  mqattrib_t	mq;		/* Message Queue Attributes			*/

  MRT_msgQ_t 	*pmq;		/* Real Time Message Queue 			*/
  MRT_vtimer_t 	*pvt;		/* virtual timer ID for periodic process  */
  MRT_vtimer_t 	*ipcvt;	/* virtual timer ID for IPC kernel calls  */

  struct proc 	*pfrom;	/* from whom does RT-process want to receive? */
  struct proc	*pto;		/* to whom does RT-process want to send? 	*/

  MRT_sem_t		*psem;	/* the process is waiting a semaphore	*/
  mrt_msg_t		*pmsg;	/* pointer to message buffer 			*/
  mrt_mhdr_t	*pmhdr;	/* pointer to message header 			*/

  list_t		slockL;	/* List of locked semaphores			*/
  list_t		sallocL;	/* List of allocated semaphores		*/

  link_t		link; 	/* Process link					*/
#endif /* MRT */

};

/* Guard word for task stacks. */
#define STACK_GUARD	((reg_t) (sizeof(reg_t) == 2 ? 0xBEEF : 0xDEADBEEF))

/* Bits for p_flags in proc[].  A process is runnable iff p_flags == 0. */
#define P_SLOT_FREE      001	/* set when slot is not in use */
#define NO_MAP           002	/* keeps unmapped forked child from running */
#define SENDING          004	/* set when process blocked trying to send */
#define RECEIVING        010	/* set when process blocked trying to recv */
#define PENDING          020	/* set when inform() of signal pending */
#define SIG_PENDING      040	/* keeps to-be-signalled proc from running */
#define P_STOP		0100	/* set when process is being traced */
#ifdef MRT
#define MRT_is_realtime(X)	(X->p_flags & MRT_P_REALTIME)
#endif


/* Bits for rt.p_flags in proc[] */
#ifdef MRT
#define MRT_P_STANDARD  0x0000    

/* lower bits for RT-proc status */
#define MRT_MBX_SND     0x0001    /* set trying to send a msg to a Mailbox 		*/
#define MRT_MBX_RCV     0x0002    /* set trying to receive a msg from a Mailbox 	*/
#define MRT_P_SLEEP     0x0004    /* set waiting to be woken up 				*/
#define MRT_SEM_DOWN    0x0008    /* set trying to down a semaphore			*/
#define MRT_LOCK_WAIT   0x0010    /* set trying to lock an interrupt descriptor 	*/

#define MRT_P_STATUS    0x00FF    /* mask for status bits */

#define MRT_is_rtime(X)		(X->rt.flags & MRT_P_REALTIME)
#define MRT_is_periodic(X)	(X->rt.flags & MRT_P_PERIODIC)
#define MRT_is_rtready(X)	((X->rt.flags & MRT_P_STATUS) == 0)
#define MRT_is_process(X)	(!(X->p_flags & P_SLOT_FREE))

#define MRT_NBRVTACT	13		/* Number of Permited VT actions 				*/ 
#define MRT_ACT_NONE 	0		/* Action to exectute by a virtual timer: none   	*/
#define MRT_ACT_SCHED	1		/* Action to exectute by a virtual timer: schedule  	*/
#define MRT_ACT_MSGOWN 	2		/* Action : send a message to the vtimer owner 		*/
#define MRT_ACT_MSGWDOG	3		/* Action : send a message to the process in param field */
#define MRT_ACT_IRQTRIG 4		/* Action : IRQ trigger 					*/
#define MRT_ACT_SNDTO   5		/* Action : Send time out					*/
#define MRT_ACT_RCVFR   6		/* Action : Receive time out					*/
#define MRT_ACT_WAKEUP  7		/* Action : Wakeup a slept process 				*/
#define MRT_ACT_DEBUG   8		/* Print the param field 					*/
#define MRT_ACT_PERIODIC 9		/* Action : Sched a periodic process 			*/
#define MRT_ACT_SNDRCV  10		/* Action : Send Request/Receive reply time out		*/
#define MRT_ACT_STOP    11		/* Action : Stops a specified process 			*/
#define MRT_ACT_DOWN    12		/* Action : Stops a specified process 			*/


#define MRT_ACT_EXPIRED	MRT_NBRVTACT /* The vtimer has expired */


/* VT descriptor status flags */
#define	VT_ALLOC		0x0000/* The VT is allocated to a process or irqd 	*/
#define	VT_FREEQ		0x0001/* The VT is in the VT Free Q				*/
#define	VT_ACTIVEQ	 	0x0002/* The VT is in the VT ActiveQ 			*/
#define	VT_EXPIREDQ  	0x0004/* The VT is in the VR ExpiredQ 			*/

#endif /* MRT */

/* Magic process table addresses. */
#define BEG_PROC_ADDR (&proc[0])
#define END_PROC_ADDR (&proc[NR_TASKS + NR_PROCS])
#define END_TASK_ADDR (&proc[NR_TASKS])
#define BEG_SERV_ADDR (&proc[NR_TASKS])
#define BEG_USER_ADDR (&proc[NR_TASKS + LOW_USER])

#define NIL_PROC          ((struct proc *) 0)
#define isidlehardware(n) ((n) == IDLE || (n) == HARDWARE)
#define isokprocn(n)      ((unsigned) ((n) + NR_TASKS) < NR_PROCS + NR_TASKS)
#define isoksrc_dest(n)   (isokprocn(n) || (n) == ANY)
#define isoksusern(n)     ((unsigned) (n) < NR_PROCS)
#define isokusern(n)      ((unsigned) ((n) - LOW_USER) < NR_PROCS - LOW_USER)
#define isrxhardware(n)   ((n) == ANY || (n) == HARDWARE)

#ifdef MRT
#define issysmrt(n)       ((n) == FS_PROC_NR || (n) == MM_PROC_NR || (n) == MRTTASK )
#define MRT_ready(x)      MRT_rdyQ_app(x)
#define MRT_unready(x)    MRT_rdyQ_rmv(x)

#define vtimer_addr(i)    (&MRT_st.vtimer[(i)])
#define mqe_addr(i)       (&MRT_sm.mqe[(i)])
#define irqd_addr(i)      (&MRT_si.irqtab[(i)])
#define msgQ_addr(i)      (&MRT_sm.msgQ[(i)])
#define sem_addr(i)       (&MRT_ss.sem[(i)])

#define expQ_addr(pty)    (&MRT_st.expQ.list[(pty)])
#define irqQ_addr(pty)    (&MRT_si.irqQ.list[(pty)])
#define rdyQ_addr(pty)    (&MRT_sp.rdyQ.list[(pty)])
#define semQ_addr(i,pty)  (&MRT_ss.sem[i].semQ.list[(pty)])

#endif /* MRT */

#define issysentn(n)      ((n) == FS_PROC_NR || (n) == MM_PROC_NR)
#define istaskp(p)        ((p) < END_TASK_ADDR && (p) != proc_addr(IDLE))
#define isuserp(p)        ((p) >= BEG_USER_ADDR)
#define rtproc_addr(n)    (&proc)[(n)]
#define proc_addr(n)      (pproc_addr + NR_TASKS)[(n)]
#define cproc_addr(n)     (&(proc + NR_TASKS)[(n)])
#define proc_number(p)    ((p)->p_nr)
#define proc_vir2phys(p, vir) \
			  (((phys_bytes)(p)->p_map[D].mem_phys << CLICK_SHIFT) \
							+ (vir_bytes) (vir))
#if (SHADOWING == 1)
#define isshadowp(p)      ((p)->p_shadow != 0)
#endif

EXTERN struct proc proc[NR_TASKS + NR_PROCS];	/* process table */
EXTERN struct proc *pproc_addr[NR_TASKS + NR_PROCS];
/* ptrs to process table slots; fast because now a process entry can be found
   by indexing the pproc_addr array, while accessing an element i requires
   a multiplication with sizeof(struct proc) to determine the address */
EXTERN struct proc *bill_ptr;	/* ptr to process to bill for clock ticks */
EXTERN struct proc *rdy_head[NQ];	/* pointers to ready list headers */
EXTERN struct proc *rdy_tail[NQ];	/* pointers to ready list tails */

#endif /* PROC_H */
