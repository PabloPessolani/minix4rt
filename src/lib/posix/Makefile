# Makefile for lib/posix.

CFLAGS	= -O -D_MINIX -D_POSIX_SOURCE
CC1	= $(CC) $(CFLAGS) -c

LIBRARY	= ../libc.a
all:	$(LIBRARY)

OBJECTS	= \
	$(LIBRARY)(__exit.o) \
	$(LIBRARY)(_access.o) \
	$(LIBRARY)(_alarm.o) \
	$(LIBRARY)(_cfgetispeed.o) \
	$(LIBRARY)(_cfgetospeed.o) \
	$(LIBRARY)(_cfsetispeed.o) \
	$(LIBRARY)(_cfsetospeed.o) \
	$(LIBRARY)(_chdir.o) \
	$(LIBRARY)(_chmod.o) \
	$(LIBRARY)(_chown.o) \
	$(LIBRARY)(_chroot.o) \
	$(LIBRARY)(_close.o) \
	$(LIBRARY)(_closedir.o) \
	$(LIBRARY)(_creat.o) \
	$(LIBRARY)(_dup.o) \
	$(LIBRARY)(_dup2.o) \
	$(LIBRARY)(_exec.o) \
	$(LIBRARY)(_execn.o) \
	$(LIBRARY)(_fcntl.o) \
	$(LIBRARY)(_fork.o) \
	$(LIBRARY)(_fpathconf.o) \
	$(LIBRARY)(_fstat.o) \
	$(LIBRARY)(_getcwd.o) \
	$(LIBRARY)(_getegid.o) \
	$(LIBRARY)(_geteuid.o) \
	$(LIBRARY)(_getgid.o) \
	$(LIBRARY)(_getgroups.o) \
	$(LIBRARY)(_getpgrp.o) \
	$(LIBRARY)(_getpid.o) \
	$(LIBRARY)(_getppid.o) \
	$(LIBRARY)(_getuid.o) \
	$(LIBRARY)(_ioctl.o) \
	$(LIBRARY)(_isatty.o) \
	$(LIBRARY)(_kill.o) \
	$(LIBRARY)(_link.o) \
	$(LIBRARY)(_lseek.o) \
	$(LIBRARY)(_mkdir.o) \
	$(LIBRARY)(_mkfifo.o) \
	$(LIBRARY)(_mknod.o) \
	$(LIBRARY)(_mktemp.o) \
	$(LIBRARY)(_mount.o) \
	$(LIBRARY)(_open.o) \
	$(LIBRARY)(_opendir.o) \
	$(LIBRARY)(_pathconf.o) \
	$(LIBRARY)(_pause.o) \
	$(LIBRARY)(_pipe.o) \
	$(LIBRARY)(_ptrace.o) \
	$(LIBRARY)(_read.o) \
	$(LIBRARY)(_readdir.o) \
	$(LIBRARY)(_rename.o) \
	$(LIBRARY)(_rewinddir.o) \
	$(LIBRARY)(_rmdir.o) \
	$(LIBRARY)(_setgid.o) \
	$(LIBRARY)(_setsid.o) \
	$(LIBRARY)(_setuid.o) \
	$(LIBRARY)(_sigaction.o) \
	$(LIBRARY)(_sigpending.o) \
	$(LIBRARY)(_sigprocmask.o) \
	$(LIBRARY)(_sigreturn.o) \
	$(LIBRARY)(_sigset.o) \
	$(LIBRARY)(_sigsetjmp.o) \
	$(LIBRARY)(_sigsuspend.o) \
	$(LIBRARY)(_sleep.o) \
	$(LIBRARY)(_stat.o) \
	$(LIBRARY)(_stime.o) \
	$(LIBRARY)(_sync.o) \
	$(LIBRARY)(_tcdrain.o) \
	$(LIBRARY)(_tcflow.o) \
	$(LIBRARY)(_tcflush.o) \
	$(LIBRARY)(_tcgetattr.o) \
	$(LIBRARY)(_tcsendbreak.o) \
	$(LIBRARY)(_tcsetattr.o) \
	$(LIBRARY)(_time.o) \
	$(LIBRARY)(_times.o) \
	$(LIBRARY)(_umask.o) \
	$(LIBRARY)(_umount.o) \
	$(LIBRARY)(_uname.o) \
	$(LIBRARY)(_unlink.o) \
	$(LIBRARY)(_utime.o) \
	$(LIBRARY)(_wait.o) \
	$(LIBRARY)(_waitpid.o) \
	$(LIBRARY)(_write.o) \
	$(LIBRARY)(_mrtgetiattr.o) \
	$(LIBRARY)(_mrtgetistat.o) \
	$(LIBRARY)(_mrtgetiint.o) \
	$(LIBRARY)(_mrtsetiattr.o) \
	$(LIBRARY)(_mrtgetsstat.o) \
	$(LIBRARY)(_mrtgetsval.o) \
	$(LIBRARY)(_mrtsetsval.o) \
	$(LIBRARY)(_mrtrestart.o) \
	$(LIBRARY)(_mrtgetpattr.o) \
	$(LIBRARY)(_mrtsetpattr.o) \
	$(LIBRARY)(_mrtgetpstat.o) \
	$(LIBRARY)(_mrtgetpint.o) \
	$(LIBRARY)(_mrtclrpstat.o) \
	$(LIBRARY)(_mrtRTstart.o) \
	$(LIBRARY)(_mrtRTstop.o) \
	$(LIBRARY)(_mrtsleep.o) \
	$(LIBRARY)(_mrtwakeup.o) \
	$(LIBRARY)(_mrtset2rt.o) \
	$(LIBRARY)(_mrtset2nrt.o) \
	$(LIBRARY)(_mrtrqst.o) \
	$(LIBRARY)(_mrtarqst.o) \
	$(LIBRARY)(_mrtreply.o) \
	$(LIBRARY)(_mrtrcv.o) \
	$(LIBRARY)(_mrtprint.o) \
	$(LIBRARY)(_mrtuprqst.o) \
	$(LIBRARY)(_mrtsign.o) \
	$(LIBRARY)(_mrtrqrcv.o) \
	$(LIBRARY)(_mrtsemalloc.o) \
	$(LIBRARY)(_mrtsemfree.o) \
	$(LIBRARY)(_mrtgetsemst.o) \
	$(LIBRARY)(_mrtgetsemin.o) \
	$(LIBRARY)(_mrtgetsemid.o) \
	$(LIBRARY)(_mrtgetsemat.o) \
	$(LIBRARY)(_mrtsemup.o) \
	$(LIBRARY)(_mrtsemdown.o) \
	$(LIBRARY)(_mrtgetmqst.o) \
	$(LIBRARY)(_mrtclrsstat.o) \
	$(LIBRARY)(_mrtclristat.o) \
	$(LIBRARY)(_mrtclrsemst.o) \
	$(LIBRARY)(_mrtclrmqst.o) \


