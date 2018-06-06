# Makefile for lib/syscall.

CC1	= $(CC) -c

LIBRARY	= ../libc.a
all:	$(LIBRARY)

OBJECTS	= \
	$(LIBRARY)(_exit.o) \
	$(LIBRARY)(access.o) \
	$(LIBRARY)(alarm.o) \
	$(LIBRARY)(brk.o) \
	$(LIBRARY)(cfgetispeed.o) \
	$(LIBRARY)(cfgetospeed.o) \
	$(LIBRARY)(cfsetispeed.o) \
	$(LIBRARY)(cfsetospeed.o) \
	$(LIBRARY)(chdir.o) \
	$(LIBRARY)(chmod.o) \
	$(LIBRARY)(chown.o) \
	$(LIBRARY)(chroot.o) \
	$(LIBRARY)(close.o) \
	$(LIBRARY)(closedir.o) \
	$(LIBRARY)(creat.o) \
	$(LIBRARY)(dup.o) \
	$(LIBRARY)(dup2.o) \
	$(LIBRARY)(execl.o) \
	$(LIBRARY)(execle.o) \
	$(LIBRARY)(execv.o) \
	$(LIBRARY)(execve.o) \
	$(LIBRARY)(fcntl.o) \
	$(LIBRARY)(fork.o) \
	$(LIBRARY)(fpathconf.o) \
	$(LIBRARY)(fstat.o) \
	$(LIBRARY)(getcwd.o) \
	$(LIBRARY)(getegid.o) \
	$(LIBRARY)(geteuid.o) \
	$(LIBRARY)(getgid.o) \
	$(LIBRARY)(getgroups.o) \
	$(LIBRARY)(getpgrp.o) \
	$(LIBRARY)(getpid.o) \
	$(LIBRARY)(getppid.o) \
	$(LIBRARY)(getuid.o) \
	$(LIBRARY)(ioctl.o) \
	$(LIBRARY)(isatty.o) \
	$(LIBRARY)(kill.o) \
	$(LIBRARY)(link.o) \
	$(LIBRARY)(lseek.o) \
	$(LIBRARY)(mkdir.o) \
	$(LIBRARY)(mkfifo.o) \
	$(LIBRARY)(mknod.o) \
	$(LIBRARY)(mktemp.o) \
	$(LIBRARY)(mount.o) \
	$(LIBRARY)(open.o) \
	$(LIBRARY)(opendir.o) \
	$(LIBRARY)(pathconf.o) \
	$(LIBRARY)(pause.o) \
	$(LIBRARY)(pipe.o) \
	$(LIBRARY)(ptrace.o) \
	$(LIBRARY)(read.o) \
	$(LIBRARY)(readdir.o) \
	$(LIBRARY)(reboot.o) \
	$(LIBRARY)(rename.o) \
	$(LIBRARY)(rewinddir.o) \
	$(LIBRARY)(rmdir.o) \
	$(LIBRARY)(sbrk.o) \
	$(LIBRARY)(seekdir.o) \
	$(LIBRARY)(setgid.o) \
	$(LIBRARY)(setsid.o) \
	$(LIBRARY)(setuid.o) \
	$(LIBRARY)(sigaction.o) \
	$(LIBRARY)(sigaddset.o) \
	$(LIBRARY)(sigdelset.o) \
	$(LIBRARY)(sigemptyset.o) \
	$(LIBRARY)(sigfillset.o) \
	$(LIBRARY)(sigismember.o) \
	$(LIBRARY)(sigpending.o) \
	$(LIBRARY)(sigprocmask.o) \
	$(LIBRARY)(sigreturn.o) \
	$(LIBRARY)(sigsuspend.o) \
	$(LIBRARY)(sleep.o) \
	$(LIBRARY)(stat.o) \
	$(LIBRARY)(stime.o) \
	$(LIBRARY)(sync.o) \
	$(LIBRARY)(tcdrain.o) \
	$(LIBRARY)(tcflow.o) \
	$(LIBRARY)(tcflush.o) \
	$(LIBRARY)(tcgetattr.o) \
	$(LIBRARY)(tcsendbreak.o) \
	$(LIBRARY)(tcsetattr.o) \
	$(LIBRARY)(time.o) \
	$(LIBRARY)(times.o) \
	$(LIBRARY)(umask.o) \
	$(LIBRARY)(umount.o) \
	$(LIBRARY)(uname.o) \
	$(LIBRARY)(unlink.o) \
	$(LIBRARY)(utime.o) \
	$(LIBRARY)(wait.o) \
	$(LIBRARY)(waitpid.o) \
	$(LIBRARY)(write.o) \
	$(LIBRARY)(mrtgetiattr.o) \
	$(LIBRARY)(mrtgetistat.o) \
	$(LIBRARY)(mrtgetiint.o) \
	$(LIBRARY)(mrtsetiattr.o) \
	$(LIBRARY)(mrtgetsstat.o) \
	$(LIBRARY)(mrtgetsval.o) \
	$(LIBRARY)(mrtsetsval.o) \
	$(LIBRARY)(mrtrestart.o) \
	$(LIBRARY)(mrtgetpattr.o) \
	$(LIBRARY)(mrtsetpattr.o) \
	$(LIBRARY)(mrtgetpstat.o) \
	$(LIBRARY)(mrtgetpint.o) \
	$(LIBRARY)(mrtclrpstat.o) \
	$(LIBRARY)(mrtsleep.o) \
	$(LIBRARY)(mrtwakeup.o) \
	$(LIBRARY)(mrtRTstart.o) \
	$(LIBRARY)(mrtRTstop.o) \
	$(LIBRARY)(mrtset2rt.o) \
	$(LIBRARY)(mrtset2nrt.o) \
	$(LIBRARY)(mrtrqst.o) \
	$(LIBRARY)(mrtarqst.o) \
	$(LIBRARY)(mrtreply.o) \
	$(LIBRARY)(mrtrcv.o) \
	$(LIBRARY)(mrtprint.o) \
	$(LIBRARY)(mrtuprqst.o) \
	$(LIBRARY)(mrtsign.o) \
	$(LIBRARY)(mrtrqrcv.o) \
	$(LIBRARY)(mrtsemalloc.o) \
	$(LIBRARY)(mrtsemfree.o) \
	$(LIBRARY)(mrtgetsemst.o) \
	$(LIBRARY)(mrtgetsemin.o) \
	$(LIBRARY)(mrtgetsemid.o) \
	$(LIBRARY)(mrtgetsemat.o) \
	$(LIBRARY)(mrtsemup.o) \
	$(LIBRARY)(mrtsemdown.o) \
	$(LIBRARY)(mrtgetmqst.o) \
	$(LIBRARY)(mrtclrsstat.o) \
	$(LIBRARY)(mrtclristat.o) \
	$(LIBRARY)(mrtclrsemst.o) \
	$(LIBRARY)(mrtclrmqst.o) \

