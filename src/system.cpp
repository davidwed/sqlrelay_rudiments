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
	#elif defined(RUDIMENTS_HAVE_SYSCONF) && defined(_SC_PAGE_SIZE)
		return sysConf(_SC_PAGE_SIZE);
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

int64_t system::getCDevelopmentSupport() {
	#if defined(_SC_2_C_DEV)
		return sysConf(_SC_2_C_DEV);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getCLanguageBindingsSupport() {
	#if defined(_SC_2_C_BIND)
		return sysConf(_SC_2_C_BIND);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getCLanguageSupport() {
	#if defined(_SC_2_C_VERSION)
		return sysConf(_SC_2_C_VERSION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getFortranDevelopmentSupport() {
	#if defined(_SC_2_FORT_DEV)
		return sysConf(_SC_2_FORT_DEV);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getFortranRuntimeSupport() {
	#if defined(_SC_2_FORT_RUN)
		return sysConf(_SC_2_FORT_RUN);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getLocaleCreationSupport() {
	#if defined(_SC_2_LOCALEDEF)
		return sysConf(_SC_2_LOCALEDEF);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSoftwareDevelopmentSupport() {
	#if defined(_SC_2_SW_DEV)
		return sysConf(_SC_2_SW_DEV);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
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

int64_t system::getMaxProcessorCount() {
	#if defined(_SC_NPROCESSORS_MAX)
		return sysConf(_SC_NPROCESSORS_MAX);
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
	#elif defined(_SC_UIO_MAXIOV)
		return sysConf(_SC_UIO_MAXIOV);
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

int64_t system::getJobControlSupport() {
	#if defined(_SC_JOB_CONTROL)
		return sysConf(_SC_JOB_CONTROL);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSavedIDsSupport() {
	#if defined(_SC_SAVED_IDS)
		return sysConf(_SC_SAVED_IDS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getCharacterTerminalSupport() {
	#if defined(_SC_2_CHAR_TERM)
		return sysConf(_SC_2_CHAR_TERM);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getUserPortabilityUtilitiesSupport() {
	#if defined(_SC_2_UPE)
		return sysConf(_SC_2_UPE);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getAIOSupport() {
	#if defined(_SC_ASYNCHRONOUS_ID)
		return sysConf(_SC_ASYNCHRONOUS_IO);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
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

int64_t system::getMaxMessagePriorities() {
	#if defined(_SC_MQ_PRIO_MAX)
		return sysConf(_SC_MQ_PRIO_MAX);
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

int64_t system::getThreadStackAddressOptionSupport() {
	#if defined(_SC_THREAD_ATTR_STACKADDR)
		return sysConf(_SC_THREAD_ATTR_STACKADDR);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadStackSizeOptionSupport() {
	#if defined(_SC_THREAD_ATTR_STACKSIZE)
		return sysConf(_SC_THREAD_ATTR_STACKSIZE);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadDestructorIterations() {
	#if defined(_SC_THREAD_DESTRUCTOR_ITERATIONS)
		return sysConf(_SC_THREAD_DESTRUCTOR_ITERATIONS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxThreadKeys() {
	#if defined(_SC_THREAD_KEYS_MAX)
		return sysConf(_SC_THREAD_KEYS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadPriorityInheritanceSupport() {
	#if defined(_SC_THREAD_PRIO_INHERIT)
		return sysConf(_SC_THREAD_PRIO_INHERIT);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadPriorityProtectionSupport() {
	#if defined(_SC_THREAD_PRIO_PROTECT)
		return sysConf(_SC_THREAD_PRIO_PROTECT);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadPrioritySchedulingSupport() {
	#if defined(_SC_THREAD_PRIORITY_SCHEDULING)
		return sysConf(_SC_THREAD_PRIORITY_SCHEDULING);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadProcessSharedSyncSupport() {
	#if defined(_SC_THREAD_PROCESS_SHARED)
		return sysConf(_SC_THREAD_PROCESS_SHARED);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadSafeFunctionsSupport() {
	#if defined(_SC_THREAD_SAFE_FUNCTIONS)
		return sysConf(_SC_THREAD_SAFE_FUNCTIONS);
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

int64_t system::getXOpenXCUVersion() {
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

int64_t system::getBarriersSupport() {
	#if defined(_SC_BARRIERS)
		return sysConf(_SC_BARRIERS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getClockSelectionSupport() {
	#if defined(_SC_CLOCK_SELECTION)
		return sysConf(_SC_CLOCK_SELECTION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getFSyncSupport() {
	#if defined(_SC_FSYNC)
		return sysConf(_SC_FSYNC);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMemoryMappedFilesSupport() {
	#if defined(_SC_MAPPED_FILES)
		return sysConf(_SC_MAPPED_FILES);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getProcessMemoryLockingSupport() {
	#if defined(_SC_MEMLOCK)
		return sysConf(_SC_MEMLOCK);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getRangeMemoryLockingSupport() {
	#if defined(_SC_MEMLOCK_RANGE)
		return sysConf(_SC_MEMLOCK_RANGE);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMemoryProtectionSupport() {
	#if defined(_SC_MEMORY_PROTECTION)
		return sysConf(_SC_MEMORY_PROTECTION);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMonotonicClockSupport() {
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

int64_t system::getReaderWriterLocksSupport() {
	#if defined(_SC_READER_WRITER_LOCKS)
		return sysConf(_SC_READER_WRITER_LOCKS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getRegexSupport() {
	#if defined(_SC_REGEXP)
		return sysConf(_SC_REGEXP);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSemaphoresSupport() {
	#if defined(_SC_SEMAPHORES)
		return sysConf(_SC_SEMAPHORES);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getPosixShellSupport() {
	#if defined(_SC_SHELL)
		return sysConf(_SC_SHELL);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSpinLocksSupport() {
	#if defined(_SC_SPIN_LOCKS)
		return sysConf(_SC_SPIN_LOCKS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSynchronizedIOSupport() {
	#if defined(_SC_SYNCHRONIZED_IO)
		return sysConf(_SC_SYNCHRONIZED_IO);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getThreadsSupport() {
	#if defined(_SC_THREADS)
		return sysConf(_SC_THREADS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getTimersSupport() {
	#if defined(_SC_TIMERS)
		return sysConf(_SC_TIMERS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenSharedMemorySupport() {
	#if defined(_SC_XOPEN_SHM)
		return sysConf(_SC_XOPEN_SHM);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSharedMemoryObjectsSupport() {
	#if defined(_SC_SHARED_MEMORY_OBJECTS)
		return sysConf(_SC_SHARED_MEMORY_OBJECTS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxLogNameLength() {
	#if defined(_SC_LOGNAME_MAX)
		return sysConf(_SC_LOGNAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMaxProcessID() {
	#if defined(_SC_MAXPID)
		return sysConf(_SC_MAXPID);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getMessagePassingSupport() {
	#if defined(_SC_MESSAGE_PASSING)
		return sysConf(_SC_MESSAGE_PASSING);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getPrioritizedIOSupport() {
	#if defined(_SC_PRIORITIZED_IO)
		return sysConf(_SC_PRIORITIZED_IO);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getRawSocketsSupport() {
	#if defined(_SC_RAW_SOCKETS)
		return sysConf(_SC_RAW_SOCKETS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getRealtimeSignalsSupport() {
	#if defined(_SC_REALTIME_SIGNALS)
		return sysConf(_SC_REALTIME_SIGNALS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getSpawnSupport() {
	#if defined(_SC_SPAWN)
		return sysConf(_SC_SPAWN);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getStackProtectionSupport() {
	#if defined(_SC_STACK_PROT)
		return sysConf(_SC_STACK_PROT);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getTimeoutsSupport() {
	#if defined(_SC_TIMEOUTS)
		return sysConf(_SC_TIMEOUTS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenILP32With32BitOffsetSupport() {
	#if defined(_SC_V6_ILP32_OFF32)
		return sysConf(_SC_V6_ILP32_OFF32);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenILP32With64BitOffsetSupport() {
	#if defined(_SC_V6_ILP32_OFFBIG)
		return sysConf(_SC_V6_ILP32_OFFBIG);
	#elif defined(_SC_V6_ILP32_OFF64)
		return sysConf(_SC_V6_ILP32_OFF64);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenCryptSupport() {
	#if defined(_SC_XOPEN_CRYPT)
		return sysConf(_SC_XOPEN_CRYPT);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenEnhancedInternationalizationSupport() {
	#if defined(_SC_XOPEN_ENH_I18N)
		return sysConf(_SC_XOPEN_ENH_I18N);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenLegacySupport() {
	#if defined(_SC_XOPEN_LEGACY)
		return sysConf(_SC_XOPEN_LEGACY);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenRealtimeSupport() {
	#if defined(_SC_XOPEN_REALTIME)
		return sysConf(_SC_XOPEN_REALTIME);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenRealtimeThreadsSupport() {
	#if defined(_SC_XOPEN_REALTIME_THREADS)
		return sysConf(_SC_XOPEN_REALTIME_THREADS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenStreamsSupport() {
	#if defined(_SC_XOPEN_STREAMS)
		return sysConf(_SC_XOPEN_STREAMS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t system::getXOpenUnixSupport() {
	#if defined(_SC_XOPEN_UNIX)
		return sysConf(_SC_XOPEN_UNIX);
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