$(LIBRARY):	$(OBJECTS)
	aal cr $@ *.o
	rm *.o

$(LIBRARY)(__exit.o):	__exit.c
	$(CC1) __exit.c

$(LIBRARY)(_access.o):	_access.c
	$(CC1) _access.c

$(LIBRARY)(_alarm.o):	_alarm.c
	$(CC1) _alarm.c

$(LIBRARY)(_cfgetispeed.o):	_cfgetispeed.c
	$(CC1) _cfgetispeed.c

$(LIBRARY)(_cfgetospeed.o):	_cfgetospeed.c
	$(CC1) _cfgetospeed.c

$(LIBRARY)(_cfsetispeed.o):	_cfsetispeed.c
	$(CC1) _cfsetispeed.c

$(LIBRARY)(_cfsetospeed.o):	_cfsetospeed.c
	$(CC1) _cfsetospeed.c

$(LIBRARY)(_chdir.o):	_chdir.c
	$(CC1) _chdir.c

$(LIBRARY)(_chmod.o):	_chmod.c
	$(CC1) _chmod.c

$(LIBRARY)(_chown.o):	_chown.c
	$(CC1) _chown.c

$(LIBRARY)(_chroot.o):	_chroot.c
	$(CC1) _chroot.c

$(LIBRARY)(_close.o):	_close.c
	$(CC1) _close.c