$(LIBRARY):	$(OBJECTS)
	aal cr $@ *.o
	rm *.o

$(LIBRARY)(_exit.o):	_exit.s
	$(CC1) _exit.s

$(LIBRARY)(access.o):	access.s
	$(CC1) access.s

$(LIBRARY)(alarm.o):	alarm.s
	$(CC1) alarm.s

$(LIBRARY)(brk.o):	brk.s
	$(CC1) brk.s

$(LIBRARY)(cfgetispeed.o):	cfgetispeed.s
	$(CC1) cfgetispeed.s

$(LIBRARY)(cfgetospeed.o):	cfgetospeed.s
	$(CC1) cfgetospeed.s

$(LIBRARY)(cfsetispeed.o):	cfsetispeed.s
	$(CC1) cfsetispeed.s

$(LIBRARY)(cfsetospeed.o):	cfsetospeed.s
	$(CC1) cfsetospeed.s

$(LIBRARY)(chdir.o):	chdir.s
	$(CC1) chdir.s

$(LIBRARY)(chmod.o):	chmod.s
	$(CC1) chmod.s

$(LIBRARY)(chown.o):	chown.s
	$(CC1) chown.s

$(LIBRARY)(chroot.o):	chroot.s
	$(CC1) chroot.s

$(LIBRARY)(close.o):	close.s
	$(CC1) close.s

