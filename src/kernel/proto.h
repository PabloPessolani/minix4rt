/* Function prototypes. */

#ifndef PROTO_H
#define PROTO_H

/* Struct declarations. */
struct dpeth;
struct proc;
struct tty;

#ifdef MRT
#define MRT_disable_irqd(d) 		d->flags |=  MRT_DISABLED
#define MRT_enable_irqd(d) 		d->flags &= ~MRT_DISABLED
#endif

/* at_wini.c, bios_wini.c, esdi_wini.c, ps_wini.c, xt_wini.c, wini.c */
_PROTOTYPE( void at_winchester_task, (void)				);
_PROTOTYPE( void bios_winchester_task, (void)				);
_PROTOTYPE( void esdi_winchester_task, (void)				);
_PROTOTYPE( void ps_winchester_task, (void)				);
_PROTOTYPE( void xt_winchester_task, (void)				);

/* aha1540.c */
_PROTOTYPE( void aha1540_scsi_task, (void)				);

/* dosfat.c, dosfile.c */
_PROTOTYPE( void dosfat_task, (void)					);
_PROTOTYPE( void dosfile_task, (void)					);
_PROTOTYPE( void dosfile_stop, (void)					);

/* clock.c */
_PROTOTYPE( void clock_task, (void)						);
_PROTOTYPE( void clock_stop, (void)						);
_PROTOTYPE( clock_t get_uptime, (void)					);
_PROTOTYPE( void syn_alrm_task, (void)					);

/* mrtvt.c */
#ifdef MRT
_PROTOTYPE( int clock_handler, (int irq) 					);

_PROTOTYPE( void MRT_timer_set, (unsigned int	harmonic )   	);
_PROTOTYPE( void MRT_timer2_set, (unsigned int	latch)	  	);
_PROTOTYPE( void MRT_timer_oneshot,(int period)				);

_PROTOTYPE( unsigned int MRT_read_timer2, (void)			);
_PROTOTYPE( int MRT_clock_handler, (int irq)				);
_PROTOTYPE( void MRT_timerQ_init, (void)					);
_PROTOTYPE( void MRT_vtimer_init, (void)					);
_PROTOTYPE( void MRT_vtimer_free,(MRT_vtimer_t *vt)			);
_PROTOTYPE( void MRT_vtimer_stop,(MRT_vtimer_t *vt)			);
_PROTOTYPE( void MRT_vtimer_rst,(MRT_vtimer_t *vt)			);
_PROTOTYPE( MRT_vtimer_t *MRT_vtimer_alloc, (MRT_vtimer_t *vt)	);
_PROTOTYPE( void MRT_vtimer_ins,(MRT_vtimer_t *vt)			);
_PROTOTYPE( void MRT_vtimer_rmv,(MRT_vtimer_t *vt) 			);
_PROTOTYPE( void MRT_vtimer_run,(MRT_vtimer_t *vt)			);
_PROTOTYPE( void MRT_vtimer_MINIX,(lcounter_t per)			);
_PROTOTYPE( void MRT_vtimer_IDLE,(int prty)				);

#endif /* MRT */

/* dmp.c */
_PROTOTYPE( void map_dmp, (void)						);
_PROTOTYPE( void p_dmp, (void)						);
_PROTOTYPE( void reg_dmp, (struct proc *rp)				);
#ifdef MRT
_PROTOTYPE(char *proc_name, (int proc_nr)					);
#endif


/* mrtdmp.c */
#ifdef MRT
_PROTOTYPE( void MRT_test, (void)						);
_PROTOTYPE( void MRT_irq_dmp, (void)					);
_PROTOTYPE( void MRT_soft_dmp, (void)					);
_PROTOTYPE( void MRT_patt_dmp, (void)					);
_PROTOTYPE( void MRT_pstat_dmp, (void)					);
_PROTOTYPE( void MRT_counter_dmp, (void)					);
_PROTOTYPE( void MRT_rdyQ_dmp, (void)					);
_PROTOTYPE( void MRT_irqQ_dmp, (void)					);
_PROTOTYPE( void MRT_vtimer_dmp, (void)					);
_PROTOTYPE( void MRT_vtQ_dmp, (void)					);
_PROTOTYPE( void MRT_mpool_dmp, (void)					);
_PROTOTYPE( void MRT_mpool2_dmp, (void)					);
_PROTOTYPE( void MRT_msgQ_dmp, (void)  					);
_PROTOTYPE( void MRT_latency_dmp, (void)					);
_PROTOTYPE( void MRT_sem_dmp, (void)					);
_PROTOTYPE( void MRT_psem_dmp, (void)					);
_PROTOTYPE( void put232, (char c)						);
#endif /* MRT */

