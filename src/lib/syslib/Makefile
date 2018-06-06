# Makefile for lib/syslib.

CFLAGS	= -O -D_MINIX -D_POSIX_SOURCE
CC1	= $(CC) $(CFLAGS) -c

LIBRARY	= ../libc.a
all:	$(LIBRARY)

OBJECTS	= \
	$(LIBRARY)(sys_abort.o) \
	$(LIBRARY)(sys_copy.o) \
	$(LIBRARY)(sys_endsig.o) \
	$(LIBRARY)(sys_exec.o) \
	$(LIBRARY)(sys_fork.o) \
	$(LIBRARY)(sys_fresh.o) \
	$(LIBRARY)(sys_getmap.o) \
	$(LIBRARY)(sys_getsp.o) \
	$(LIBRARY)(sys_kill.o) \
	$(LIBRARY)(sys_newmap.o) \
	$(LIBRARY)(sys_oldsig.o) \
	$(LIBRARY)(sys_sendsig.o) \
	$(LIBRARY)(sys_sigret.o) \
	$(LIBRARY)(sys_times.o) \
	$(LIBRARY)(sys_trace.o) \
	$(LIBRARY)(sys_xit.o) \
	$(LIBRARY)(taskcall.o) \
	$(LIBRARY)(rtt_getiattr.o) \
	$(LIBRARY)(rtt_getistat.o) \
	$(LIBRARY)(rtt_getiint.o) \
	$(LIBRARY)(rtt_setiattr.o) \
	$(LIBRARY)(rtt_getsstat.o) \
	$(LIBRARY)(rtt_getsval.o) \
	$(LIBRARY)(rtt_setsval.o) \
	$(LIBRARY)(rtt_restart.o) \
	$(LIBRARY)(rtt_getpattr.o) \
	$(LIBRARY)(rtt_setpattr.o) \
	$(LIBRARY)(rtt_getpstat.o) \
	$(LIBRARY)(rtt_getpint.o) \
	$(LIBRARY)(rtt_clrpstat.o) \
	$(LIBRARY)(rtt_rtstart.o) \
	$(LIBRARY)(rtt_rtstop.o) \
	$(LIBRARY)(rtt_semalloc.o) \
	$(LIBRARY)(rtt_semfree.o) \
	$(LIBRARY)(rtt_getsemst.o) \
	$(LIBRARY)(rtt_getsemin.o) \
	$(LIBRARY)(rtt_getsemid.o) \
	$(LIBRARY)(rtt_getsemat.o) \
	$(LIBRARY)(rtt_exit.o) \
	$(LIBRARY)(rtt_sendsig.o) \
	$(LIBRARY)(rtkrncall.o) \
	$(LIBRARY)(rtt_getmqst.o) \
	$(LIBRARY)(rtt_clrsstat.o) \
	$(LIBRARY)(rtt_clristat.o) \
	$(LIBRARY)(rtt_clrsemst.o) \
	$(LIBRARY)(rtt_clrmqst.o) \

$(LIBRARY):	$(OBJECTS)
	aal cr $@ *.o
	rm *.o

$(LIBRARY)(sys_abort.o):	sys_abort.c
	$(CC1) sys_abort.c

$(LIBRARY)(sys_copy.o):	sys_copy.c
	$(CC1) sys_copy.c

$(LIBRARY)(sys_endsig.o):	sys_endsig.c
	$(CC1) sys_endsig.c

$(LIBRARY)(sys_exec.o):	sys_exec.c
	$(CC1) sys_exec.c

$(LIBRARY)(sys_fork.o):	sys_fork.c
	$(CC1) sys_fork.c

$(LIBRARY)(sys_fresh.o):	sys_fresh.c
	$(CC1) sys_fresh.c

$(LIBRARY)(sys_getmap.o):	sys_getmap.c
	$(CC1) sys_getmap.c

$(LIBRARY)(sys_getsp.o):	sys_getsp.c
	$(CC1) sys_getsp.c

$(LIBRARY)(sys_kill.o):	sys_kill.c
	$(CC1) sys_kill.c

$(LIBRARY)(sys_newmap.o):	sys_newmap.c
	$(CC1) sys_newmap.c

$(LIBRARY)(sys_oldsig.o):	sys_oldsig.c
	$(CC1) sys_oldsig.c