$(LIBRARY)(closedir.o):	closedir.s
	$(CC1) closedir.s

$(LIBRARY)(creat.o):	creat.s
	$(CC1) creat.s

$(LIBRARY)(dup.o):	dup.s
	$(CC1) dup.s

$(LIBRARY)(dup2.o):	dup2.s
	$(CC1) dup2.s

$(LIBRARY)(execl.o):	execl.s
	$(CC1) execl.s

$(LIBRARY)(execle.o):	execle.s
	$(CC1) execle.s

$(LIBRARY)(execv.o):	execv.s
	$(CC1) execv.s

$(LIBRARY)(execve.o):	execve.s
	$(CC1) execve.s

$(LIBRARY)(fcntl.o):	fcntl.s
	$(CC1) fcntl.s

$(LIBRARY)(fork.o):	fork.s
	$(CC1) fork.s

$(LIBRARY)(fpathconf.o):	fpathconf.s
	$(CC1) fpathconf.s

$(LIBRARY)(fstat.o):	fstat.s
	$(CC1) fstat.s

$(LIBRARY)(getcwd.o):	getcwd.s
	$(CC1) getcwd.s

$(LIBRARY)(getegid.o):	getegid.s
	$(CC1) getegid.s

$(LIBRARY)(geteuid.o):	geteuid.s
	$(CC1) geteuid.s

$(LIBRARY)(getgid.o):	getgid.s
	$(CC1) getgid.s

$(LIBRARY)(getgroups.o):	getgroups.s
	$(CC1) getgroups.s

$(LIBRARY)(getpgrp.o):	getpgrp.s
	$(CC1) getpgrp.s

$(LIBRARY)(getpid.o):	getpid.s
	$(CC1) getpid.s

$(LIBRARY)(getppid.o):	getppid.s
	$(CC1) getppid.s

$(LIBRARY)(getuid.o):	getuid.s
	$(CC1) getuid.s

$(LIBRARY)(ioctl.o):	ioctl.s
	$(CC1) ioctl.s

$(LIBRARY)(isatty.o):	isatty.s
	$(CC1) isatty.s

$(LIBRARY)(kill.o):	kill.s
	$(CC1) kill.s

$(LIBRARY)(link.o):	link.s
	$(CC1) link.s

$(LIBRARY)(lseek.o):	lseek.s
	$(CC1) lseek.s

$(LIBRARY)(mkdir.o):	mkdir.s
	$(CC1) mkdir.s

$(LIBRARY)(mkfifo.o):	mkfifo.s
	$(CC1) mkfifo.s

$(LIBRARY)(mknod.o):	mknod.s
	$(CC1) mknod.s

$(LIBRARY)(mktemp.o):	mktemp.s
	$(CC1) mktemp.s

$(LIBRARY)(mount.o):	mount.s
	$(CC1) mount.s

$(LIBRARY)(open.o):	open.s
	$(CC1) open.s

$(LIBRARY)(opendir.o):	opendir.s
	$(CC1) opendir.s

$(LIBRARY)(pathconf.o):	pathconf.s
	$(CC1) pathconf.s

$(LIBRARY)(pause.o):	pause.s
	$(CC1) pause.s

$(LIBRARY)(pipe.o):	pipe.s
	$(CC1) pipe.s

$(LIBRARY)(ptrace.o):	ptrace.s
	$(CC1) ptrace.s

$(LIBRARY)(read.o):	read.s
	$(CC1) read.s

$(LIBRARY)(readdir.o):	readdir.s
	$(CC1) readdir.s

$(LIBRARY)(reboot.o):	reboot.s
	$(CC1) reboot.s

$(LIBRARY)(rename.o):	rename.s
	$(CC1) rename.s

$(LIBRARY)(rewinddir.o):	rewinddir.s
	$(CC1) rewinddir.s

$(LIBRARY)(rmdir.o):	rmdir.s
	$(CC1) rmdir.s

