// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROCESS_H
#define RUDIMENTS_PROCESS_H

#include <rudiments/private/process.h>

// wrap:
//	unistd.h - nice()
//			getpid(),getppid(),getpgid()/getpgrp()
//			setpgid()/setpgrp()
//			getsid()
//			setsid()
//			getuid(),geteuid(),getgid(),getegid()
//			getgroups()
//			setuid(),setreuid(),seteuid()
//			setgid(),setregid(),setegid()
//			fork(),vfork()
//			profil()
//			acct()
//			getdtablesize()
//			brk()/sbrk()
//			syscall()
//	sched.h - functions for manipulating the scheduler
//			sched_setparam(),sched_getparam()
//			sched_setscheduler(),sched_getscheduler()
//			sched_yield(),
//			sched_get_priority_max(),sched_get_priority_min()
//			sched_rr_get_interval()
//			sched_setaffinity(),sched_getaffinity()
//	sys/acct.h - acct() - process accounting on/off
//	sys/ptrace.h - ptrace()
//	sys/resource.h - get/set process priority/resource usage
//				getrlimit(),setrlimit()
//				getrusage()
//				getpriority(),setpriority()
//	sys/stat.h - umask()
//	sys/times.h - times() - function for getting process times
//	not in solaris -
//		unistd.h - daemon()
//	not in many systems - 
//		unistd.h - group_member()
//				getresuid(),getresgid()
//				setresuid(),setresgid()
//		sys/gmon.h and sys/gmon_out.h - monstartup()
//	linux only - 
//		execinfo.h - backtrace(),backtrace_symbols(),
//				backtrace_symbols_fd()
//		sys/prctl.h - prctl()
//	not in bsd - 
//		ucontext.h - user-level context switching
//			getcontext(),setcontext(),swapcontext(),makecontext()


class process {
	public:

	#include <rudiments/private/process.h>
};

#endif
