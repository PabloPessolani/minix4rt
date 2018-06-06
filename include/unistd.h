/* The <unistd.h> header contains a few miscellaneous manifest constants. */

#ifndef _UNISTD_H
#define _UNISTD_H

/* POSIX requires size_t and ssize_t in <unistd.h> and elsewhere. */
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef int ssize_t;
#endif

/* Values used by access().  POSIX Table 2-8. */
#define F_OK               0	/* test if file exists */
#define X_OK               1	/* test if file is executable */
#define W_OK               2	/* test if file is writable */
#define R_OK               4	/* test if file is readable */

/* Values used for whence in lseek(fd, offset, whence).  POSIX Table 2-9. */
#define SEEK_SET           0	/* offset is absolute  */
#define SEEK_CUR           1	/* offset is relative to current position */
#define SEEK_END           2	/* offset is relative to end of file */

/* This value is required by POSIX Table 2-10. */
#define _POSIX_VERSION 199009L	/* which standard is being conformed to */

/* These three definitions are required by POSIX Sec. 8.2.1.2. */
#define STDIN_FILENO       0	/* file descriptor for stdin */
#define STDOUT_FILENO      1	/* file descriptor for stdout */
#define STDERR_FILENO      2	/* file descriptor for stderr */

#ifdef _MINIX
/* How to exit the system. */
#define RBT_HALT	   0
#define RBT_REBOOT	   1
#define RBT_PANIC	   2	/* for servers */
#define RBT_MONITOR	   3	/* let the monitor do this */
#define RBT_RESET	   4	/* hard reset the system */
#endif

/* NULL must be defined in <unistd.h> according to POSIX Sec. 2.7.1. */
#define NULL    ((void *)0)

/* The following relate to configurable system variables. POSIX Table 4-2. */
#define _SC_ARG_MAX		1
#define _SC_CHILD_MAX		2
#define _SC_CLOCKS_PER_SEC	3
#define _SC_CLK_TCK             3
#define _SC_NGROUPS_MAX		4
#define _SC_OPEN_MAX		5
#define _SC_JOB_CONTROL		6
#define _SC_SAVED_IDS		7
#define _SC_VERSION		8
#define _SC_STREAM_MAX		9
#define _SC_TZNAME_MAX         10

/* The following relate to configurable pathname variables. POSIX Table 5-2. */
#define _PC_LINK_MAX		1	/* link count */
#define _PC_MAX_CANON		2	/* size of the canonical input queue */
#define _PC_MAX_INPUT		3	/* type-ahead buffer size */
#define _PC_NAME_MAX		4	/* file name size */
#define _PC_PATH_MAX		5	/* pathname size */
#define _PC_PIPE_BUF		6	/* pipe size */
#define _PC_NO_TRUNC		7	/* treatment of long name components */
#define _PC_VDISABLE		8	/* tty disable */
#define _PC_CHOWN_RESTRICTED	9	/* chown restricted or not */

/* POSIX defines several options that may be implemented or not, at the
 * implementer's whim.  This implementer has made the following choices:
 *
 * _POSIX_JOB_CONTROL	    not defined:	no job control
 * _POSIX_SAVED_IDS 	    not defined:	no saved uid/gid
 * _POSIX_NO_TRUNC	    defined as -1:	long path names are truncated
 * _POSIX_CHOWN_RESTRICTED  defined:		you can't give away files
 * _POSIX_VDISABLE	    defined:		tty functions can be disabled
 */
#define _POSIX_NO_TRUNC       (-1)
#define _POSIX_CHOWN_RESTRICTED  1

/* Function Prototypes. */
#ifndef _ANSI_H
#include <ansi.h>
#endif

