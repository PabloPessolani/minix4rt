#ifndef TYPE_H
#define TYPE_H

typedef _PROTOTYPE( void task_t, (void) );
typedef _PROTOTYPE( int (*rdwt_t), (message *m_ptr) );
typedef _PROTOTYPE( void (*watchdog_t), (void) );

struct tasktab {
  task_t *initial_pc;
  int stksize;
  char name[8];
};

struct memory {
  phys_clicks base;
  phys_clicks size;
};

/* Administration for clock polling. */
struct milli_state {
  unsigned long accum_count;	/* accumulated clock ticks */
  unsigned prev_count;		/* previous clock value */
};

#if (CHIP == INTEL)
typedef unsigned port_t;
typedef unsigned segm_t;
typedef unsigned reg_t;		/* machine register */

/* The stack frame layout is determined by the software, but for efficiency
 * it is laid out so the assembly code to use it is as simple as possible.
 * 80286 protected mode and all real modes use the same frame, built with
 * 16-bit registers.  Real mode lacks an automatic stack switch, so little
 * is lost by using the 286 frame for it.  The 386 frame differs only in
 * having 32-bit registers and more segment registers.  The same names are
 * used for the larger registers to avoid differences in the code.
 */
struct stackframe_s {           /* proc_ptr points here */
#if _WORD_SIZE == 4
  u16_t gs;                     /* last item pushed by save */
  u16_t fs;                     /*  ^ */
#endif
  u16_t es;                     /*  | */
  u16_t ds;                     /*  | */
  reg_t di;			/* di through cx are not accessed in C */
  reg_t si;			/* order is to match pusha/popa */
  reg_t fp;			/* bp */
  reg_t st;			/* hole for another copy of sp */
  reg_t bx;                     /*  | */
  reg_t dx;                     /*  | */
  reg_t cx;                     /*  | */
  reg_t retreg;			/* ax and above are all pushed by save */
  reg_t retadr;			/* return address for assembly code save() */
  reg_t pc;			/*  ^  last item pushed by interrupt */
  reg_t cs;                     /*  | */
  reg_t psw;                    /*  | */
  reg_t sp;                     /*  | */
  reg_t ss;                     /* these are pushed by CPU during interrupt */
};

struct segdesc_s {		/* segment descriptor for protected mode */
  u16_t limit_low;
  u16_t base_low;
  u8_t base_middle;
  u8_t access;			/* |P|DL|1|X|E|R|A| */
#if _WORD_SIZE == 4
  u8_t granularity;		/* |G|X|0|A|LIMT| */
  u8_t base_high;
#else
  u16_t reserved;
#endif
};

/* Registers used in an PC real mode call for BIOS or DOS services.  A
 * driver is called through the vector if the interrupt number is zero.
 */
union reg86 {
  struct l {
	u32_t	ef;			/* 32 bit flags (output only) */
	u32_t	vec;			/* Driver vector (input only) */
	u32_t	_ds_es[1];
	u32_t	eax;			/* 32 bit general registers */
	u32_t	ebx;
	u32_t	ecx;
	u32_t	edx;
	u32_t	esi;
	u32_t	edi;
	u32_t	ebp;
  } l;
  struct w {
	u16_t	f, _ef[1];		/* 16 bit flags (output only) */
	u16_t	off, seg;		/* Driver vector (input only) */
	u16_t	ds, es;			/* DS and ES real mode segment regs */
	u16_t	ax, _eax[1];		/* 16 bit general registers */
	u16_t	bx, _ebx[1];
	u16_t	cx, _ecx[1];
	u16_t	dx, _edx[1];
	u16_t	si, _esi[1];
	u16_t	di, _edi[1];
	u16_t	bp, _ebp[1];
  } w;
  struct b {
	u8_t	intno, _intno[3];	/* Interrupt number (input only) */
	u8_t	_vec[4];
	u8_t	_ds_es[4];
	u8_t	al, ah, _eax[2];	/* 8 bit general registers */
	u8_t	bl, bh, _ebx[2];
	u8_t	cl, ch, _ecx[2];
	u8_t	dl, dh, _edx[2];
	u8_t	_esi[4];
	u8_t	_edi[4];
	u8_t	_ebp[4];
  } b;
};

typedef _PROTOTYPE( int (*irq_handler_t), (int irq) );

#endif /* (CHIP == INTEL) */

#if (CHIP == M68000)
typedef _PROTOTYPE( void (*dmaint_t), (void) );