$(LIBRARY)(sbrk.o):	sbrk.s
	$(CC1) sbrk.s

$(LIBRARY)(seekdir.o):	seekdir.s
	$(CC1) seekdir.s

$(LIBRARY)(setgid.o):	setgid.s
	$(CC1) setgid.s

$(LIBRARY)(setsid.o):	setsid.s
	$(CC1) setsid.s

$(LIBRARY)(setuid.o):	setuid.s
	$(CC1) setuid.s

$(LIBRARY)(sigaction.o):	sigaction.s
	$(CC1) sigaction.s

$(LIBRARY)(sigaddset.o):	sigaddset.s
	$(CC1) sigaddset.s

$(LIBRARY)(sigdelset.o):	sigdelset.s
	$(CC1) sigdelset.s

$(LIBRARY)(sigemptyset.o):	sigemptyset.s
	$(CC1) sigemptyset.s

$(LIBRARY)(sigfillset.o):	sigfillset.s
	$(CC1) sigfillset.s

$(LIBRARY)(sigismember.o):	sigismember.s
	$(CC1) sigismember.s

$(LIBRARY)(sigpending.o):	sigpending.s
	$(CC1) sigpending.s

$(LIBRARY)(sigprocmask.o):	sigprocmask.s
	$(CC1) sigprocmask.s

$(LIBRARY)(sigreturn.o):	sigreturn.s
	$(CC1) sigreturn.s

$(LIBRARY)(sigsuspend.o):	sigsuspend.s
	$(CC1) sigsuspend.s

$(LIBRARY)(sleep.o):	sleep.s
	$(CC1) sleep.s

$(LIBRARY)(stat.o):	stat.s
	$(CC1) stat.s

$(LIBRARY)(stime.o):	stime.s
	$(CC1) stime.s

$(LIBRARY)(sync.o):	sync.s
	$(CC1) sync.s

$(LIBRARY)(tcdrain.o):	tcdrain.s
	$(CC1) tcdrain.s

$(LIBRARY)(tcflow.o):	tcflow.s
	$(CC1) tcflow.s

$(LIBRARY)(tcflush.o):	tcflush.s
	$(CC1) tcflush.s

$(LIBRARY)(tcgetattr.o):	tcgetattr.s
	$(CC1) tcgetattr.s

$(LIBRARY)(tcsendbreak.o):	tcsendbreak.s
	$(CC1) tcsendbreak.s

$(LIBRARY)(tcsetattr.o):	tcsetattr.s
	$(CC1) tcsetattr.s

$(LIBRARY)(time.o):	time.s
	$(CC1) time.s

$(LIBRARY)(times.o):	times.s
	$(CC1) times.s

$(LIBRARY)(umask.o):	umask.s
	$(CC1) umask.s

$(LIBRARY)(umount.o):	umount.s
	$(CC1) umount.s

$(LIBRARY)(uname.o):	uname.s
	$(CC1) uname.s

$(LIBRARY)(unlink.o):	unlink.s
	$(CC1) unlink.s

$(LIBRARY)(utime.o):	utime.s
	$(CC1) utime.s

$(LIBRARY)(wait.o):	wait.s
	$(CC1) wait.s

$(LIBRARY)(waitpid.o):	waitpid.s
	$(CC1) waitpid.s

$(LIBRARY)(write.o):	write.s
	$(CC1) write.s

$(LIBRARY)(mrtgetiattr.o):	mrtgetiattr.s
	$(CC1) mrtgetiattr.s

$(LIBRARY)(mrtgetistat.o):	mrtgetistat.s
	$(CC1) mrtgetistat.s

$(LIBRARY)(mrtgetiint.o):	mrtgetiint.s
	$(CC1) mrtgetiint.s

$(LIBRARY)(mrtsetiattr.o):	mrtsetiattr.s
	$(CC1) mrtsetiattr.s

$(LIBRARY)(mrtgetsstat.o):	mrtgetsstat.s
	$(CC1) mrtgetsstat.s

$(LIBRARY)(mrtgetsval.o):	mrtgetsval.s
	$(CC1) mrtgetsval.s

