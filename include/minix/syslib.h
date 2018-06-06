
/* Prototypes for system library functions. */

#ifndef _SYSLIB_H
#define _SYSLIB_H

/* Hide names to avoid name space pollution. */
#define sendrec		_sendrec
#define receive		_receive
#define send		_send


#ifdef MRT

#define MRT_ANYPROC	-(NR_TASKS+100)		

#define rtkcall		_rtkcall

#define MRT_NCALLS		28
#define MRT_FIRSTCALL		100

#define MRT_GETIATTR		(MRT_FIRSTCALL + 0)
#define MRT_GETISTAT		(MRT_FIRSTCALL + 1)
#define MRT_GETIINT		(MRT_FIRSTCALL + 2)
#define MRT_SETIATTR		(MRT_FIRSTCALL + 3)

#define MRT_GETSSTAT		(MRT_FIRSTCALL + 4)
#define MRT_GETSVAL 		(MRT_FIRSTCALL + 5)
#define MRT_SETSVAL 		(MRT_FIRSTCALL + 6)
#define MRT_RESTART		(MRT_FIRSTCALL + 7)

#define MRT_GETPATTR		(MRT_FIRSTCALL + 8)
#define MRT_SETPATTR		(MRT_FIRSTCALL + 9)

#define MRT_GETPSTAT		(MRT_FIRSTCALL + 10)
#define MRT_GETPINT		(MRT_FIRSTCALL + 11)
#define MRT_CLRPSTAT		(MRT_FIRSTCALL + 12)

#define MRT_RTSTART		(MRT_FIRSTCALL + 13)
#define MRT_RTSTOP 		(MRT_FIRSTCALL + 14)

#define MRT_EXIT			(MRT_FIRSTCALL + 15)
#define MRT_STDSIGNAL 		(MRT_FIRSTCALL + 16)

#define MRT_SEMALLOC		(MRT_FIRSTCALL + 17)
#define MRT_SEMFREE 		(MRT_FIRSTCALL + 18)
#define MRT_GETSEMSTAT 		(MRT_FIRSTCALL + 19)
#define MRT_GETSEMINT		(MRT_FIRSTCALL + 20)
#define MRT_GETSEMID		(MRT_FIRSTCALL + 21)
#define MRT_GETSEMATTR 		(MRT_FIRSTCALL + 22)

#define MRT_GETMQSTAT 		(MRT_FIRSTCALL + 23)

#define MRT_CLRSSTAT 		(MRT_FIRSTCALL + 24)
#define MRT_CLRISTAT 		(MRT_FIRSTCALL + 25)
#define MRT_CLRSEMSTAT 		(MRT_FIRSTCALL + 26)
#define MRT_CLRMQSTAT 		(MRT_FIRSTCALL + 27)

#endif


/* Minix user+system library. */
_PROTOTYPE( void printk, (char *_fmt, ...)				);

#ifdef MRT
_PROTOTYPE( void printf232, (char *_fmt, ...)				);
_PROTOTYPE( int rtkcall, (int _rtk_nr, void *_parm_ptr)		);

_PROTOTYPE(int rtt_getiattr, (int  caller,int  irq,char *ptr)	);
_PROTOTYPE(int rtt_getistat,(int  caller,int  irq,char *ptr)	);
_PROTOTYPE(int rtt_getiint, (int  caller,int  irq,char *ptr)	);
_PROTOTYPE(int rtt_setiattr, (int  caller,int  irq,char *ptr)	);

_PROTOTYPE(int rtt_getsstat, (int  caller,char *ptr)	 		);
_PROTOTYPE(int rtt_getsval,  (int  caller,char *ptr)			);
_PROTOTYPE(int rtt_setsval,  (int  caller,unsigned int flags)	);
_PROTOTYPE(int rtt_restart,  (int  caller,int harmonic, int refresh));

_PROTOTYPE(int rtt_setpattr, (int  caller,char *ptr)			);
_PROTOTYPE(int rtt_getpattr, (int  caller,int  pid,char *ptr)	);

