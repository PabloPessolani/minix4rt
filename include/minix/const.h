/* Copyright (C) 1998 by Prentice-Hall, Inc.  Permission is hereby granted
 * to redistribute the binary and source programs of this system for
 * educational or research purposes.  For other use, written permission from
 * Prentice-Hall is required.  
 */

#ifndef CHIP
#error CHIP is not defined
#endif

#define EXTERN        extern	/* used in *.h files */
#define PRIVATE       static	/* PRIVATE x limits the scope of x */
#define PUBLIC			/* PUBLIC is the opposite of PRIVATE */
#define FORWARD       static	/* some compilers require this to be 'static'*/

#define TRUE               1	/* used for turning integers into Booleans */
#define FALSE              0	/* used for turning integers into Booleans */

#ifdef MRT
#define HZ	          50	/* clock freq (software settable on IBM-PC) 	*/
#define MRT_TIMERCOEF  1	/* initial value for MRT_timercoef			*/ 
#else
#define HZ	          60	/* clock freq (software settable on IBM-PC) */
#endif 

#define BLOCK_SIZE      1024	/* # bytes in a disk block */
#define SUPER_USER (uid_t) 0	/* uid_t of superuser */

#define MAJOR	           8	/* major device = (dev>>MAJOR) & 0377 */
#define MINOR	           0	/* minor device = (dev>>MINOR) & 0377 */

#define NULL     ((void *)0)	/* null pointer */
#define CPVEC_NR          16	/* max # of entries in a SYS_VCOPY request */
#define NR_IOREQS	MIN(NR_BUFS, 64)
				/* maximum number of entries in an iorequest */

#define NR_SEGS            3	/* # segments per process */
#define T                  0	/* proc[i].mem_map[T] is for text */
#define D                  1	/* proc[i].mem_map[D] is for data */
#define S                  2	/* proc[i].mem_map[S] is for stack */

/* Process numbers of some important processes. */
#define MM_PROC_NR         0	/* process number of memory manager */
#define FS_PROC_NR         1	/* process number of file system */
#define INET_PROC_NR       2	/* process number of the TCP/IP server */
#define INIT_PROC_NR	(INET_PROC_NR + ENABLE_NETWORKING)
				/* init -- the process that goes multiuser */
#define LOW_USER	(INET_PROC_NR + ENABLE_NETWORKING)
				/* first user not part of operating system */

/* Miscellaneous */
#define BYTE            0377	/* mask for 8 bits */
#define READING            0	/* copy data to user */
#define WRITING            1	/* copy data from user */
#define NO_NUM        0x8000	/* used as numerical argument to panic() */
#define NIL_PTR   (char *) 0	/* generally useful expression */
#define HAVE_SCATTERED_IO  1	/* scattered I/O is now standard */

/* Macros. */
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define MIN(a, b)   ((a) < (b) ? (a) : (b))



/* Number of tasks. */
#ifdef MRT

#define SET_BIT(a, b)   (a |= b)
#define CLR_BIT(a, b)   (a &= ~b)
#define TEST_BIT(a, b)  (a & b)

#define NR_TASKS	(9 + ENABLE_WINI + ENABLE_SCSI + ENABLE_CDROM \
			+ ENABLE_MINIX4RT \
			+ ENABLE_DOSDSK + ENABLE_NETWORKING + 2 * ENABLE_AUDIO)
#else
#define NR_TASKS	(9 + ENABLE_MINIX4RT + ENABLE_WINI + ENABLE_SCSI + ENABLE_CDROM \
			+ ENABLE_DOSDSK + ENABLE_NETWORKING + 2 * ENABLE_AUDIO)
#endif

/* Memory is allocated in clicks. */
#if (CHIP == INTEL)
#define CLICK_SIZE       256	/* unit in which memory is allocated */
#define CLICK_SHIFT        8	/* log2 of CLICK_SIZE */
#endif

#if (CHIP == SPARC) || (CHIP == M68000)
#define CLICK_SIZE	4096	/* unit in which memory is alocated */
#define CLICK_SHIFT	  12	/* 2log of CLICK_SIZE */
#endif

#define click_to_round_k(n) \
	((unsigned) ((((unsigned long) (n) << CLICK_SHIFT) + 512) / 1024))
#if CLICK_SIZE < 1024
#define k_to_click(n) ((n) * (1024 / CLICK_SIZE))
#else
#define k_to_click(n) ((n) / (CLICK_SIZE / 1024))
#endif

#define ABS             -999	/* this process means absolute memory */