/* dp8390.c */
_PROTOTYPE( void dp8390_task, (void)					);
_PROTOTYPE( void dp_dump, (void)						);
_PROTOTYPE( void dp8390_stop, (void)					);

/* floppy.c, stfloppy.c */
_PROTOTYPE( void floppy_task, (void)					);
_PROTOTYPE( void floppy_stop, (void)					);

/* main.c, stmain.c */
_PROTOTYPE( void main, (void)							);
_PROTOTYPE( void panic, (const char *s, int n)				);
#ifdef MRT
_PROTOTYPE( void MRT_idle, (void)						);
_PROTOTYPE( void MRT_idle_init, (void)					);
_PROTOTYPE( int MRT_idle_handler, (int irq)				);
#endif /* MRT */

/* mcd.c */
_PROTOTYPE( void mcd_task, (void)						);

/* memory.c */
_PROTOTYPE( void mem_task, (void)						);

/* misc.c */
_PROTOTYPE( int env_parse, (char *env, char *fmt, int field,
			long *param, long min, long max)			);

/* printer.c, stprint.c */
_PROTOTYPE( void printer_task, (void)					);

/* proc.c */
_PROTOTYPE( void interrupt, (int task)					);
_PROTOTYPE( int lock_mini_send, (struct proc *caller_ptr,
		int dest, message *m_ptr)					);
_PROTOTYPE( void lock_pick_proc, (void)					);
_PROTOTYPE( void lock_ready, (struct proc *rp)				);
_PROTOTYPE( void lock_sched, (void)						);
_PROTOTYPE( void lock_unready, (struct proc *rp)			);
_PROTOTYPE( int sys_call, (int function, int src_dest, message *m_ptr)	);
_PROTOTYPE( void unhold, (void)						);

#ifdef MRT
/* mrtproc.c */
_PROTOTYPE( int rtk_call, (int rtkcall, int rtk_nr, void *parm_ptr));
_PROTOTYPE( void MRT_rdyQ_init,(void)					);
_PROTOTYPE( void MRT_rdyQ_app,(MRT_proc_t	*pp)				);
_PROTOTYPE( void MRT_rdyQ_ins,(MRT_proc_t	*pp)				);
_PROTOTYPE( void MRT_rdyQ_rmv, (MRT_proc_t	*pp)			);
_PROTOTYPE( int MRT_pick_proc,(void)					);
_PROTOTYPE( void MRT_mpool_init,(void )					);
_PROTOTYPE( void MRT_proc_init,(void )					);
_PROTOTYPE( void MRT_proc_rst,(struct proc *rp)				);
_PROTOTYPE( void MRT_pstat_rst,(struct proc *rp)			);
_PROTOTYPE( void MRT_pattr_rst,(struct proc *rp)			);
_PROTOTYPE( int MRT_set2nrt, (MRT_proc_t	*rp) 				);
_PROTOTYPE( int MRT_wakeup,(struct proc *rp)				);
_PROTOTYPE( int MRT_sleep,(struct proc *rp,long int timeout)	);
_PROTOTYPE( void MRT_prtylvl_set,(int  priority)			);

/* mrtmsg.c */
_PROTOTYPE( void MRT_msgQ_init,(void )					);
_PROTOTYPE( MRT_msgQ_t *MRT_msgQ_alloc,(MRT_msgQ_t *mq)		);
_PROTOTYPE( void MRT_msgQ_rst,(MRT_msgQ_t *mq)				);
_PROTOTYPE( void MRT_msgQ_free,(MRT_msgQ_t *mq)				);
_PROTOTYPE( void MRT_mqe_rst,(MRT_mqe_t *mqe)				);
_PROTOTYPE( MRT_mqe_t *MRT_mqe_alloc,(void)				);
_PROTOTYPE( void MRT_mqe_free,(MRT_mqe_t *mqe)				);
_PROTOTYPE( int  MRT_mqe_app,(MRT_msgQ_t *pmq, MRT_mqe_t *mqe)	);
_PROTOTYPE( void MRT_mqe_rmv,(MRT_msgQ_t *pmq, MRT_mqe_t *mqe)	);
_PROTOTYPE( MRT_mqe_t *MRT_mqe_pick,(MRT_msgQ_t *pmq, int source)	);
_PROTOTYPE( int MRT_ksend, (int src, int dest,int prty, int mtype));
_PROTOTYPE( int rtk_set2rt, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_set2nrt, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_sleep, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_wakeup, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_rqst, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_reply, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_arqst, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_print, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_uprqst, (void *parm_ptr)				);
_PROTOTYPE( int rtk_sign, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_receive, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_rqrcv, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_semup, (void *parm_ptr) 				);
_PROTOTYPE( int rtk_semdown, (void *parm_ptr) 				);