_PROTOTYPE( void _exit, (int _status)					);
_PROTOTYPE( int access, (const char *_path, int _amode)			);
_PROTOTYPE( unsigned int alarm, (unsigned int _seconds)			);
_PROTOTYPE( int chdir, (const char *_path)				);
_PROTOTYPE( int chown, (const char *_path, Uid_t _owner, Gid_t _group)	);
_PROTOTYPE( int close, (int _fd)					);
_PROTOTYPE( char *ctermid, (char *_s)					);
_PROTOTYPE( char *cuserid, (char *_s)					);
_PROTOTYPE( int dup, (int _fd)						);
_PROTOTYPE( int dup2, (int _fd, int _fd2)				);
_PROTOTYPE( int execl, (const char *_path, const char *_arg, ...)	);
_PROTOTYPE( int execle, (const char *_path, const char *_arg, ...)	);
_PROTOTYPE( int execlp, (const char *_file, const char *arg, ...)	);
_PROTOTYPE( int execv, (const char *_path, char *const _argv[])		);
_PROTOTYPE( int execve, (const char *_path, char *const _argv[], 
						char *const _envp[])	);
_PROTOTYPE( int execvp, (const char *_file, char *const _argv[])	);
_PROTOTYPE( pid_t fork, (void)						);
_PROTOTYPE( long fpathconf, (int _fd, int _name)			);
_PROTOTYPE( char *getcwd, (char *_buf, size_t _size)			);
_PROTOTYPE( gid_t getegid, (void)					);
_PROTOTYPE( uid_t geteuid, (void)					);
_PROTOTYPE( gid_t getgid, (void)					);
_PROTOTYPE( int getgroups, (int _gidsetsize, gid_t _grouplist[])	);
_PROTOTYPE( char *getlogin, (void)					);
_PROTOTYPE( pid_t getpgrp, (void)					);
_PROTOTYPE( pid_t getpid, (void)					);
_PROTOTYPE( pid_t getppid, (void)					);
_PROTOTYPE( uid_t getuid, (void)					);
_PROTOTYPE( int isatty, (int _fd)					);
_PROTOTYPE( int link, (const char *_existing, const char *_new)		);
_PROTOTYPE( off_t lseek, (int _fd, off_t _offset, int _whence)		);
_PROTOTYPE( long pathconf, (const char *_path, int _name)		);
_PROTOTYPE( int pause, (void)						);
_PROTOTYPE( int pipe, (int _fildes[2])					);
_PROTOTYPE( ssize_t read, (int _fd, void *_buf, size_t _n)		);
_PROTOTYPE( int rmdir, (const char *_path)				);
_PROTOTYPE( int setgid, (Gid_t _gid)					);
_PROTOTYPE( int setpgid, (pid_t _pid, pid_t _pgid)			);
_PROTOTYPE( pid_t setsid, (void)					);
_PROTOTYPE( int setuid, (Uid_t _uid)					);
_PROTOTYPE( unsigned int sleep, (unsigned int _seconds)			);
_PROTOTYPE( long sysconf, (int _name)					);
_PROTOTYPE( pid_t tcgetpgrp, (int _fd)					);
_PROTOTYPE( int tcsetpgrp, (int _fd, pid_t _pgrp_id)			);
_PROTOTYPE( char *ttyname, (int _fd)					);
_PROTOTYPE( int unlink, (const char *_path)				);
_PROTOTYPE( ssize_t write, (int _fd, const void *_buf, size_t _n)	);

#ifdef _MINIX
_PROTOTYPE( int brk, (char *_addr)					);
_PROTOTYPE( int chroot, (const char *_name)				);
_PROTOTYPE( int mknod, (const char *_name, Mode_t _mode, Dev_t _addr)	);
_PROTOTYPE( int mknod4, (const char *_name, Mode_t _mode, Dev_t _addr,
	    long _size)							);
_PROTOTYPE( char *mktemp, (char *_template)				);
_PROTOTYPE( int mount, (char *_spec, char *_name, int _flag)		);
_PROTOTYPE( long ptrace, (int _req, pid_t _pid, long _addr, long _data)	);
_PROTOTYPE( char *sbrk, (int _incr)					);
_PROTOTYPE( int sync, (void)						);
_PROTOTYPE( int umount, (const char *_name)				);
_PROTOTYPE( int reboot, (int _how, ...)					);
_PROTOTYPE( int gethostname, (char *_hostname, size_t _len)		);
_PROTOTYPE( int getdomainname, (char *_domain, size_t _len)		);
_PROTOTYPE( int ttyslot, (void)						);
_PROTOTYPE( int fttyslot, (int _fd)					);
_PROTOTYPE( char *crypt, (const char *_key, const char *_salt)		);
#endif