typedef u32_t reg_t;		/* machine register */

/* The name and fields of this struct were chosen for PC compatibility. */
struct stackframe_s {
  reg_t retreg;			/* d0 */
  reg_t d1;
  reg_t d2;
  reg_t d3;
  reg_t d4;
  reg_t d5;
  reg_t d6;
  reg_t d7;
  reg_t a0;
  reg_t a1;
  reg_t a2;
  reg_t a3;
  reg_t a4;
  reg_t a5;
  reg_t fp;			/* also known as a6 */
  reg_t sp;			/* also known as a7 */
  reg_t pc;
  u16_t psw;
  u16_t dummy;			/* make size multiple of reg_t for system.c */
};

struct fsave {
  struct cpu_state {
	u16_t i_format;
	u32_t i_addr;
	u16_t i_state[4];
  } cpu_state;
  struct state_frame {
	u8_t frame_type;
	u8_t frame_size;
	u16_t reserved;
	u8_t frame[212];
  } state_frame;
  struct fpp_model {
	u32_t fpcr;
	u32_t fpsr;
	u32_t fpiar;
	struct fpN {
		u32_t high;
		u32_t low;
		u32_t mid;
	} fpN[8];
  } fpp_model;
};
#endif /* (CHIP == M68000) */

#ifdef	MRT

/*============================== BASIC TYPES ======================================*/

typedef struct link_s {
		struct link_s	*next;		/* pointer to the next object 	*/
		struct link_s	*prev;		/* pointer to the previous object 	*/
		char			object;		/* object type				*/
		void			*this;		/* pointer to the object itself	*/ 
	};
typedef struct link_s link_t;

typedef struct list_s {
		int 			inQ;			/* Current number of vtimer in queue*/ 
		int			maxinQ;		/* Maximun number of vtimer enqueued*/
		link_t 	 	*first;		/* pointer to the first item 		*/
		link_t		*last;		/* pointer to the last  item 		*/
	};
typedef struct list_s list_t;

typedef struct plist_s {
		bitmap16_t		bitmap;		/* bitmap of queues 			*/
								/* DONT MOVE FROM FIRST POSITION	*/

		int 			inQ;			/* Current number of vtimer in queue*/ 
		int			maxinQ;		/* Maximun number of vtimer enqueued*/
		list_t		list[MRT_NR_PRTY];	
	};
typedef struct plist_s plist_t;


/*============================== VIRTUAL TIMERS ======================================*/

struct MRT_vtimer_s {					/* PROGRAMMER FILLED FIELDS		*/
			lcounter_t	period;		/* Period in ticks			*/
			lcounter_t	limit;		/* expirations to be stopped  	*/
			action_t	action;		/* action on expiration			*/
			param_t	param;		/* action parameter			*/
			priority_t	priority;		/* owner's priority OJO: CONVERTIR	*/
								/* A PUNTERO PARA QUE REFLEJE LA    */
								/* PRIORIDAD DEL PROCESO		*/

								/* SYSTEM FILLED FIELDS- STATICS	*/
			int		index;		/* vtimer number 				*/
			int		owner;		/* number of the owner process	*/
								/* CONVERTIR A PUNTERO 			*/

								/* SYSTEM FILLED FIELDS- DYNAMIC	*/
			unsigned int flags;		/* status flags				*/
			lcounter_t	nextexp;		/* ticks to next vtimer expiration  */

								/* STATISTICAL FIELDS- DYNAMIC	*/
			lcounter_t	timestamp;		/* Last expiration  timestamp   	*/
			lcounter_t	expired;		/* number of period expirations 	*/

			link_t 	link;			/* link into the timer list		*/
	};

typedef struct MRT_vtimer_s MRT_vtimer_t;

typedef struct	{
		long		earliest;  		/* ticks to earliest expiration  		*/
		lcounter_t	maxper;   		/* the highest period 				*/	
		list_t	list;			/* Active Queue VT list				*/
	} MRT_actQ_t;


typedef	struct	{
		plist_t  		expQ;			/* queues of expired virtual timers */
								/* DONT MOVE FROM FIRST POSITION	*/
		MRT_actQ_t  	actQ;			/* queue of active virtual timers 	*/
		list_t	  	freeQ;		/* queue of free virtual timers 	*/
		MRT_vtimer_t vtimer[NR_VTIMERS]; 	/* pool of virtual timers 		*/

		lcounter_t		T2curr;   		/* Timer2 current value			*/
		lcounter_t		T2prev;   		/* Timer2 previous value		*/
		unsigned int	latch;   		/* Current PIT latch			*/
		} MRT_systmr_t;