$(LIBRARY)(_closedir.o):	_closedir.c
	$(CC1) _closedir.c

$(LIBRARY)(_creat.o):	_creat.c
	$(CC1) _creat.c

$(LIBRARY)(_dup.o):	_dup.c
	$(CC1) _dup.c

$(LIBRARY)(_dup2.o):	_dup2.c
	$(CC1) _dup2.c

$(LIBRARY)(_exec.o):	_exec.c
	$(CC1) _exec.c

$(LIBRARY)(_execn.o):	_execn.c
	$(CC1) _execn.c

$(LIBRARY)(_fcntl.o):	_fcntl.c
	$(CC1) _fcntl.c

$(LIBRARY)(_fork.o):	_fork.c
	$(CC1) _fork.c

$(LIBRARY)(_fpathconf.o):	_fpathconf.c
	$(CC1) _fpathconf.c

$(LIBRARY)(_fstat.o):	_fstat.c
	$(CC1) _fstat.c

$(LIBRARY)(_getcwd.o):	_getcwd.c
	$(CC1) _getcwd.c

$(LIBRARY)(_getegid.o):	_getegid.c
	$(CC1) _getegid.c

$(LIBRARY)(_geteuid.o):	_geteuid.c
	$(CC1) _geteuid.c

$(LIBRARY)(_getgid.o):	_getgid.c
	$(CC1) _getgid.c

$(LIBRARY)(_getgroups.o):	_getgroups.c
	$(CC1) _getgroups.c

$(LIBRARY)(_getpgrp.o):	_getpgrp.c
	$(CC1) _getpgrp.c

$(LIBRARY)(_getpid.o):	_getpid.c
	$(CC1) _getpid.c

$(LIBRARY)(_getppid.o):	_getppid.c
	$(CC1) _getppid.c

$(LIBRARY)(_getuid.o):	_getuid.c
	$(CC1) _getuid.c

$(LIBRARY)(_ioctl.o):	_ioctl.c
	$(CC1) _ioctl.c

$(LIBRARY)(_isatty.o):	_isatty.c
	$(CC1) _isatty.c

$(LIBRARY)(_kill.o):	_kill.c
	$(CC1) _kill.c

$(LIBRARY)(_link.o):	_link.c
	$(CC1) _link.c

$(LIBRARY)(_lseek.o):	_lseek.c
	$(CC1) _lseek.c

$(LIBRARY)(_mkdir.o):	_mkdir.c
	$(CC1) _mkdir.c

$(LIBRARY)(_mkfifo.o):	_mkfifo.c
	$(CC1) _mkfifo.c

$(LIBRARY)(_mknod.o):	_mknod.c
	$(CC1) _mknod.c

$(LIBRARY)(_mktemp.o):	_mktemp.c
	$(CC1) _mktemp.c

$(LIBRARY)(_mount.o):	_mount.c
	$(CC1) _mount.c

$(LIBRARY)(_open.o):	_open.c
	$(CC1) _open.c

$(LIBRARY)(_opendir.o):	_opendir.c
	$(CC1) _opendir.c

$(LIBRARY)(_pathconf.o):	_pathconf.c
	$(CC1) _pathconf.c

$(LIBRARY)(_pause.o):	_pause.c
	$(CC1) _pause.c

$(LIBRARY)(_pipe.o):	_pipe.c
	$(CC1) _pipe.c

$(LIBRARY)(_ptrace.o):	_ptrace.c
	$(CC1) _ptrace.c

$(LIBRARY)(_read.o):	_read.c
	$(CC1) _read.c

$(LIBRARY)(_readdir.o):	_readdir.c
	$(CC1) _readdir.c

$(LIBRARY)(_rename.o):	_rename.c
	$(CC1) _rename.c

$(LIBRARY)(_rewinddir.o):	_rewinddir.c
	$(CC1) _rewinddir.c

$(LIBRARY)(_rmdir.o):	_rmdir.c
	$(CC1) _rmdir.c