#ifndef _CONFIG_H
#include <minix/config.h>
#endif

#ifdef MRT 
#ifndef	_CONST_H
#include    <minix/const.h>
#endif

#ifndef	_TYPES_H
#include    <sys/types.h>
#endif

#ifndef	_TYPE_H
#include    <minix/type.h>
#endif

/*============================================================================*/
/*					SYSTEM STRUCTS						*/
/*============================================================================*/

/*--------------------------- SYSTEM WIDE STATISTICS -------------------------*/
struct mrt_sysstat_s {
		lcounter_t		scheds;	/* schedules counter			*/
		lcounter_t		messages;	/* message counter - ID count		*/
   		lcounter_t		interrupts;	/* Interrupt counter			*/
   		lcounter_t		ticks;	/* Less Significative tick counter	*/
   		lcounter_t		highticks;	/* More Significative tick counter 	*/
   		lcounter_t		idlemax;	/* Maximun idle counter 	 	*/
   		lcounter_t		idlelast;	/* last idle counter 		 	*/
		};	
typedef struct mrt_sysstat_s mrt_sysstat_t;

/*--------------------------- SYSTEM OPERATIONAL VALUES ----------------------*/
struct mrt_sysval_s	{
		unsigned int	flags; 
		bitmap16_t		virtual_PIC; /* Virtual PIC for MINIX  		*/
		lcounter_t		PIT_latency; /* PIT latency in Hz between two reads*/
		unsigned		PIT_latch;	 /* TIMER_FREQ/MRT_tickrate		*/
		scounter_t		tickrate;	 /* Real-Time ticks by second		*/
		scounter_t		harmonic;	 /* MRT_tickrate = MRT_harmonic x HZ*/
		scounter_t		refresh;	 /* Idle refresh tick count 		*/
		};
typedef struct mrt_sysval_s mrt_sysval_t;

/*============================================================================*/
/*					IRQ STRUCTS							*/
/*============================================================================*/
/*--------------------------- IRQ SPECIFIED ARGUMENTS  ----------------------*/
struct mrt_irqattr_s { 
		lcounter_t		period;	/* For Timer Driven period in ticks */
		proc_nbr_t		task;		/* Real Time task number 		*/
		proc_nbr_t		watchdog;	/* Interrupt Watch dog process	*/
		priority_t		priority;	/* RT or NRT priority			*/
		irq_type_t		irqtype;	/* IRQ Type					*/
		char			name[MAXPNAME]; 	/* name of the RT driver	*/
 }; 
typedef struct mrt_irqattr_s mrt_irqattr_t;

/*--------------------------- IRQ STATISTICS ----------------------*/
struct mrt_irqstat_s { 
		lcounter_t		count;	/* Interrupt counter			*/
		scounter_t		maxshower;	/* Maximum shower value			*/
		lcounter_t		mdl;		/* Missed Deadlines 			*/
		lcounter_t		timestamp;	/* Last Interrupt timestamp   	*/
		lcounter_t		maxlat;	/* Maximun Interrupt latency PIT Hz */
		int			reenter;	/* Maximun reentrancy level   	*/
 }; 
typedef struct mrt_irqstat_s mrt_irqstat_t;

/*--------------------------- IRQ INTERNAL ----------------------*/
struct mrt_irqint_s  { 
		int			irq;		/* irq number				*/
		scounter_t		harmonic;	/* MRT_sv.harmonic when mrtode starts*/
		int			vtimer;	/* VT assigned for Timer Driven IRQs*/
		int			flags;	/* MRT_ENQUEUED, MRT_TRIGGEREG	*/

 }; 
typedef struct mrt_irqint_s mrt_irqint_t;

/*============================================================================*/
/*					PROCESS STRUCTS						*/
/*============================================================================*/
struct mrt_pattr_s {
		int 		flags;	/* Real Time Flags			 	*/
 		priority_t	baseprty;	/* Real Time BASE priority			*/
 		lcounter_t	period;	/* period in RT-ticks				*/
		scounter_t	limit;	/* maximun number of process schedulings  */
  		lcounter_t	deadline;  	/* process deadline				*/
		int	 	watchdog;   /* Watchdog process				*/