_PROTOTYPE(int rtt_getpstat, (int caller,int  pid,char *ptr)	);
_PROTOTYPE(int rtt_getpint,  (int caller,int  pid,char *ptr)	);
_PROTOTYPE(int rtt_clrpstat, (int caller,int  pid)			);

_PROTOTYPE( int rtt_rtstart, (int caller,int _harmonic, int refresh));
_PROTOTYPE( int rtt_rtstop, (int caller)					);

_PROTOTYPE( int rtt_exit, (int caller)					);
_PROTOTYPE( int rtt_sendsig, (int caller,int pid)			);

_PROTOTYPE(int rtt_semalloc, (int  caller,char *ptr)			);
_PROTOTYPE(int rtt_semfree, (int  caller, int semid)			);
_PROTOTYPE(int rtt_getsemstat, (int  caller,int semid, char *ptr)	);
_PROTOTYPE(int rtt_getsemint, (int  caller,int semid, char *ptr)	);
_PROTOTYPE(int rtt_getsemid, (int  caller, char *ptr)			);
_PROTOTYPE(int rtt_getsemattr, (int  caller,int semid, char *ptr)	);

_PROTOTYPE(int rtt_getmqstat, (int  caller,int msgqid, char *ptr)	);

_PROTOTYPE(int rtt_clrsstat,(int  caller)	 				);
_PROTOTYPE(int rtt_clristat,(int  caller,int  irq)			);
_PROTOTYPE(int rtt_clrsemstat, (int  caller,int semid)		);
_PROTOTYPE(int rtt_clrmqstat, (int  caller,int msgqid)		);

#endif /* MRT */

_PROTOTYPE( int sendrec, (int _src_dest, message *_m_ptr)		);
_PROTOTYPE( int _taskcall, (int _who, int _syscallnr, message *_msgptr)	);

/* Minix system library. */
_PROTOTYPE( int receive, (int _src, message *_m_ptr)			);
_PROTOTYPE( int send, (int _dest, message *_m_ptr)			);

_PROTOTYPE( int sys_abort, (int _how, ...)				);
_PROTOTYPE( int sys_adjmap, (int _proc, struct mem_map *_ptr, 
		vir_clicks _data_clicks, vir_clicks _sp)		);
_PROTOTYPE( int sys_copy, (int _src_proc, int _src_seg, phys_bytes _src_vir, 
	int _dst_proc, int _dst_seg, phys_bytes _dst_vir, phys_bytes _bytes));
_PROTOTYPE( int sys_exec, (int _proc, char *_ptr, int _traced, 
				char *_aout, vir_bytes _initpc)		);
_PROTOTYPE( int sys_execmap, (int _proc, struct mem_map *_ptr)		);
_PROTOTYPE( int sys_fork, (int _parent, int _child, int _pid, 
					phys_clicks _shadow)		);
_PROTOTYPE( int sys_fresh, (int _proc, struct mem_map *_ptr,
	phys_clicks _dc, phys_clicks *_basep, phys_clicks *_sizep)	);
_PROTOTYPE( int sys_getsp, (int _proc, vir_bytes *_newsp)		);
_PROTOTYPE( int sys_newmap, (int _proc, struct mem_map *_ptr)		);
_PROTOTYPE( int sys_getmap, (int _proc, struct mem_map *_ptr)		);
_PROTOTYPE( int sys_sendsig, (int _proc, struct sigmsg *_ptr)		);
_PROTOTYPE( int sys_oldsig, (int _proc, int _sig, sighandler_t _sighandler));
_PROTOTYPE( int sys_endsig, (int _proc)					);
_PROTOTYPE( int sys_sigreturn, (int _proc, vir_bytes _scp, int _flags)	);
_PROTOTYPE( int sys_trace, (int _req, int _procnr, long _addr, long *_data_p));
_PROTOTYPE( int sys_xit, (int _parent, int _proc, phys_clicks *_basep, 
						 phys_clicks *_sizep));
_PROTOTYPE( int sys_kill, (int _proc, int _sig)				);
_PROTOTYPE( int sys_times, (int _proc, clock_t _ptr[5])			);

#endif /* _SYSLIB_H */