$(LIBRARY)(_setgid.o):	_setgid.c
	$(CC1) _setgid.c

$(LIBRARY)(_setsid.o):	_setsid.c
	$(CC1) _setsid.c

$(LIBRARY)(_setuid.o):	_setuid.c
	$(CC1) _setuid.c

$(LIBRARY)(_sigaction.o):	_sigaction.c
	$(CC1) _sigaction.c

$(LIBRARY)(_sigpending.o):	_sigpending.c
	$(CC1) _sigpending.c

$(LIBRARY)(_sigprocmask.o):	_sigprocmask.c
	$(CC1) _sigprocmask.c

$(LIBRARY)(_sigreturn.o):	_sigreturn.c
	$(CC1) _sigreturn.c

$(LIBRARY)(_sigset.o):	_sigset.c
	$(CC1) _sigset.c

$(LIBRARY)(_sigsetjmp.o):	_sigsetjmp.c
	$(CC1) _sigsetjmp.c

$(LIBRARY)(_sigsuspend.o):	_sigsuspend.c
	$(CC1) _sigsuspend.c

$(LIBRARY)(_sleep.o):	_sleep.c
	$(CC1) _sleep.c

$(LIBRARY)(_stat.o):	_stat.c
	$(CC1) _stat.c

$(LIBRARY)(_stime.o):	_stime.c
	$(CC1) _stime.c

$(LIBRARY)(_sync.o):	_sync.c
	$(CC1) _sync.c

$(LIBRARY)(_tcdrain.o):	_tcdrain.c
	$(CC1) _tcdrain.c

$(LIBRARY)(_tcflow.o):	_tcflow.c
	$(CC1) _tcflow.c

$(LIBRARY)(_tcflush.o):	_tcflush.c
	$(CC1) _tcflush.c

$(LIBRARY)(_tcgetattr.o):	_tcgetattr.c
	$(CC1) _tcgetattr.c

$(LIBRARY)(_tcsendbreak.o):	_tcsendbreak.c
	$(CC1) _tcsendbreak.c

$(LIBRARY)(_tcsetattr.o):	_tcsetattr.c
	$(CC1) _tcsetattr.c

$(LIBRARY)(_time.o):	_time.c
	$(CC1) _time.c

$(LIBRARY)(_times.o):	_times.c
	$(CC1) _times.c

$(LIBRARY)(_umask.o):	_umask.c
	$(CC1) _umask.c

$(LIBRARY)(_umount.o):	_umount.c
	$(CC1) _umount.c

$(LIBRARY)(_uname.o):	_uname.c /usr/include/minix/config.h
	$(CC1) _uname.c

$(LIBRARY)(_unlink.o):	_unlink.c
	$(CC1) _unlink.c

$(LIBRARY)(_utime.o):	_utime.c
	$(CC1) _utime.c

$(LIBRARY)(_wait.o):	_wait.c
	$(CC1) _wait.c

$(LIBRARY)(_waitpid.o):	_waitpid.c
	$(CC1) _waitpid.c

$(LIBRARY)(_write.o):	_write.c
	$(CC1) _write.c

$(LIBRARY)(_mrtgetiattr.o):	_mrtgetiattr.c
	$(CC1) _mrtgetiattr.c

$(LIBRARY)(_mrtgetistat.o):	_mrtgetistat.c
	$(CC1) _mrtgetistat.c

$(LIBRARY)(_mrtgetiint.o):	_mrtgetiint.c
	$(CC1) _mrtgetiint.c

$(LIBRARY)(_mrtsetiattr.o):	_mrtsetiattr.c
	$(CC1) _mrtsetiattr.c

$(LIBRARY)(_mrtgetsstat.o):	_mrtgetsstat.c
	$(CC1) _mrtgetsstat.c

$(LIBRARY)(_mrtgetsval.o):	_mrtgetsval.c
	$(CC1) _mrtgetsval.c

$(LIBRARY)(_mrtsetsval.o):	_mrtsetsval.c
	$(CC1) _mrtsetsval.c

$(LIBRARY)(_mrtrestart.o):	_mrtrestart.c
	$(CC1) _mrtrestart.c