$(LIBRARY)(sys_sendsig.o):	sys_sendsig.c
	$(CC1) sys_sendsig.c

$(LIBRARY)(sys_sigret.o):	sys_sigret.c
	$(CC1) sys_sigret.c

$(LIBRARY)(sys_times.o):	sys_times.c
	$(CC1) sys_times.c

$(LIBRARY)(sys_trace.o):	sys_trace.c
	$(CC1) sys_trace.c

$(LIBRARY)(sys_xit.o):	sys_xit.c
	$(CC1) sys_xit.c

$(LIBRARY)(taskcall.o):	taskcall.c
	$(CC1) taskcall.c

$(LIBRARY)(rtt_getiattr.o):	rtt_getiattr.c
	$(CC1) rtt_getiattr.c

$(LIBRARY)(rtt_getistat.o):	rtt_getistat.c
	$(CC1) rtt_getistat.c

$(LIBRARY)(rtt_getiint.o):	rtt_getiint.c
	$(CC1) rtt_getiint.c

$(LIBRARY)(rtt_setiattr.o):	rtt_setiattr.c
	$(CC1) rtt_setiattr.c

$(LIBRARY)(rtt_getsstat.o):	rtt_getsstat.c
	$(CC1) rtt_getsstat.c

$(LIBRARY)(rtt_getsval.o):	rtt_getsval.c
	$(CC1) rtt_getsval.c

$(LIBRARY)(rtt_setsval.o):	rtt_setsval.c
	$(CC1) rtt_setsval.c

$(LIBRARY)(rtt_restart.o):	rtt_restart.c
	$(CC1) rtt_restart.c

$(LIBRARY)(rtt_getpattr.o):	rtt_getpattr.c
	$(CC1) rtt_getpattr.c

$(LIBRARY)(rtt_setpattr.o):	rtt_setpattr.c
	$(CC1) rtt_setpattr.c

$(LIBRARY)(rtt_getpstat.o):	rtt_getpstat.c
	$(CC1) rtt_getpstat.c

$(LIBRARY)(rtt_getpint.o):	rtt_getpint.c
	$(CC1) rtt_getpint.c

$(LIBRARY)(rtt_clrpstat.o):	rtt_clrpstat.c
	$(CC1) rtt_clrpstat.c

$(LIBRARY)(rtt_rtstart.o):	rtt_rtstart.c
	$(CC1) rtt_rtstart.c

$(LIBRARY)(rtt_rtstop.o):	rtt_rtstop.c
	$(CC1) rtt_rtstop.c

$(LIBRARY)(rtt_exit.o):		rtt_exit.c
	$(CC1) rtt_exit.c

$(LIBRARY)(rtt_sendsig.o):	rtt_sendsig.c
	$(CC1) rtt_sendsig.c

$(LIBRARY)(rtkrncall.o):	rtkrncall.c
	$(CC1) rtkrncall.c

$(LIBRARY)(rtt_semalloc.o):	rtt_semalloc.c
	$(CC1) rtt_semalloc.c

$(LIBRARY)(rtt_semfree.o):	rtt_semfree.c
	$(CC1) rtt_semfree.c

$(LIBRARY)(rtt_getsemst.o):	rtt_getsemst.c
	$(CC1) rtt_getsemst.c

$(LIBRARY)(rtt_getsemin.o):	rtt_getsemin.c
	$(CC1) rtt_getsemin.c

$(LIBRARY)(rtt_getsemid.o):	rtt_getsemid.c
	$(CC1) rtt_getsemid.c

$(LIBRARY)(rtt_getsemat.o):	rtt_getsemat.c
	$(CC1) rtt_getsemat.c

$(LIBRARY)(rtt_getmqst.o):	rtt_getmqst.c
	$(CC1) rtt_getmqst.c

$(LIBRARY)(rtt_clrsstat.o):	rtt_clrsstat.c
	$(CC1) rtt_clrsstat.c

$(LIBRARY)(rtt_clristat.o):	rtt_clristat.c
	$(CC1) rtt_clristat.c

$(LIBRARY)(rtt_clrsemst.o):	rtt_clrsemst.c
	$(CC1) rtt_clrsemst.c

$(LIBRARY)(rtt_clrmqst.o):	rtt_clrmqst.c
	$(CC1) rtt_clrmqst.c