/* mrtsem.c */
_PROTOTYPE( void MRT_spool_init,(void )					);
_PROTOTYPE( void MRT_sem_rst,(MRT_sem_t	*psem)			);
_PROTOTYPE( MRT_sem_t *MRT_sem_alloc,(MRT_proc_t *pproc)		);
_PROTOTYPE( void MRT_sem_free,(MRT_proc_t *pproc,MRT_sem_t *psem)	);
_PROTOTYPE( void MRT_sproc_app,(MRT_sem_t *psem,MRT_proc_t *pproc));
_PROTOTYPE( void MRT_sproc_rmv,(MRT_sem_t	*psem,MRT_proc_t *pproc));
_PROTOTYPE( MRT_proc_t *MRT_sproc_pick,(MRT_sem_t*psem)		);

_PROTOTYPE( int MRT_semup,(MRT_sem_t *psem)				);
_PROTOTYPE( int MRT_semdown,(MRT_sem_t *psem, lcounter_t timeout)	);
_PROTOTYPE( void MRT_sem_unlock,(MRT_proc_t *rp)			);
_PROTOTYPE( void MRT_sem_empty,(MRT_proc_t *rp)				);

/* mrtlib.c */
_PROTOTYPE( void MRT_link_rst,(link_t *plink, void *this, int objtype));

_PROTOTYPE( void MRT_list_rst, (list_t *pl)				);
_PROTOTYPE( void MRT_list_app,(list_t *plist, link_t *plink)	);
_PROTOTYPE( void MRT_list_ins,(list_t *plist, link_t *plink)	);
_PROTOTYPE( void MRT_list_rmv,(list_t *plist, link_t *plink)	);

_PROTOTYPE( void MRT_plist_rst, (plist_t *pl)				);
_PROTOTYPE( void MRT_plist_app,(plist_t *plist, link_t *plink,int pty)	);
_PROTOTYPE( void MRT_plist_ins,(plist_t *plist, link_t *plink,int pty)	);
_PROTOTYPE( void MRT_plist_rmv,(plist_t *plist, link_t *plink,int pty)	);
_PROTOTYPE( link_t *MRT_plist_get,(plist_t *plist)			);

_PROTOTYPE( struct proc *MRT_pid2pptr,( pid_t pid)			);
_PROTOTYPE(void int2bin,    (int number, char *bin)			);

_PROTOTYPE( void MRT_inherit,(int prty, MRT_proc_t *dest_ptr)	);
_PROTOTYPE( void MRT_disinherit, (struct proc *dest_ptr)		);
#endif /* MRT */

/* rs232.c */
_PROTOTYPE( void rs_init, (struct tty *tp)				);
_PROTOTYPE( void MRT_rs_init, (void)					);

/* sb16_dsp.c */
_PROTOTYPE( void dsp_task, (void)						);

/* sb16_mixer.c */
_PROTOTYPE( void mixer_task, (void)						);

/* system.c */
_PROTOTYPE( void cause_sig, (int proc_nr, int sig_nr)			);
_PROTOTYPE( void inform, (void)						);
_PROTOTYPE( phys_bytes numap, (int proc_nr, vir_bytes vir_addr, 
		vir_bytes bytes)					);
_PROTOTYPE( void sys_task, (void)					);
_PROTOTYPE( phys_bytes umap, (struct proc *rp, int seg, vir_bytes vir_addr,
		vir_bytes bytes)						);


#ifdef MRT
/* mrttask.c */
_PROTOTYPE( void MRT_sys_task, (void)					);
#endif /* MRT */

/* table.c */
_PROTOTYPE( void mapdrivers, (void)						);