		scounter_t	mq_size;	/* Message Queue Size				*/
   unsigned int		mq_flags;	/* Message Queue Policy Flags			*/
		};
typedef struct mrt_pattr_s  mrt_pattr_t;

struct mrt_pstat_s {
  		lcounter_t	scheds;	/* number of schedules	 			*/
  		lcounter_t	mdl;		/* Missed DeadLines	 			*/
  		lcounter_t	timestamp;	/* Last schedule timestamp in ticks 	*/
  		lcounter_t	maxlat;	/* Maximun latency in timer Hz 		*/
  		scounter_t	minlax;	/* Minimun laxity in timer Hz 		*/
  		lcounter_t	msgsent;	/* Messages sent by the process 		*/
  		lcounter_t	msgrcvd;	/* Messages received by the process		*/
		};
typedef struct mrt_pstat_s mrt_pstat_t;

struct mrt_pint_s {
		int		vt;		/* virtual timer ID for periodic process  */
		priority_t	priority;	/* Real Time EFECTIVE priority		*/
		scounter_t	mqID;		/* Message Queue ID				*/
		int		p_nr;		/* process slot 					*/
		};
typedef struct mrt_pint_s mrt_pint_t;

/*============================================================================*/
/*				RT-SEMAPHORE  DATA  STRUCTS					*/
/*============================================================================*/
struct mrt_semattr_s {
		int			value;	/* semaphore Value				*/
		unsigned int	flags;	/* semaphore policy/status flags		*/
		priority_t		priority;	/* Ceiling priority - for future uses 	*/
		char			name[MAXPNAME]; 	/* name of the semaphore		*/
		};
typedef struct mrt_semattr_s mrt_semattr_t;

struct mrt_semstat_s {
		long			ups;		/* total #  of sem up() calls 		*/
		long			downs;	/* total #  of sem down() calls 		*/
		int			maxinQ;	/* maximun # of process enqueued		*/
		};
typedef struct mrt_semstat_s mrt_semstat_t;

struct mrt_semint_s {
		int			index;	/* semaphore ID					*/
		int			owner;	/* semaphore owner				*/
		int			inQ;		/* # of process enqueued			*/
		};
typedef struct mrt_semint_s mrt_semint_t;

struct mrt_down_s {
		int			index;	/* semaphore ID					*/
  		lcounter_t		timeout;	/* timeout in ticks	 			*/
		};
typedef struct mrt_down_s mrt_down_t;

/*============================================================================*/
/*				MESSAGE QUEUE  DATA  STRUCTS					*/
/*============================================================================*/

/* Real-Time Message Queue */

struct mrt_msgqstat_s {
		long			delivered;	/* total #  of msgs delivered 		*/
		long			enqueued;	/* total # of msgs enqueued			*/
		int			maxinQ;	/* maximun # of process enqueued		*/
		};
typedef struct mrt_msgqstat_s mrt_msgqstat_t;


/*============================================================================*/
/*				RT-IPC  DATA  STRUCTS						*/
/*============================================================================*/
struct mrt_ipc_s {
  		int		p_nr;		/* request destination number			*/
  		pid_t		pid;		/* request destination PID			*/
  		mrt_msg_t	*msg;		/* Message to be sent				*/
  		mrt_msg_t	*msg2;	/* Message to be sent use for replies	*/
		mrt_mhdr_t  *hdr;		/* pointer to the buffer for the msg header */
  		lcounter_t	timeout;	/* rqst timeout in ticks 			*/
		int		priority;	/* message RT priority				*/
		};
typedef struct mrt_ipc_s mrt_ipc_t;
typedef struct mrt_ipc_s mrt_rqst_t;
typedef struct mrt_ipc_s mrt_arqst_t;
typedef struct mrt_ipc_s mrt_rply_t;
typedef struct mrt_ipc_s mrt_sign_t;
typedef struct mrt_ipc_s mrt_uprq_t;
typedef struct mrt_ipc_s mrt_rcv_t;


/*---------------- MINIX4RT SYSTEM CALLS FUNCTION PROTOTYPES -------------*/