$(LIBRARY)(_mrtgetpattr.o):	_mrtgetpattr.c
	$(CC1) _mrtgetpattr.c

$(LIBRARY)(_mrtsetpattr.o):	_mrtsetpattr.c
	$(CC1) _mrtsetpattr.c

$(LIBRARY)(_mrtgetpstat.o):	_mrtgetpstat.c
	$(CC1) _mrtgetpstat.c

$(LIBRARY)(_mrtgetpint.o):	_mrtgetpint.c
	$(CC1) _mrtgetpint.c

$(LIBRARY)(_mrtclrpstat.o):	_mrtclrpstat.c
	$(CC1) _mrtclrpstat.c

$(LIBRARY)(_mrtsleep.o):	_mrtsleep.c
	$(CC1) _mrtsleep.c

$(LIBRARY)(_mrtwakeup.o):	_mrtwakeup.c
	$(CC1) _mrtwakeup.c

$(LIBRARY)(_mrtRTstart.o):	_mrtRTstart.c
	$(CC1) _mrtRTstart.c

$(LIBRARY)(_mrtRTstop.o):	_mrtRTstop.c
	$(CC1) _mrtRTstop.c

$(LIBRARY)(_mrtset2rt.o):	_mrtset2rt.c
	$(CC1) _mrtset2rt.c

$(LIBRARY)(_mrtset2nrt.o):	_mrtset2nrt.c
	$(CC1) _mrtset2nrt.c

$(LIBRARY)(_mrtrqst.o):	_mrtrqst.c
	$(CC1) _mrtrqst.c

$(LIBRARY)(_mrtarqst.o): _mrtarqst.c
	$(CC1) _mrtarqst.c

$(LIBRARY)(_mrtreply.o): _mrtreply.c
	$(CC1) _mrtreply.c

$(LIBRARY)(_mrtrcv.o): _mrtrcv.c
	$(CC1) _mrtrcv.c

$(LIBRARY)(_mrtprint.o): _mrtprint.c
	$(CC1) _mrtprint.c

$(LIBRARY)(_mrtuprqst.o): _mrtuprqst.c
	$(CC1) _mrtuprqst.c

$(LIBRARY)(_mrtsign.o): _mrtsign.c
	$(CC1) _mrtsign.c

$(LIBRARY)(_mrtrqrcv.o): _mrtrqrcv.c
	$(CC1) _mrtrqrcv.c

$(LIBRARY)(_mrtsemalloc.o): _mrtsemalloc.c
	$(CC1) _mrtsemalloc.c

$(LIBRARY)(_mrtsemfree.o): _mrtsemfree.c
	$(CC1) _mrtsemfree.c

$(LIBRARY)(_mrtgetsemst.o): _mrtgetsemst.c
	$(CC1) _mrtgetsemst.c

$(LIBRARY)(_mrtgetsemin.o): _mrtgetsemin.c
	$(CC1) _mrtgetsemin.c

$(LIBRARY)(_mrtgetsemid.o): _mrtgetsemid.c
	$(CC1) _mrtgetsemid.c

$(LIBRARY)(_mrtgetsemat.o): _mrtgetsemat.c
	$(CC1) _mrtgetsemat.c

$(LIBRARY)(_mrtsemup.o): _mrtsemup.c
	$(CC1) _mrtsemup.c

$(LIBRARY)(_mrtsemdown.o): _mrtsemdown.c
	$(CC1) _mrtsemdown.c

$(LIBRARY)(_mrtgetmqst.o): _mrtgetmqst.c
	$(CC1) _mrtgetmqst.c

$(LIBRARY)(_mrtclrsstat.o):	_mrtclrsstat.c
	$(CC1) _mrtclrsstat.c

$(LIBRARY)(_mrtclristat.o):	_mrtclristat.c
	$(CC1) _mrtclristat.c

$(LIBRARY)(_mrtclrsemst.o):	_mrtclrsemst.c
	$(CC1) _mrtclrsemst.c

$(LIBRARY)(_mrtclrmqst.o):	_mrtclrmqst.c
	$(CC1) _mrtclrmqst.c