/* Flag bits for i_mode in the inode. */
#define I_TYPE          0170000	/* this field gives inode type */
#define I_REGULAR       0100000	/* regular file, not dir or special */
#define I_BLOCK_SPECIAL 0060000	/* block special file */
#define I_DIRECTORY     0040000	/* file is a directory */
#define I_CHAR_SPECIAL  0020000	/* character special file */
#define I_NAMED_PIPE	0010000 /* named pipe (FIFO) */
#define I_SET_UID_BIT   0004000	/* set effective uid_t on exec */
#define I_SET_GID_BIT   0002000	/* set effective gid_t on exec */
#define ALL_MODES       0006777	/* all bits for user, group and others */
#define RWX_MODES       0000777	/* mode bits for RWX only */
#define R_BIT           0000004	/* Rwx protection bit */
#define W_BIT           0000002	/* rWx protection bit */
#define X_BIT           0000001	/* rwX protection bit */
#define I_NOT_ALLOC     0000000	/* this inode is free */

/* Some limits. */
#define MAX_BLOCK_NR  ((block_t) 077777777)	/* largest block number */
#define HIGHEST_ZONE   ((zone_t) 077777777)	/* largest zone number */
#define MAX_INODE_NR      ((ino_t) 0177777)	/* largest inode number */
#define MAX_FILE_POS ((off_t) 037777777777)	/* largest legal file offset */

#define NO_BLOCK              ((block_t) 0)	/* absence of a block number */
#define NO_ENTRY                ((ino_t) 0)	/* absence of a dir entry */
#define NO_ZONE                ((zone_t) 0)	/* absence of a zone number */
#define NO_DEV                  ((dev_t) 0)	/* absence of a device numb */

#ifdef	MRT

#ifndef	_CONST_H
#define     _CONST_H
#endif

#define NR_IRQ_SOFT    		16
#define NR_IRQ_VECTORS    	16


/************* MRT_sv.flags ************************/
/*--------------------- System processing flags ------------------------------------------*/
#define 	MRT_RTMODE		0x0001  /* RT processing mode						*/
#define	MRT_MINIXCLI	0x0002  /* MINIX virtual IF 1=CLI            			*/
#define	MRT_NEWINT 		0x0004  /* is set if an IRQ has occurred during int flushing*/
#define     MRT_FLUSHLCK      0x0008  /* This flag is set when MRT_flush_int is running 	*/
#define     MRT_NOFLUSH       0x0010  /* This flag is to avoid calling MRT_irq_flush 	*/
#define     MRT_NEEDSCHED     0x0020  /* Set  by a IRQ handler to invoke the scheduler 	*/
#define     MRT_RTSCHED       0x0040  /* The current proc has been selected by  		*/
						  /* the RT scheduler MRT_pick_proc 			*/

/*--------------------- System processing options ----------------------------------------*/
#define	MRT_DBG232		0x0100  /* Flag to allow printf232 					*/
#define	MRT_LATENCY		0x0200  /* Flag to allow latency computation 			*/
#define     MRT_BPIP          0x0400  /* Basic Priority Inheritance Protocol enabled	*/
#define     MRT_ONESHOT       0x0800  /* One Shot timer						*/
#define     MRT_PRTYMASK      0x1000  /* Change PIC Mask to the process priority enabled  */


#define	MAXMQSIZE		10	/* Maximun Message Queue size 	*/

/*  FLAGS for Message Queue flags  */
#define 	MQ_PRTYORDER	0x0001  /* Priority Order Policy		*/
#define	MQ_PRTYINHERIT 	0x0002  /* Priority Inheritance policy	*/

/*  FLAGS for Semaphore flags  */
#define 	SEM_PRTYORDER	0x0001  /* Priority Order Policy		*/
#define	SEM_PRTYINHERIT 	0x0002  /* Priority Inheritance policy	*/
#define	SEM_MUTEX	 	0x0004  /* MUTEX Semaphore			*/

#define	MAXPNAME		16	/* Maximun chars for object names 	*/

/* Interrupt descriptor types */
#define	MRT_NRTIRQ		0x00	/* Non Real Time IRQ handler		*/
#define	MRT_RTIRQ		0x01	/* Real Time IRQ handler		*/
#define	MRT_TDIRQ		0x02	/* Timer Driven IRQ handler		*/
#define	MRT_SOFTIRQ		0x04	/* Software Interrupt 			*/
#define	MRT_ISTMSG		0x08	/* Send a message to an IST 		*/
#define	MRT_RELATPER	0x10	/* The period is related to the current PIT Hz */