/*============================== INTERRUPTS ======================================*/

struct MRT_irqd_s {

		/*-------------------- STATISTICAL FIELDS   ----------------------------*/ 
		lcounter_t		count;	/* Interrupt counter				*/
		scounter_t		maxshower;	/* Maximun shower value				*/
  		lcounter_t		mdl;		/* Missed DeadLines				*/
		lcounter_t		timestamp;	/* Last Interrupt timestamp   		*/
		lcounter_t		maxlat;	/* Maximun Interrupt latency in timer hz 	*/
		int			reenter;	/* maximun reentrancy level			*/

		/*-------------------- INTERNAL USE  FIELDS   ----------------------------*/
		int			irq;		/* IRQ number					*/
		scounter_t		harmonic;	/* Harmonic when the period was set 	*/
		MRT_vtimer_t	*pvt;		/* VT assigned for Timer Driven IRQs 	*/
		unsigned int	flags;	/* MRT_ENQUEUED, MRT_TRIGGERED, ETC		*/

		lcounter_t		latency;	/* Interrupt latency in timer hz 		*/
		scounter_t		shower;	/* TD Interrupt counter since last period	*/
		scounter_t		before;	/* TIC counter in MRT_IRQ_dispatch 		*/

		link_t		link;		/* next/prev links to other irqd		*/

		/*-------------------- FUNCTIONAL FIELDS    ----------------------------*/ 
		irq_handler_t	nrthandler;	/* Non real time handler			*/
		irq_handler_t	rthandler;	/* real time handler				*/
		lcounter_t		period;	/* For Timer Driven period in ticks 	*/
		proc_nbr_t		task;		/* Real Time task number 			*/
		proc_nbr_t		watchdog;	/* Interrupt watchdog process			*/
		priority_t		priority;	/* RT  priority					*/
		irq_type_t		irqtype;	/* MRT_RTIRQ | MRT_TDIRQ | MRT_SOFTIRQ 	*/
		char			name[MAXPNAME]; 	/* name of the RT driver	*/
};
typedef struct MRT_irqd_s MRT_irqd_t;

typedef	struct	{
		plist_t		irqQ;			/* interrupt descriptors queue		*/
								/* DONT MOVE FROM FIRST POSITION	*/
		list_t		mnxQ;			/* Pending MINIX interrupt descriptors	*/

		bitmap16_t		hard_use;		/* bit map of all in-use hard interrupts 	*/
		bitmap16_t		soft_use;		/* bit map of all in-use soft interrupts 	*/

		MRT_irqd_t 		irqtab[NR_IRQ_VECTORS+NR_IRQ_SOFT]; /* IRQ desc table 	*/
		irq_mask_t		mask[MRT_NR_PRTY];/* PIC masks for each priority		*/
		} MRT_sysirq_t;

/*============================== READY PROCESSES =====================================*/

typedef	struct proc MRT_proc_t;

typedef    struct		{
		plist_t	 	rdyQ;
		}MRT_sysproc_t;

/*============================== MESSAGES ======================================*/

/* Real-Time Message Queue Entry */
struct MRT_mqe_s {
		mrt_msgd_t			msgd;
		int				index;
		MRT_vtimer_t		*pvt;		/* VT for timeouts			*/
		link_t			link;
	};
typedef struct MRT_mqe_s MRT_mqe_t;

/* Real-Time Message Queue */
struct MRT_msgQ_s	{
		int			index;	/* message queue ID (for quick search )   */
		int			size;		/* message queue size				*/
		unsigned int	flags;	/* message queue policy flags			*/
		int			owner;	/* msg queue owner				*/
		long			delivered;	/* total #  of msgs delivered 		*/
		long			enqueued;	/* total # of msgs enqueued			*/
		link_t		link;
		plist_t 		plist;	/* Array of priority message lists		*/
};
typedef struct MRT_msgQ_s MRT_msgQ_t;

typedef	struct	{
		list_t 	 	mqeL;			/* free message queue entries list  */
		MRT_mqe_t		mqe[NR_MESSAGES]; /* message queue entries		*/

		list_t 	 	msgQL;		/* free msgQ  list			*/
		MRT_msgQ_t 		msgQ[NR_MSGQ]; 	/* message queues				*/
 		} MRT_sysmsg_t;