$(LIBRARY)(mrtsetsval.o):	mrtsetsval.s
	$(CC1) mrtsetsval.s

$(LIBRARY)(mrtrestart.o):	mrtrestart.s
	$(CC1) mrtrestart.s

$(LIBRARY)(mrtgetpattr.o):	mrtgetpattr.s
	$(CC1) mrtgetpattr.s

$(LIBRARY)(mrtsetpattr.o):	mrtsetpattr.s
	$(CC1) mrtsetpattr.s

$(LIBRARY)(mrtgetpstat.o):	mrtgetpstat.s
	$(CC1) mrtgetpstat.s

$(LIBRARY)(mrtgetpint.o):	mrtgetpint.s
	$(CC1) mrtgetpint.s

$(LIBRARY)(mrtclrpstat.o):	mrtclrpstat.s
	$(CC1) mrtclrpstat.s

$(LIBRARY)(mrtsleep.o):		mrtsleep.s
	$(CC1) mrtsleep.s

$(LIBRARY)(mrtwakeup.o):	mrtwakeup.s
	$(CC1) mrtwakeup.s

$(LIBRARY)(mrtRTstart.o):	mrtRTstart.s
	$(CC1) mrtRTstart.s

$(LIBRARY)(mrtRTstop.o):	mrtRTstop.s
	$(CC1) mrtRTstop.s

$(LIBRARY)(mrtset2rt.o):	mrtset2rt.s
	$(CC1) mrtset2rt.s

$(LIBRARY)(mrtset2nrt.o):	mrtset2nrt.s
	$(CC1) mrtset2nrt.s

$(LIBRARY)(mrtrqst.o):	mrtrqst.s
	$(CC1) mrtrqst.s

$(LIBRARY)(mrtarqst.o):	mrtarqst.s
	$(CC1) mrtarqst.s

$(LIBRARY)(mrtreply.o):	mrtreply.s
	$(CC1) mrtreply.s

$(LIBRARY)(mrtrcv.o):	mrtrcv.s
	$(CC1) mrtrcv.s

$(LIBRARY)(mrtprint.o):	mrtprint.s
	$(CC1) mrtprint.s

$(LIBRARY)(mrtuprqst.o): mrtuprqst.s
	$(CC1) mrtuprqst.s

$(LIBRARY)(mrtsign.o):	mrtsign.s
	$(CC1) mrtsign.s

$(LIBRARY)(mrtrqrcv.o):	mrtrqrcv.s
	$(CC1) mrtrqrcv.s

$(LIBRARY)(mrtsemalloc.o):	mrtsemalloc.s
	$(CC1) mrtsemalloc.s

$(LIBRARY)(mrtsemfree.o):	mrtsemfree.s
	$(CC1) mrtsemfree.s

$(LIBRARY)(mrtgetsemst.o):	mrtgetsemst.s
	$(CC1) mrtgetsemst.s

$(LIBRARY)(mrtgetsemin.o):	mrtgetsemin.s
	$(CC1) mrtgetsemin.s

$(LIBRARY)(mrtgetsemid.o):	mrtgetsemid.s
	$(CC1) mrtgetsemid.s

$(LIBRARY)(mrtgetsemat.o):	mrtgetsemat.s
	$(CC1) mrtgetsemat.s

$(LIBRARY)(mrtsemup.o):	mrtsemup.s
	$(CC1) mrtsemup.s

$(LIBRARY)(mrtsemdown.o):	mrtsemdown.s
	$(CC1) mrtsemdown.s

$(LIBRARY)(mrtgetmqst.o):	mrtgetmqst.s
	$(CC1) mrtgetmqst.s

$(LIBRARY)(mrtclrsstat.o):	mrtclrsstat.s
	$(CC1) mrtclrsstat.s

$(LIBRARY)(mrtclristat.o):	mrtclristat.s
	$(CC1) mrtclristat.s

$(LIBRARY)(mrtclrsemst.o):	mrtclrsemst.s
	$(CC1) mrtclrsemst.s

$(LIBRARY)(mrtclrmqst.o):	mrtclrmqst.s
	$(CC1) mrtclrmqst.s