_PROTOTYPE( int  mrt_getiattr,(int irq, mrt_irqattr_t *irqd)	);
_PROTOTYPE( int  mrt_getistat,(int irq, mrt_irqstat_t *irqd)	);
_PROTOTYPE( int  mrt_getiint, (int irq, mrt_irqint_t  *irqd)	);
_PROTOTYPE( int  mrt_setiattr,(int irq, mrt_irqattr_t *irqd)	);

_PROTOTYPE( int  mrt_getsstat, (mrt_sysstat_t *stats)			);
_PROTOTYPE( int  mrt_getsval, (mrt_sysval_t *vals)			);
_PROTOTYPE( int  mrt_setsval, (unsigned int flags)			);
_PROTOTYPE( int  mrt_restart, (int harmonic, int refresh)		);

_PROTOTYPE( int  mrt_setpattr, (mrt_pattr_t *pattr)			);
_PROTOTYPE( int  mrt_getpattr, (pid_t pid, mrt_pattr_t *pattr)	);

_PROTOTYPE( int  mrt_getpstat,(pid_t pid, mrt_pstat_t *pstat)	);
_PROTOTYPE( int  mrt_getpint,(pid_t pid, mrt_pint_t *pint)  	);
_PROTOTYPE( int  mrt_clrpstat,(pid_t pid )				);

_PROTOTYPE( int  mrt_RTstart, (int harmonic, int refresh)		);
_PROTOTYPE( int  mrt_RTstop, (void )					);

_PROTOTYPE( int  mrt_semalloc,(mrt_semattr_t *s)			);
_PROTOTYPE( int  mrt_semfree,(int semid)					);
_PROTOTYPE( int  mrt_getsemstat,(int semid, mrt_semstat_t *s)  	);
_PROTOTYPE( int  mrt_getsemint,(int semid, mrt_semint_t *s)		);
_PROTOTYPE( int  mrt_getsemid,(char *sname)				);
_PROTOTYPE( int  mrt_getsemattr,(int semid, mrt_semattr_t *s)  	);

_PROTOTYPE( int  mrt_getmqstat,(int msgqid, mrt_msgqstat_t *s)  	);

_PROTOTYPE( int  mrt_clrsstat,(void )					);
_PROTOTYPE( int  mrt_clristat,(int irq)					);
_PROTOTYPE( int  mrt_clrsemstat,(int semid)			  	);
_PROTOTYPE( int  mrt_clrmqstat,(int msgqid)			  	);


/*---------------- MINIX4RT KERNEL CALLS FUNCTION PROTOTYPES -------------*/

_PROTOTYPE( int mrt_set2rt,(void )						);
_PROTOTYPE( int mrt_set2nrt,(void )						);
_PROTOTYPE( int mrt_sleep,(lcounter_t timeout)				);
_PROTOTYPE( int mrt_wakeup,(mrtpid_t mrtpid)				);
_PROTOTYPE( int mrt_rqst,(mrtpid_t mrtpid, mrt_msg_t *m_ptr, lcounter_t timeout ));
_PROTOTYPE( int mrt_arqst,(mrtpid_t mrtpid, mrt_msg_t *m_ptr));
_PROTOTYPE( int mrt_reply,(mrtpid_t mrtpid, mrt_msg_t *m_ptr));
_PROTOTYPE( int mrt_rcv, (mrtpid_t mrtpid, mrt_msg_t *msg, mrt_mhdr_t *hdr, lcounter_t tout));
_PROTOTYPE( int mrt_uprqst,(mrtpid_t mrtpid, mrt_msg_t *m_ptr, int priority ));
_PROTOTYPE( int mrt_sign,(mrtpid_t mrtpid, mrt_msg_t *m_ptr ));
_PROTOTYPE( int mrt_print,(char *string)					);
_PROTOTYPE( int mrt_rqrcv,(mrtpid_t mrtpid, mrt_msg_t *rqst,mrt_msg_t *rply, mrt_mhdr_t *hdr, lcounter_t tout));
_PROTOTYPE( int mrt_semup,(int semid)					);
_PROTOTYPE( int mrt_semdown,(int semid, lcounter_t tout)		);

#endif /* MRT */

#endif /* _UNISTD_H */