/* tty.c */
_PROTOTYPE( void handle_events, (struct tty *tp)			);
_PROTOTYPE( void sigchar, (struct tty *tp, int sig)			);
_PROTOTYPE( void tty_task, (void)						);
_PROTOTYPE( int in_process, (struct tty *tp, char *buf, int count));
_PROTOTYPE( void out_process, (struct tty *tp, char *bstart, char *bpos,
				char *bend, int *icount, int *ocount)	);
_PROTOTYPE( void tty_wakeup, (clock_t now)				);
_PROTOTYPE( void tty_reply, (int code, int replyee, int proc_nr,
							int status)	);
_PROTOTYPE( void tty_devnop, (struct tty *tp)				);

/* library */
_PROTOTYPE( void *memcpy, (void *_s1, const void *_s2, size_t _n)	);

#if (CHIP == INTEL)

/* 3c503.c */
_PROTOTYPE( int el2_probe, (struct dpeth *dep)				);

/* clock.c */
_PROTOTYPE( void milli_start, (struct milli_state *msp)		);
_PROTOTYPE( unsigned milli_elapsed, (struct milli_state *msp)	);
_PROTOTYPE( void milli_delay, (unsigned millisec)			);

/* console.c */
_PROTOTYPE( void cons_stop, (void)						);
_PROTOTYPE( void putk, (int c)						);
_PROTOTYPE( void scr_init, (struct tty *tp)				);
_PROTOTYPE( void toggle_scroll, (void)					);
_PROTOTYPE( int con_loadfont, (phys_bytes user_phys)			);
_PROTOTYPE( void select_console, (int cons_line)			);

/* cstart.c */
_PROTOTYPE( void cstart, (U16_t cs, U16_t ds, U16_t mds,
				U16_t parmoff, U16_t parmsize)		);
_PROTOTYPE( char *k_getenv, (char *name)					);

/* exception.c */
_PROTOTYPE( void exception, (unsigned vec_nr)				);

/* i8259.c */
_PROTOTYPE( irq_handler_t get_irq_handler, (int irq)			);
_PROTOTYPE( void put_irq_handler, (int irq, irq_handler_t handler));
_PROTOTYPE( void intr_init, (int mine)					);
#ifdef MRT
_PROTOTYPE( int spurious_irq, (int irq) 					);
#endif
/* mrtirq.c */
#ifdef MRT
_PROTOTYPE( int MRT_softirq_alloc, (void)					); 
_PROTOTYPE( void MRT_softirq_free, (int irq)				); 
_PROTOTYPE( void MRT_irqQ_init, (void)					);

_PROTOTYPE( void MRT_irqd_set, (int irq, MRT_irqd_t *p_irqd)); 
_PROTOTYPE( void MRT_irqd_free, (MRT_irqd_t *p_irqd)	); 

_PROTOTYPE( int MRT_IRQ_dispatch, (int irq)				);
_PROTOTYPE( void MRT_flush_int, (void)					);

_PROTOTYPE( int MRT_testbm_si, (int p)					);
_PROTOTYPE( int MRT_testbm_st, (int p)					);

_PROTOTYPE( void lock, (void)							);
_PROTOTYPE( void unlock, (void)						);

_PROTOTYPE( void enable_irq, (unsigned irq)				);
_PROTOTYPE( int disable_irq, (unsigned irq)				);

_PROTOTYPE( void MRT_irqd_enQ,(MRT_irqd_t  *p)				);
_PROTOTYPE( void MRT_irqd_deQ,(MRT_irqd_t  *p)				);
_PROTOTYPE( void MRT_irqd_rst,(MRT_irqd_t  *p)				);
_PROTOTYPE( void MRT_irqd_clr,(MRT_irqd_t  *p)				);

_PROTOTYPE( void irqd_clrstat, (MRT_irqd_t *d)				);

_PROTOTYPE( void MRT_set_irq,(unsigned int irq)				);
_PROTOTYPE( void MRT_reset_irq,(unsigned int irq)			);

_PROTOTYPE( void MRT_irqd_lock,(unsigned int irq)			);
_PROTOTYPE( void MRT_irqd_unlock,(unsigned int irq)			);

_PROTOTYPE( int MRT_do_handler,(MRT_irqd_t  	*d)			);

#endif /* MRT */


/* keyboard.c */
_PROTOTYPE( void kb_init, (struct tty *tp)				);
_PROTOTYPE( int kbd_loadmap, (phys_bytes user_phys)			);
_PROTOTYPE( void wreboot, (int how)						);

