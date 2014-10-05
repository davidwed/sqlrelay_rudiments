// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SYS_H
#define RUDIMENTS_SYS_H

#include <rudiments/private/sysincludes.h>

class sysprivate;

/** The sys class provides methods for controlling the state of and
 *  accessing information about the operating system. */

class RUDIMENTS_DLLSPEC sys {
	public:

		/** Returns the name of the operating system.
		 *  Linux, FreeBSD, Windows, Cygwin, etc. */
		static	char	*getOperatingSystemName();

		/** Returns a string representing the operating system release.
		 *  Exactly what is returned varies significantly between
		 *  operating systems. */
		static	char	*getOperatingSystemRelease();

		/** Returns a string representing the operating system version.
		 *  Exactly what is returned varies significantly between
		 *  operating systems. */
		static	char	*getOperatingSystemVersion();

		/** Returns the hardware architecture of the environment that
		 *  the program is running in.  i386, x86_64, ppc, etc. */
		static	char	*getOperatingSystemArchitecture();

		/** Returns the host name of the machine that the program
                 *  is running on, as understood by the kernel.  This may or
                 *  may not necessarily be the name the machine is known by
                 *  on the network. */
		static	char	*getHostName();

		/** Sets the host name of the machine, as understood by the
		 *  kernel. */
		static	bool	setHostName(const char *hostname);

		/** Sets the host name of the machine, as understood by the
		 *  kernel. */
		static	bool	setHostName(const char *hostname,
						uint64_t hostnamelen);

		/** Sets the parameters to the load averages from the past
 		 *  one, five and fifteen minutes.  Returns true on success
 		 *  and false on failure. */
		static	bool	getLoadAverages(double *oneminuteaverage,
						double *fiveminuteaverage,
						double *fifteenminuteaverage);

		/** Causes all pending writes to be committed to storage. */
		static	void	sync();

		/** Halts the operating system.  The behavior of this command
		 *  may vary between operating systems but it generally just
		 *  halts the kernel without attempting to shut processes down
		 *  gracefully or commit pending writes to storage. */
		static	bool	halt();

		/** Shuts down the the machine.  The behavior of this command
		 *  may vary between operating systems but it generally attempts
		 *  to shut processes down gracefully, commits pending writes to
		 *  storage, halts the kernel and powers down the physical
		 *  hardware. */
		static	bool	shutDown();

		/** Shuts down and restarts the machine.  The behavior of this
		 *  command may vary between operating systems.  Particularly,
		 *  many operating sytems execute a graceful shutdown before
		 *  the restart, while others do not. */
		static	bool	reboot();

		/** Returns the maximum length of each command line argument.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxCommandLineArgumentLength();

		/** Returns the maximum number of processes that may run
		 *  concurrently by each user.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxProcessesPerUser();

		/** Returns the maximum length allowed for the host name, not
		 *  including the null terminator.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxHostNameLength();

		/** Returns the maximum length allowed for a login name, not
		 *  including the null terminator.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxLoginNameLength();

		/** Returns the number of clock ticks per second - in
		 *  particular, the number of times per second that the kernel
		 *  interrupts the CPU(s) to perform process accounting and
		 *  other tasks.  Some kernels are "tickless" though, and
		 *  the kernel is only interrupted on-demand.  This method
		 *  returns whatever the kernel reports, though some kernels
		 *  report a value, whether it is valid or not.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getClockTicksPerSecond();

		/** Returns the maximum number of files that a single process
		 *  can have open simultaneously.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxOpenFilesPerProcess();

		/** Returns the page size, in bytes.
		 *  Returns -1 if not supported by the system. */
		static	int32_t	getPageSize();

		/** Returns the maximum number of streams that a single process
 		 *  can have open simultaneously.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxOpenStreamsPerProcess();

		/** Returns the maximum number of symlinks that a pathname
		 *  can resolve to before resolution returns ELOOP.  This is
		 *  useful for detecting symlink loops.
		 *
		 *  For instance, this value is used by cat in the following
		 *  example:
		 *
		 *  $ ln -s file otherfile
		 *  $ ln -s otherfile file
		 *  $ cat file
		 *  cat: file: Too many levels of symbolic links
		 *
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxSymlinkLoops();

		/** Returns the maximum length of a terminal device name,
		 *  including the null terminator.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxTerminalDeviceNameLength();

		/** Returns the maximum length of a timezone name.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxTimezoneNameLength();

		/** Returns the maximum length of a utility's input line length,
		 *  either from standard input or from a file, including the
		 *  trailing newline.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxLineLength();

		/** Returns the number of pages of physical memory, not
		 *  including swap.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getPhysicalPageCount();

		/** Returns the number of pages of physical memory that
		 *  are available.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getAvailablePhysicalPageCount();

		/** Returns the number of processors the system is configured
		 *  to support.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getProcessorCount();

		/** Returns the maximum number of processors the system can
		 *  possibly be configured to support.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxProcessorCount();

		/** Returns the number of processors that are currently online.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getProcessorsOnline();

		/** Returns the maximum number of groups that a user can be
		 *  a member of, aside from its primary group.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxSupplementalGroupsPerUser();

		/** Returns the maximum number of delay timer expiration
		 *  overruns.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxDelayTimerExpirations();

		/** Returns the maximum number of realtime signals that
		 *  are reserved for application use.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxRealtimeSignals();

		/** Returns the maximum number of semaphores that a single
		 *  process can have open simultaneously.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxSemaphoresPerProcess();

		/** Returns the maximum value that a semaphore can have.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxSemaphoreValue();

		/** Returns the maximum number of signals that a single process
		 *  can have pending at once.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxSignalQueueLength();

		/** Returns the maximum number of timers that a single process
		 *  can have running at once.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxTimersPerProcess();

		/** Returns a recommended buffer size for use by the
		 *  getgrnam_r and getgrgid_r functions.  This is used
		 *  internally in the groupentry class if the system
		 *  supports those functions.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getSuggestedGroupEntryBufferSize();

		/** Returns a recommended buffer size for use by the
		 *  getpwnam_r and getpwgid_r functions.  This is used
		 *  internally in the userentry class if the system
		 *  supports those functions.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getSuggestedPasswordEntryBufferSize();

		/** Returns the minimum size of the stack for each thread.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMinThreadStackSize();

		/** Returns the maximum number of threads that a single process
		 *  can have open simultaneously.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxThreadsPerProcess();

		/** Returns the number of tries that will be made to
		 *  destroy thraed-specific data on thread exit.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getThreadDestructorIterations();

		/** Returns the maximum number of keys per process.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxThreadKeys();

		/** Returns the maximum number of functions that can be
		 *  registered to run when the process exits.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxAtExitFunctions();

		/** Returns the number of CPU's that can be bound together
		 *  in a single CPU Set.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getCPUSetSize();

		/** Returns the maximum length that a password can be.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxPasswordLength();

		/** Returns the maximum length that a password can be.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxLogNameLength();

		/** Returns the maximum length that a login name can be.
		 *  Returns -1 if not supported by the system. */
		static	int64_t	getMaxProcessID();

		/** Use the sysconf() system call directly to access values
		 *  other than the ones provided above. */
		static	int64_t	sysConf(int32_t name);

	#include <rudiments/private/sys.h>
};

#endif