/*============================== SEMAPHORES ======================================*/

/* Real-Time Semaphore */
struct MRT_sem_s {
		int			index;	/* semaphore ID					*/
		int			value;	/* semaphore Value				*/
		priority_t		priority;	/* Ceiling priority - for future uses 	*/
		unsigned int	flags;	/* semaphore policy/status flags		*/
		int			owner;	/* semaphore owner				*/
		long			ups;		/* total #  of sem up() calls 		*/
		long			downs;	/* total #  of sem down() calls 		*/
		MRT_proc_t		*carrier;	/* the process that currently have the mutex sem */

		link_t		alloclk;	/* Allocated list link				*/
		link_t		locklk;	/* Locked list link				*/

		char			name[MAXPNAME]; 	/* name of the semaphore		*/
		plist_t	 	plist;	/* Priority List of waiting process		*/
	};
typedef struct MRT_sem_s MRT_sem_t;


typedef	struct	{
		list_t 	 	sfreeL;	/* Semaphore free  list				*/
		MRT_sem_t 		sem[NR_SEM];/* Semaphore pool					*/
 		} MRT_syssem_t;

/*============================== SYSTEM WIDE  ======================================*/

struct MRT_sysstat_s {
		lcounter_t		scheds;	/* schedules counter			*/
		lcounter_t		messages;	/* message counter - ID count		*/
   volatile lcounter_t		interrupts;	/* Interrupt counter			*/
   volatile lcounter_t		ticks;	/* Less Significative tick counter	*/
   volatile lcounter_t		highticks;	/* More Significative tick counter 	*/
   		lcounter_t		idlemax;	/* idle maximum value			*/
		lcounter_t		idlelast;	/* last counter				*/
		lcounter_t		oneshots;	/* Number of one shots			*/
		};
	
typedef struct MRT_sysstat_s MRT_sysstat_t;

typedef	struct	{
		/* DON'T MOVE flags FROM THE FIRST POSITION IN THE STRUCT !! */	
		unsigned int	flags; 
		/* MRT_RTMODE	0x0001 RT processing mode					*/
		/* MRT_MINIXCLI	0x0002 MINIX virtual IF 1=CLI            			*/
		/* MRT_NEWINT	0x0004 is set if an IRQ has occurred during int flushing*/
		/* MRT_FLUSHLCK   0x0008 This flag is set when MRT_flush_int is running */
		/* MRT_NOFLUSH    0x0010 This flag is to avoid calling MRT_irq_flush 	*/
		/* MRT_NEEDSCHED  0x0020 Set by a IRQ handler to invoke the scheduler 	*/
		/* MRT_RTSCHED    0x0040 The current proc has been selected by  		*/
		/* 				 the RT scheduler MRT_pick_proc 			*/
		/* MRT_DBG232	0x0100 Flag to allow printf232				*/
		/* MRT_LATENCY	0x0200 Flag to allow latency computation 			*/
		/* MRT_BPIP       0x0400 Basic Priority Inheritance Protocol enabled	*/
		/* MRT_ONESHOT    0x0800 One Shot timer						*/
		/* MRT_PRTYMASK   0x1000 Change PIC Mask to the process priority enabled*/

		bitmap16_t		virtual_PIC; /* Virtual PIC for MINIX  				*/
		bitmap16_t		real_PIC; 	 /* Virtual PIC for MINIX  				*/
		lcounter_t		PIT_latency; /* PIT latency in Hz between two reads 		*/
		unsigned		PIT_latch;	 /* TIMER_FREQ/MRT_tickrate				*/
		scounter_t		tickrate;	 /* Real-Time ticks by second				*/

		scounter_t		harmonic;	 /* tickrate = MRT_sv.harmonic * HZ			*/
		scounter_t		refresh; 	 /* ticks to refresh idlerfsh				*/
		unsigned int 	tick2Hz;	 /* TIMER_FREQ/HZ/MRT_sv.harmonic			*/

		int			MINIX_soft;	 /* MINIX CLOCK Software interrupt   		*/ 
		int			IDLE_soft;	 /* IDLE Refresh Software interrupt   		*/ 
		priority_t		prtylvl;	 /* Current priority level 				*/
		lcounter_t		idlecount;	 /* idle loop counter					*/
		MRT_sysstat_t	counter;	 /* system statistics 					*/

		} MRT_sysval_t;


#endif	/* MRT */

#endif /* TYPE_H */
