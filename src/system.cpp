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

#ifdef RUDIMENTS_HAVE_SYS_REBOOT_H
	// for reboot
	#include <sys/reboot.h>
#endif

#ifdef RUDIMENTS_HAVE_LINUX_REBOOT_H
	// for linux reboot commands
	#include <linux/reboot.h>
#endif

#ifdef RUDIMENTS_HAVE_PROTOTYPES_H
	// for uadmin
	#include <prototypes.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_UADMIN_H
	// for uadmin
	#include <sys/uadmin.h>
#endif

#ifdef RUDIMENTS_HAVE_ROSTER_H
	// for BRoster::_ShutDown
	#include <Roster.h>
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

bool system::halt() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_HALT)
			cmd=LINUX_REBOOT_CMD_HALT;
		#elif defined(RB_HALT)
			cmd=RB_HALT;
		#else
			#error no RB_HALT or anything like it
		#endif
		return (::reboot(cmd)!=-1);
	#elif defined(RUDIMENTS_HAVE_REBOOT_2)
		int32_t	cmd;
		#if defined(RB_HALT)
			cmd=RB_HALT;
		#else
			#error no RB_HALT or anything like it
		#endif
		return (::reboot(cmd,"")!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN)
		return (uadmin(A_SHUTDOWN,AD_HALT,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_ROSTER_PRIVATE_SHUTDOWN)
		_kern_shutdown(false);
		return true;
	#else
		#error no reboot or anything like it
	#endif
}

#ifdef RUDIMENTS_HAVE_BROSTER__SHUTDOWN
class BRoster::Private {
	public:
		Private(BRoster *r) : roster(r) {
		};
		status_t	ShutDown(bool reboot, bool confirm, bool sync) {
			return roster->_ShutDown(reboot,confirm,sync);
		};
	private:
		BRoster	*r;
};
#endif

bool system::shutDown() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_POWER_OFF)
			cmd=LINUX_REBOOT_CMD_POWER_OFF;
		#elif defined(RB_POWEROFF)
			cmd=RB_POWEROFF;
		#elif defined(RB_HALT)
			// OS X doesn't appear to have a power-down option,
			// just use halt instead
			cmd=RB_HALT;
		#else
			#error no RB_POWEROFF or anything like it
		#endif
		return (::reboot(cmd)!=-1);
	#elif defined(RUDIMENTS_HAVE_REBOOT_2)
		int32_t	cmd;
		#if defined(RB_POWERDOWN)
			cmd=RB_POWERDOWN;
		#elif defined(RB_HALT)
			// Solaris doesn't appear to have a power-down option,
			// just use halt instead
			cmd=RB_HALT;
		#else
			#error no RB_POWERDOWN or anything like it
		#endif
		return (::reboot(cmd,"")!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN)
		return (uadmin(A_SHUTDOWN,AD_PWRDOWN,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_BROSTER__SHUTDOWN)
		BRoster			roster;
		BRoster::Private	rosterprivate(&roster);
		return (rosterprivate.ShutDown(false,false,true)==B_OK);
	#else
		#error no reboot or anything like it
	#endif
}

bool system::reboot() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_RESTART)
			cmd=LINUX_REBOOT_CMD_RESTART;
		#elif defined(RB_AUTOBOOT)
			cmd=RB_AUTOBOOT;
		#else
			#error no RB_AUTOBOOT or anything like it
		#endif
		return (::reboot(cmd)!=-1);
	#elif defined(RUDIMENTS_HAVE_REBOOT_2)
		int32_t	cmd;
		#if defined(RB_AUTOBOOT)
			cmd=RB_AUTOBOOT;
		#else
			#error no RB_AUTOBOOT or anything like it
		#endif
		return (::reboot(cmd,"")!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN)
		return (uadmin(A_SHUTDOWN,AD_BOOT,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_ROSTER_PRIVATE_SHUTDOWN)
		BRoster			roster;
		BRoster::Private	rosterprivate(roster);
		return (rosterprivate._ShutDown(true,false,true)==B_OK);
	#else
		#error no reboot or anything like it
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

int64_t system::getMaxLineLength() {
	#if defined(_SC_LINE_MAX)
		return sysConf(_SC_LINE_MAX);
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

int64_t system::getMaxAtExitFunctions() {
	#if defined(_SC_ATEXIT_MAX)
		return sysConf(_SC_ATEXIT_MAX);
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

int64_t system::getMaxPasswordLength() {
	#if defined(_SC_PASS_MAX)
		return sysConf(_SC_PASS_MAX);
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
