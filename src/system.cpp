// Copyright (c) 2012 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/system.h>
#include <rudiments/error.h>
#include <rudiments/charstring.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	// for sysconf and getpagesize
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_UTSNAME_H
	// for uname
	#include <sys/utsname.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	// for getloadavg on linux/bsd
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_LOADAVG_H
	// for getloadavg on solaris
	#include <sys/loadavg.h>
#endif

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

char *system::getOperatingSystemName() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.sysname);
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *system::getOperatingSystemRelease() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.release);
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *system::getOperatingSystemVersion() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.version);
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *system::getOperatingSystemArchitecture() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.machine);
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *system::getHostName() {
	#if defined(RUDIMENTS_HAVE_GETHOSTNAME)

		// initialize winsock if necessary
		winsock::initWinsock();

		// initialize the result code
		int32_t		result;

		// start with a 64-byte buffer
		uint64_t	namelen=64;

		// try to get the name, retry if the buffer was too small...
		do {

			// create a buffer to store the name
			char	*name=new char[namelen];

			// try to get the name, retry if
			// the system call was interrupted
			do {
				result=gethostname(name,namelen);
			} while (result==-1 && error::getErrorNumber()==EINTR);

			// return the name on success
			if (!result) {
				return name;
			}

			// clean up,
			delete[] name;

			// resize the buffer
			namelen=namelen+64;

		} while (result==-1 && error::getErrorNumber()==ENAMETOOLONG);

		// for any other error, return null
		return NULL;
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

bool system::setHostName(const char *hostname) {
	return setHostName(hostname,charstring::length(hostname));
}

bool system::setHostName(const char *hostname, uint64_t hostnamelen) {
	#if defined(RUDIMENTS_HAVE_SETHOSTNAME)
		winsock::initWinsock();
		int32_t	result;
		do {
			// Some systems (Solaris) have a char *, rather than
			// const char * for the first parameter.  Nothing will
			// be written back to that parameter so it's safe to
			// cast it to a char * and this appears to work on all
			// platforms.
			result=sethostname((char *)hostname,hostnamelen);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

char *system::getNISDomainName() {
	#if defined(RUDIMENTS_HAVE_GETDOMAINNAME)

		// initialize winsock if necessary
		winsock::initWinsock();

		// initialize the result code
		int32_t		result;

		// start with a 64-byte buffer
		uint64_t	namelen=64;

		// try to get the name, retry if the buffer was too small...
		do {

			// create a buffer to store the name
			char	*name=new char[namelen];

			// try to get the name, retry if
			// the system call was interrupted
			do {
				result=getdomainname(name,namelen);
			} while (result==-1 && error::getErrorNumber()==EINTR);

			// return the name on success
			if (!result) {
				return name;
			}

			// clean up,
			delete[] name;

			// resize the buffer
			namelen=namelen+64;

		} while (result==-1 && error::getErrorNumber()==ENAMETOOLONG);

		// for any other error, return null
		return NULL;
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

bool system::setNISDomainName(const char *nisdomainname) {
	return setNISDomainName(nisdomainname,
				charstring::length(nisdomainname));
}

bool system::setNISDomainName(const char *nisdomainname,
					uint64_t nisdomainnamelen) {
	#if defined(RUDIMENTS_HAVE_SETDOMAINNAME)
		winsock::initWinsock();
		int32_t	result;
		do {
			result=setdomainname(nisdomainname,nisdomainnamelen);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

#ifndef LOADAVG_1MIN
	#define LOADAVG_1MIN 0
#endif

#ifndef LOADAVG_5MIN
	#define LOADAVG_5MIN 1
#endif

#ifndef LOADAVG_15MIN
	#define LOADAVG_15MIN 2
#endif

bool system::getLoadAverages(double *oneminuteaverage,
				double *fiveminuteaverage,
				double *fifteenminuteaverage) {
	#if defined(RUDIMENTS_HAVE_GETLOADAVG)
		double	averages[3]={0.0,0.0,0.0};
		bool	retval=!getloadavg(averages,3);
		*oneminuteaverage=averages[LOADAVG_1MIN];
		*fiveminuteaverage=averages[LOADAVG_5MIN];
		*fifteenminuteaverage=averages[LOADAVG_15MIN];
		return retval;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

void system::sync() {
	#if defined(RUDIMENTS_HAVE_SYNC)
		sync();
	#endif
}

int64_t system::getMaxCommandLineArgumentLength() {
	#if defined(_SC_ARG_MAX)
		return sysConf(_SC_ARG_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxProcessesPerUser() {
	#if defined(_SC_CHILD_MAX)
		return sysConf(_SC_CHILD_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxHostNameLength() {
	#if defined(_SC_HOST_NAME_MAX)
		return sysConf(_SC_HOST_NAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxLoginNameLength() {
	#if defined(_SC_LOGIN_NAME_MAX)
		return sysConf(_SC_LOGIN_NAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getClockTicksPerSecond() {
	#if defined(_SC_CLK_TCK)
		return sysConf(_SC_CLK_TCK);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxOpenFilesPerProcess() {
	#if defined(_SC_OPEN_MAX)
		return sysConf(_SC_OPEN_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int32_t system::getPageSize() {
	#if defined(RUDIMENTS_HAVE_GETPAGESIZE)
		return getpagesize();
	#elif defined(RUDIMENTS_HAVE_SYSCONF) && defined(_SC_PAGESIZE)
		return sysConf(_SC_PAGESIZE);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMINFO)
		SYSTEM_INFO	systeminfo;
		GetSystemInfo(&systeminfo);
		return systeminfo.dwPageSize;
	#else
		#error no getpagesize or anything like it
	#endif
}

int64_t system::getMaxRegexDups() {
	#if defined(_SC_RE_DUP_MAX)
		return sysConf(_SC_RE_DUP_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxOpenStreamsPerProcess() {
	#if defined(_SC_STREAM_MAX)
		return sysConf(_SC_STREAM_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxSymlinkLoops() {
	#if defined(_SC_SYMLOOP_MAX)
		return sysConf(_SC_SYMLOOP_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxTerminalDeviceNameLength() {
	#if defined(_SC_TTY_NAME_MAX)
		return sysConf(_SC_TTY_NAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxTimezoneNameLength() {
	#if defined(_SC_TZNAME_MAX)
		return sysConf(_SC_TZNAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getPosixVersion() {
	#if defined(_SC_VERSION)
		return sysConf(_SC_VERSION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxCalculatorBase() {
	#if defined(_SC_BC_BASE_MAX)
		return sysConf(_SC_BC_BASE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxCalculatorValue() {
	#if defined(_SC_BC_DIM_MAX)
		return sysConf(_SC_BC_DIM_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxCalculatorScale() {
	#if defined(_SC_BC_SCALE_MAX)
		return sysConf(_SC_BC_SCALE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxCalculatorStringLength() {
	#if defined(_SC_BC_STRING_MAX)
		return sysConf(_SC_BC_STRING_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxCollationWeights() {
	#if defined(_SC_COLL_WEIGHTS_MAX)
		return sysConf(_SC_COLL_WEIGHTS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxNestedExpressions() {
	#if defined(_SC_EXPR_NEST_MAX)
		return sysConf(_SC_EXPR_NEST_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxLineLength() {
	#if defined(_SC_LINE_MAX)
		return sysConf(_SC_LINE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getPosix2Version() {
	#if defined(_SC_2_VERSION)
		return sysConf(_SC_2_VERSION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getCDevelopmentSupported() {
	#if defined(_SC_2_C_DEV)
		return (sysConf(_SC_2_C_DEV)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getCLanguageBindingsSupported() {
	#if defined(_SC_2_C_BIND)
		return (sysConf(_SC_2_C_BIND)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getFortranDevelopmentSupported() {
	#if defined(_SC_2_FORT_DEV)
		return (sysConf(_SC_2_FORT_DEV)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getFortranRuntimeSupported() {
	#if defined(_SC_2_FORT_RUN)
		return (sysConf(_SC_2_FORT_RUN)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getLocaleCreationSupported() {
	#if defined(_SC_2_LOCALEDEF)
		return (sysConf(_SC_2_LOCALEDEF)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getSoftwareDevelopmentSupported() {
	#if defined(_SC_2_SW_DEV)
		return (sysConf(_SC_2_SW_DEV)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int64_t system::getPhysicalPageCount() {
	#if defined(_SC_PHYS_PAGES)
		return sysConf(_SC_PHYS_PAGES);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getAvailablePhysicalPageCount() {
	#if defined(_SC_AVPHYS_PAGES)
		return sysConf(_SC_AVPHYS_PAGES);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getProcessorCount() {
	#if defined(_SC_NPROCESSORS_CONF)
		return sysConf(_SC_NPROCESSORS_CONF);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getProcessorsOnline() {
	#if defined(_SC_NPROCESSORS_ONLN)
		return sysConf(_SC_NPROCESSORS_ONLN);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxIOVectorElements() {
	#if defined(_SC_IOV_MAX)
		return sysConf(_SC_IOV_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxSupplementalGroupsPerUser() {
	#if defined(_SC_NGROUPS_MAX)
		return sysConf(_SC_NGROUPS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getJobControlSupported() {
	#if defined(_SC_JOB_CONTROL)
		return (sysConf(_SC_JOB_CONTROL)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getSavedIDsSupported() {
	#if defined(_SC_SAVED_IDS)
		return (sysConf(_SC_SAVED_IDS)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getCharacterTerminalSupported() {
	#if defined(_SC_2_CHAR_TERM)
		return (sysConf(_SC_2_CHAR_TERM)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool system::getUserPortabilityUtilitiesSupported() {
	#if defined(_SC_2_UPE)
		return (sysConf(_SC_2_UPE)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int64_t system::getMaxAIOOperationsPerList() {
	#if defined(_SC_AIO_LISTIO_MAX)
		return sysConf(_SC_AIO_LISTIO_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxOutstandingAIOOperations() {
	#if defined(_SC_AIO_MAX)
		return sysConf(_SC_AIO_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxAIOPriorityDelta() {
	#if defined(_SC_AIO_PRIO_DELTA_MAX)
		return sysConf(_SC_AIO_PRIO_DELTA_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxDelayTimerExpirations() {
	#if defined(_SC_DELAYTIMER_MAX)
		return sysConf(_SC_DELAYTIMER_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxMessageQueuesPerProcess() {
	#if defined(_SC_MQ_OPEN_MAX)
		return sysConf(_SC_MQ_OPEN_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxRealtimeSignals() {
	#if defined(_SC_RTSIG_MAX)
		return sysConf(_SC_RTSIG_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxSemaphoresPerProcess() {
	#if defined(_SC_SEM_NSEMS_MAX)
		return sysConf(_SC_SEM_NSEMS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxSemaphoreValue() {
	#if defined(_SC_SEM_VALUE_MAX)
		return sysConf(_SC_SEM_VALUE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxSignalQueueLength() {
	#if defined(_SC_SIGQUEUE_MAX)
		return sysConf(_SC_SIGQUEUE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxTimersPerProcess() {
	#if defined(_SC_TIMER_MAX)
		return sysConf(_SC_TIMER_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSuggestedGroupEntryBufferSize() {
	#if defined(_SC_GETGR_R_SIZE_MAX)
		return sysConf(_SC_GETGR_R_SIZE_MAX);
	#else
		return 1024;
	#endif
}

int64_t system::getSuggestedPasswordEntryBufferSize() {
	#if defined(_SC_GETPW_R_SIZE_MAX)
		return sysConf(_SC_GETPW_R_SIZE_MAX);
	#else
		return 1024;
	#endif
}

int64_t system::getMinThreadStackSize() {
	#if defined(_SC_THREAD_STACK_MIN)
		return sysConf(_SC_THREAD_STACK_MIN);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxThreadsPerProcess() {
	#if defined(_SC_THREAD_THREADS_MAX)
		return sysConf(_SC_THREAD_THREADS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxAtExitFunctions() {
	#if defined(_SC_ATEXIT_MAX)
		return sysConf(_SC_ATEXIT_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenVersion() {
	#if defined(_SC_XOPEN_VERSION)
		return sysConf(_SC_XOPEN_VERSION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXCUVersion() {
	#if defined(_SC_XOPEN_XCU_VERSION)
		return sysConf(_SC_XOPEN_XCU_VERSION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getCPUSetSize() {
	#if defined(_SC_CPUSET_SIZE)
		return sysConf(_SC_CPUSET_SIZE);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getBarriersVersion() {
	#if defined(_SC_BARRIERS)
		return sysConf(_SC_BARRIERS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getClockSelectionVersion() {
	#if defined(_SC_CLOCK_SELECTION)
		return sysConf(_SC_CLOCK_SELECTION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getFSyncSupported() {
	#if defined(_SC_FSYNC)
		return (sysConf(_SC_FSYNC)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getMemoryMappedFilesSupported() {
	#if defined(_SC_MAPPED_FILES)
		return (sysConf(_SC_MAPPED_FILES)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getProcessMemoryLockingSupported() {
	#if defined(_SC_MEMLOCK)
		return (sysConf(_SC_MEMLOCK)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getRangeMemoryLockingSupported() {
	#if defined(_SC_MEMLOCK_RANGE)
		return (sysConf(_SC_MEMLOCK_RANGE)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getMemoryProtectionSupported() {
	#if defined(_SC_MEMORY_PROTECTION)
		return (sysConf(_SC_MEMORY_PROTECTION)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMonotonicClockVersion() {
	#if defined(_SC_MONOTONIC_CLOCK)
		return sysConf(_SC_MONOTONIC_CLOCK);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxPasswordLength() {
	#if defined(_SC_PASS_MAX)
		return sysConf(_SC_PASS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getReaderWriterLocksVersion() {
	#if defined(_SC_READER_WRITER_LOCKS)
		return sysConf(_SC_READER_WRITER_LOCKS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getRegexSupported() {
	#if defined(_SC_REGEXP)
		return (sysConf(_SC_REGEXP)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSemaphoresVersion() {
	#if defined(_SC_SEMAPHORES)
		return sysConf(_SC_SEMAPHORES);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getPosixShellSupported() {
	#if defined(_SC_SHELL)
		return (sysConf(_SC_SHELL)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSpinLocksVersion() {
	#if defined(_SC_SPIN_LOCKS)
		return sysConf(_SC_SPIN_LOCKS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getSynchronizedIOSupported() {
	#if defined(_SC_SYNCHRONIZED_IO)
		return (sysConf(_SC_SYNCHRONIZED_IO)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadsVersion() {
	#if defined(_SC_THREADS)
		return sysConf(_SC_THREADS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getTimersVersion() {
	#if defined(_SC_TIMERS)
		return sysConf(_SC_TIMERS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool system::getSharedMemorySupported() {
	#if defined(_SC_XOPEN_SHM)
		return (sysConf(_SC_XOPEN_SHM)!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::sysConf(int32_t name) {
	#if defined(RUDIMENTS_HAVE_SYSCONF)
		int64_t	result;
		do {
			result=sysconf(name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