/* klib*.s */
_PROTOTYPE( void int86, (void)						);
_PROTOTYPE( void cp_mess, (int src,phys_clicks src_clicks,vir_bytes src_offset,
		phys_clicks dst_clicks, vir_bytes dst_offset)		);
_PROTOTYPE( int in_byte, (port_t port)					);
_PROTOTYPE( int in_word, (port_t port)					);
#ifdef MRT
_PROTOTYPE( void MRT_lock, (void)						);
_PROTOTYPE( void MRT_unlock, (void)			 			);
_PROTOTYPE( void MRT_saveFunlock, (long *flags)				);
_PROTOTYPE( void MRT_saveFlock, (long *flags)				);
_PROTOTYPE( void MRT_restoreF, (long *flags)	 			);
_PROTOTYPE( void MRT_enable_irq, (unsigned irq)				);
_PROTOTYPE( int  MRT_disable_irq, (unsigned irq)			);
_PROTOTYPE( void MRT_set_PIC, (unsigned mask)      			);
_PROTOTYPE( unsigned MRT_get_PIC, (void)        			);
_PROTOTYPE( void MRT_enable_PIC, (unsigned irq)     			);
#else
_PROTOTYPE( void lock, (void)							);
_PROTOTYPE( void unlock, (void)						);
_PROTOTYPE( void enable_irq, (unsigned irq)				);
_PROTOTYPE( int disable_irq, (unsigned irq)				);
#endif  /* MRT */
_PROTOTYPE( u16_t mem_rdw, (segm_t segm, vir_bytes offset)		);
_PROTOTYPE( void out_byte, (port_t port, int value)			);
_PROTOTYPE( void out_word, (port_t port, int value)			);
_PROTOTYPE( void phys_copy, (phys_bytes source, phys_bytes dest,
		phys_bytes count)							);
_PROTOTYPE( void port_read, (unsigned port, phys_bytes destination,
		unsigned bytcount)						);
_PROTOTYPE( void port_read_byte, (unsigned port, phys_bytes destination,
		unsigned bytcount)						);
_PROTOTYPE( void port_write, (unsigned port, phys_bytes source,
		unsigned bytcount)						);
_PROTOTYPE( void port_write_byte, (unsigned port, phys_bytes source,
		unsigned bytcount)						);
_PROTOTYPE( void reset, (void)						);
_PROTOTYPE( void vid_vid_copy, (unsigned src, unsigned dst, unsigned count));
_PROTOTYPE( void mem_vid_copy, (u16_t *src, unsigned dst, unsigned count));
_PROTOTYPE( void level0, (void (*func)(void))				);
_PROTOTYPE( void monitor, (void)					);

/* misc.c */
_PROTOTYPE( void mem_init, (void)					);

/* mpx*.s */
_PROTOTYPE( void idle_task, (void)					);
_PROTOTYPE( void restart, (void)					);

/* The following are never called from C (pure asm procs). */

/* Exception handlers (real or protected mode), in numerical order. */
void _PROTOTYPE( int00, (void) ), _PROTOTYPE( divide_error, (void) );
void _PROTOTYPE( int01, (void) ), _PROTOTYPE( single_step_exception, (void) );
void _PROTOTYPE( int02, (void) ), _PROTOTYPE( nmi, (void) );
void _PROTOTYPE( int03, (void) ), _PROTOTYPE( breakpoint_exception, (void) );
void _PROTOTYPE( int04, (void) ), _PROTOTYPE( overflow, (void) );
void _PROTOTYPE( int05, (void) ), _PROTOTYPE( bounds_check, (void) );
void _PROTOTYPE( int06, (void) ), _PROTOTYPE( inval_opcode, (void) );
void _PROTOTYPE( int07, (void) ), _PROTOTYPE( copr_not_available, (void) );
void				  _PROTOTYPE( double_fault, (void) );
void				  _PROTOTYPE( copr_seg_overrun, (void) );
void				  _PROTOTYPE( inval_tss, (void) );
void				  _PROTOTYPE( segment_not_present, (void) );
void				  _PROTOTYPE( stack_exception, (void) );
void				  _PROTOTYPE( general_protection, (void) );
void				  _PROTOTYPE( page_fault, (void) );
void				  _PROTOTYPE( copr_error, (void) );