/* Process Descriptor types */
#define MRT_P_REALTIME  0x4000    /* set when process is Real-Time 		*/
#define MRT_P_PERIODIC  0x8000    /* set when process is Real-Time Periodic 	*/

/* Message descriptor types */
#define MT_MBFREE		0x00		/* for free mailbox entry			*/
#define MT_UPRQST		0x01		/* up request message type			*/
#define MT_REQUEST	0x02		/* request message type				*/
#define MT_AREQUEST	0x03		/* request message type asynchronously 	*/
#define MT_REPLY		0x04		/* reply message type				*/
#define MT_SIGNAL		0x05		/* SIGNAL message type				*/
#define MT_RQRCV		0x06		/* Request/Receive  message type		*/

#define MT_KRNIRQ		0x10		/* Kernel inform an Interrupt			*/
#define MT_KRNTOUT	0x11		/* Kernel inform a Timeout 			*/
#define MT_KRNDLINE	0x12		/* Kernel inform a deadline expiration 	*/
#define MT_KRNWDOG	0x13		/* Kernel inform a watchdog proc has dead */
#define MT_KRNPROC	0x14		/* Kernel inform a process dead 		*/


/* Interrupt descriptor status flags */
#define	MRT_ENQUEUED	0x0001	/* The int descriptor is enqueued in the int pending Q*/
#define	MRT_DISABLED 	0x0002	/* The handler is disabled 					*/
#define	MRT_TRIGGERED  	0x0004	/* The interrupt has been triggered 			*/
#define	MRT_LATCALC  	0x0008	/* Set this bit for latency computation for the irqd 	*/
#define	MRT_TDTRIGGER 	0x0010	/* A TD IRQ has occurred, signal timer to trigger it 	*/
#define	MRT_DELAYED 	0x0020	/* An IRQ is temporaly delayed 			 	*/
#define	MRT_TASKLOCKED 	0x0040	/* The IRQ locked by a Task	 			 	*/
#define	MRT_HANDLOCKED 	0x0080	/* The IRQ is locked by a IRQ Handler		 	*/
#define	MRT_TASKWAIT	0x0100	/* The Task is waiting the lock 			 	*/
#define	MRT_HANDWAIT 	0x0200	/* The Handler is waiting the lock			 	*/

/* Real-Time Priorities		*/
#define	MRT_PRI00		0x00		/* HIGHEST	*/
#define	MRT_PRI01		0x01
#define	MRT_PRI02		0x02
#define	MRT_PRI03		0x03
#define	MRT_PRI04		0x04
#define	MRT_PRI05		0x05
#define	MRT_PRI06		0x06
#define	MRT_PRI07		0x07
#define	MRT_PRI08		0x08
#define	MRT_PRI09		0x09
#define	MRT_PRI10		0x0A
#define	MRT_PRI11		0x0B
#define	MRT_PRI12		0x0C
#define	MRT_PRI13		0x0D
#define	MRT_PRI14		0x0E
#define	MRT_PRI15		0x0F		/* LOWEST	*/

#define	MRT_PRIHIGHEST	MRT_PRI00
#define     MRT_PRILOWEST	MRT_PRI15
#define	MRT_NR_PRTY		16

/* Object types	*/
#define	MRT_OBJVT		1		/* Virtual Timer		*/
#define	MRT_OBJPROC		2		/* Process			*/
#define	MRT_OBJMQ		3		/* Message Queue 		*/
#define	MRT_OBJMQE 		4		/* Message Queue Entry	*/
#define	MRT_OBJIRQD		5		/* Interrupt Descriptor */
#define	MRT_OBJSEM		6		/* RT Semaphore		*/

#define MAXHARMONIC	10000			/* Maximun Timer Harmonic */
#define MAXREFRESH	10000			/* Maximun Idle Refresh count */
#define NOVTIMER		-1

#define MRT_MINPER  	    1		
#define MRT_MAXPER  	10000			

#define MRTNOMSGQ		-1
#define MRTMSGQFREE	-1
#define MRTMQEFREE	-1

/* Virtual timer Creation Error codes */
#define ENOVTIMER		-1
#define EBADPERIOD	-2
#define EBADLIMIT 	-3
#define EBADACTION 	-4
#define EBADOWNER		-5

#define DBG232_BUFF 	1024 /* debug output buffer size */
#define DBG232_ON 	1
#define DBG232_OFF 	0

#define 	MRT_IRQ_DISABLED		0
#define 	MRT_IRQ_ENABLED		1

#define	MRT_NOWAIT			0
#define	MRT_FOREVER			-1

#endif	/* MRT */