/* Hardware interrupt handlers. */
_PROTOTYPE( void hwint00, (void) );
_PROTOTYPE( void hwint01, (void) );
_PROTOTYPE( void hwint02, (void) );
_PROTOTYPE( void hwint03, (void) );
_PROTOTYPE( void hwint04, (void) );
_PROTOTYPE( void hwint05, (void) );
_PROTOTYPE( void hwint06, (void) );
_PROTOTYPE( void hwint07, (void) );
_PROTOTYPE( void hwint08, (void) );
_PROTOTYPE( void hwint09, (void) );
_PROTOTYPE( void hwint10, (void) );
_PROTOTYPE( void hwint11, (void) );
_PROTOTYPE( void hwint12, (void) );
_PROTOTYPE( void hwint13, (void) );
_PROTOTYPE( void hwint14, (void) );
_PROTOTYPE( void hwint15, (void) );

/* Software interrupt handlers, in numerical order. */
_PROTOTYPE( void trp, (void) );
_PROTOTYPE( void s_call, (void) ), _PROTOTYPE( p_s_call, (void) );
_PROTOTYPE( void level0_call, (void) );

/* ne2000.c */
_PROTOTYPE( int ne_probe, (struct dpeth *dep)				);

/* printer.c */
_PROTOTYPE( void pr_restart, (void)					);


/* protect.c */
_PROTOTYPE( void prot_init, (void)					);
_PROTOTYPE( void init_codeseg, (struct segdesc_s *segdp, phys_bytes base,
		phys_bytes size, int privilege)				);
_PROTOTYPE( void init_dataseg, (struct segdesc_s *segdp, phys_bytes base,
		phys_bytes size, int privilege)				);
_PROTOTYPE( phys_bytes seg2phys, (U16_t seg)				);
_PROTOTYPE( void enable_iop, (struct proc *pp)				);

/* pty.c */
_PROTOTYPE( void do_pty, (struct tty *tp, message *m_ptr)		);
_PROTOTYPE( void pty_init, (struct tty *tp)				);

/* system.c */
_PROTOTYPE( void alloc_segments, (struct proc *rp)			);

/* wdeth.c */
_PROTOTYPE( int wdeth_probe, (struct dpeth *dep)			);

#endif /* (CHIP == INTEL) */

#if (CHIP == M68000)

/* cstart.c */
_PROTOTYPE( void cstart, (char *parmoff, size_t parmsize)		);

/* stfloppy.c */
_PROTOTYPE( void fd_timer, (void)					);

/* stmain.c */
_PROTOTYPE( void none, (void)						);
_PROTOTYPE( void rupt, (void)						);
_PROTOTYPE( void trap, (void)						);
_PROTOTYPE( void checksp, (void)					);
_PROTOTYPE( void aciaint, (void)					);
_PROTOTYPE( void fake_int, (const char *s, int t)			);
_PROTOTYPE( void timint, (int t)					);
_PROTOTYPE( void mdiint, (void)						);
_PROTOTYPE( void iob, (int t)						);
_PROTOTYPE( void idle_task, (void)					);

/* rs232.c */
_PROTOTYPE( void siaint, (int type)					);

/* stcon.c */
_PROTOTYPE( void func_key, (void)					);
_PROTOTYPE( void dump, (void)						);
_PROTOTYPE( void putk, (int c)						);

/* stdma.c */
_PROTOTYPE( void dmagrab, (int p, dmaint_t func)			);
_PROTOTYPE( void dmafree, (int p)					);
_PROTOTYPE( void dmaint, (void)						);
_PROTOTYPE( void dmaaddr, (phys_bytes ad)				);
_PROTOTYPE( int dmardat, (int mode, int delay)				);
_PROTOTYPE( void dmawdat, (int mode, int data, int delay)		);
_PROTOTYPE( void dmawcmd, (int data, unsigned mode)			);
_PROTOTYPE( void dmacomm, (int mode, int data, int delay)		);
_PROTOTYPE( int dmastat, (int mode, int delay)				);

/* stdskclk.c */
_PROTOTYPE( int do_xbms, (phys_bytes address, int count, int rw, int minor) );
 
/* stkbd.c */
_PROTOTYPE( void kbdint, (void)						);
_PROTOTYPE( void kb_timer, (void)					);
_PROTOTYPE( int kb_read, (int minor, char **bufindirect)		);
_PROTOTYPE( void kb_init, (int minor)					);

/* stshadow.c */
_PROTOTYPE( void mkshadow, (struct proc *p, phys_clicks c2)		);
_PROTOTYPE( void rmshadow, (struct proc *p, phys_clicks *basep,
		phys_clicks *sizep)					);
_PROTOTYPE( void unshadow, (struct proc *p)				);
 
/* stvdu.c */
_PROTOTYPE( void flush, (struct tty *tp)				);
_PROTOTYPE( void console, (struct tty *tp)				);
_PROTOTYPE( void out_char, (struct tty *tp, int c)			);
_PROTOTYPE( void scr_init, (int minor)					);
_PROTOTYPE( void vduswitch, (struct tty *tp)				);
_PROTOTYPE( void vdusetup, (unsigned int vres, char *vram,
			    unsigned short *vrgb)			);
_PROTOTYPE( void vbl, (void)						);
_PROTOTYPE( int vdu_loadfont, (message *m_ptr)				);

/* stwini.c */
_PROTOTYPE( int wini_open, (message *mp)				);
_PROTOTYPE( int wini_rdwt, (message *mp)				);
_PROTOTYPE( int wini_hvrdwt, (message *mp)				);
_PROTOTYPE( int wini_transfer, (int rw, int pnr, int minor,
		long pos, int count, vir_bytes vadr)			);
_PROTOTYPE( int wini_ioctl, (message *mp)				);
_PROTOTYPE( int wini_close, (message *mp)				);

/* stacsi.c */
_PROTOTYPE( int acsi_cmd, (int drive,  unsigned char *cmd, int cmdlen,
		phys_bytes address, phys_bytes data_len,  int rw)	);

/* stscsi.c */
_PROTOTYPE( void scsi_task, (void)					);
_PROTOTYPE( void scsidmaint, (void)					);
_PROTOTYPE( void scsiint, (void)					);
_PROTOTYPE( int scsi_cmd, (int drive,  unsigned char *cmd, int cmdlen,
		phys_bytes address, phys_bytes data_len,  int rw)	);

/* klib68k.s */
_PROTOTYPE( void flipclicks, (phys_clicks c1, phys_clicks c2, phys_clicks n) );
_PROTOTYPE( void copyclicks, (phys_clicks src, phys_clicks dest,
		phys_clicks nclicks)					);
_PROTOTYPE( void zeroclicks, (phys_clicks dest, phys_clicks nclicks)	);
_PROTOTYPE( void phys_copy, (phys_bytes src, phys_bytes dest, phys_bytes n) );

/* stdskclks.s */
_PROTOTYPE( int rd1byte, (void)						);
_PROTOTYPE( int wr1byte, (int)						);
_PROTOTYPE( long getsupra, (void)					);
_PROTOTYPE( long geticd, (void)						);

/* mpx.s */
_PROTOTYPE( int lock, (void)						);
_PROTOTYPE( void unlock, (void)						);
_PROTOTYPE( void restore, (int oldsr)					);
_PROTOTYPE( void reboot, (void)						);
_PROTOTYPE( int test_and_set, (char *flag)				);
_PROTOTYPE( unsigned long get_mem_size, (char *start_addr)		);

/* stprint.c */
#ifdef DEBOUT
_PROTOTYPE( void prtc, (int c)						);
#endif

#ifdef FPP
/* fpp.c */
_PROTOTYPE( void fppinit, (void)					);
_PROTOTYPE( void fpp_new_state, (struct proc *rp)			);
_PROTOTYPE( void fpp_save, (struct proc *rp, struct cpu_state *p)	);
_PROTOTYPE( struct cpu_state  *fpp_restore, (struct proc *rp)		);

/* fpps.s */
_PROTOTYPE( void _fppsave, (struct state_frame *p)			);
_PROTOTYPE( void _fppsavereg, (struct fpp_model *p)			);
_PROTOTYPE( void _fpprestore, (struct state_frame *p)			);
_PROTOTYPE( void _fpprestreg, (struct fpp_model *p)			);
#endif

#if (SHADOWING == 0)
/* pmmu.c */
_PROTOTYPE(void pmmuinit , (void)					);
_PROTOTYPE(void pmmu_init_proc , (struct proc *rp )			);
_PROTOTYPE(void pmmu_restore , (struct proc *rp )			);
_PROTOTYPE(void pmmu_delete , (struct proc *rp )			);
_PROTOTYPE(void pmmu_flush , (struct proc *rp )				);
#endif

#endif /* (CHIP == M68000) */

#endif /* PROTO_H */
