// Copyright (c) 2012 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/sys.h>
#include <rudiments/error.h>
#include <rudiments/charstring.h>
#include <rudiments/device.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/stringbuffer.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	// for sysconf, getpagesize, gethostname, sethostname
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
	// for BRoster::_ShutDown on Haiku
	#include <Roster.h>
	extern "C" {
		extern	status_t	_kern_shutdown(bool reboot);
	}
#endif

#ifdef RUDIMENTS_HAVE_MISSING_SETHOSTNAME_DECLARATION
	extern "C" {
		extern	int	sethostname(char *name, int namelen);
	}
#endif

#include <stdio.h>

char *sys::getOperatingSystemName() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.sysname);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		return charstring::duplicate("Windows");
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *sys::getOperatingSystemRelease() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.release);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// for windows, just return the same as the version number
		return getOperatingSystemVersion();
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *sys::getOperatingSystemVersion() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.version);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)

		// get the os version info
		// (yes, this craziness is how you have to do it)
		OSVERSIONINFOEX	info;
		rawbuffer::zero(&info,sizeof(info));
		info.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
		if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
			info.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
				return NULL;
			}
		}

		// build up the version number
		stringbuffer	retval;
		retval.append((uint64_t)info.dwMajorVersion)->append(".");
		retval.append((uint64_t)info.dwMinorVersion)->append(".");
		retval.append((uint64_t)info.dwBuildNumber);

		// return the version number
		return retval.detachString();
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *sys::getOperatingSystemArchitecture() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return (result==-1)?NULL:charstring::duplicate(u.machine);
	#elif defined(RUDIMENTS_HAVE_GETNATIVESYSTEMINFO)

		SYSTEM_INFO	info;
		GetNativeSystemInfo((LPSYSTEM_INFO)&info);

		char	*arch=NULL;
		switch (info.wProcessorArchitecture) {
			case PROCESSOR_ARCHITECTURE_AMD64:
				arch=charstring::duplicate("amd64");
				break;
			case PROCESSOR_ARCHITECTURE_IA64:
				arch=charstring::duplicate("x86_64");
				break;
			case PROCESSOR_ARCHITECTURE_INTEL:
				arch=charstring::duplicate("x86");
				break;
			case PROCESSOR_ARCHITECTURE_UNKNOWN:
				arch=charstring::duplicate("Unknown");
				break;
		}
		return arch;
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

char *sys::getHostName() {
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

bool sys::setHostName(const char *hostname) {
	return setHostName(hostname,charstring::length(hostname));
}

bool sys::setHostName(const char *hostname, uint64_t hostnamelen) {
	#if defined(RUDIMENTS_HAVE_SETHOSTNAME) || \
		defined(RUDIMENTS_HAVE_MISSING_SETHOSTNAME_DECLARATION)
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

#ifndef LOADAVG_1MIN
	#define LOADAVG_1MIN 0
#endif

#ifndef LOADAVG_5MIN
	#define LOADAVG_5MIN 1
#endif

#ifndef LOADAVG_15MIN
	#define LOADAVG_15MIN 2
#endif

bool sys::getLoadAverages(double *oneminuteaverage,
				double *fiveminuteaverage,
				double *fifteenminuteaverage) {
	*oneminuteaverage=0.0;
	*fiveminuteaverage=0.0;
	*fifteenminuteaverage=0.0;
	#if defined(RUDIMENTS_HAVE_GETLOADAVG)
		// use getloadavg if the system has it
		double	averages[3]={0.0,0.0,0.0};
		bool	retval=!getloadavg(averages,3);
		*oneminuteaverage=averages[LOADAVG_1MIN];
		*fiveminuteaverage=averages[LOADAVG_5MIN];
		*fifteenminuteaverage=averages[LOADAVG_15MIN];
		return retval;
	#elif defined(RUDIMENTS_HAVE_SCO_AVENRUN)
		// use avenrun if the system has it
		device	avenrun;
		if (avenrun.open("/dev/table/avenrun",O_RDONLY)) {
			uint16_t	avgs[3];
			if (avenrun.read((void *)avgs,sizeof(avgs))==
							sizeof(avgs)) {
				*oneminuteaverage=((double)avgs[0])/256.0;
				*fiveminuteaverage=((double)avgs[1])/256.0;
				*fifteenminuteaverage=((double)avgs[2])/256.0;
				return true;
			}
		}
		return false;
	#else
		// try /proc/loadavg as a last-ditch effort
		device	loadavg;
		if (!loadavg.open("/proc/loadavg",O_RDONLY)) {
			error::setErrorNumber(ENOSYS);
			return false;
		}

		char		*buffer=NULL;
		if (loadavg.read(&buffer," ")==RESULT_ERROR) {
			return false;
		}
		*oneminuteaverage=(double)charstring::toFloat(buffer);
		delete[] buffer;

		if (loadavg.read(&buffer," ")==RESULT_ERROR) {
			return false;
		}
		*fiveminuteaverage=(double)charstring::toFloat(buffer);
		delete[] buffer;
		
		if (loadavg.read(&buffer," ")==RESULT_ERROR) {
			return false;
		}
		*fifteenminuteaverage=(double)charstring::toFloat(buffer);
		delete[] buffer;

		return true;
	#endif
}

void sys::sync() {
	#if defined(RUDIMENTS_HAVE_SYNC)
		sync();
	#endif
}

bool sys::halt() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_HALT)
			cmd=LINUX_REBOOT_CMD_HALT;
		#elif defined(RB_HALT)
			cmd=RB_HALT;
		#elif defined(RB_HALT_SYSTEM)
			cmd=RB_HALT_SYSTEM;
		#elif defined(RBT_HALT)
			cmd=RBT_HALT;
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
		// bootstr is const char * on some platforms and char * on
		// others.  This works for both.
		char	*bootstr=(char *)"";
		return (::reboot(cmd,bootstr)==-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_CHAR)
		return (uadmin(A_SHUTDOWN,AD_HALT,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_INT)
		return (uadmin(A_SHUTDOWN,AD_HALT,0)!=-1);
	#elif defined(RUDIMENTS_HAVE_BROSTER__SHUTDOWN)
		_kern_shutdown(false);
		return true;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
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
		BRoster	*roster;
};
#endif

bool sys::shutDown() {
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
		#elif defined(RB_HALT_SYSTEM)
			// Older linux doesn't have a power-down option
			// just use halt instead
			cmd=RB_HALT_SYSTEM;
		#elif defined(RBT_HALT)
			// Minix doesn't have a power-down option
			// just use halt instead
			cmd=RBT_HALT;
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
		// bootstr is const char * on some platforms and char * on
		// others.  This works for both.
		char	*bootstr=(char *)"";
		return (::reboot(cmd,bootstr)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_CHAR)
		return (uadmin(A_SHUTDOWN,AD_PWRDOWN,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_INT)
		// OSR6 doesn't appear to define AD_PWRDOWN,
		// just use halt instead.
		return (uadmin(A_SHUTDOWN,AD_HALT,0)!=-1);
	#elif defined(RUDIMENTS_HAVE_BROSTER__SHUTDOWN)
		BRoster			roster;
		BRoster::Private	rosterprivate(&roster);
		return (rosterprivate.ShutDown(false,false,true)==B_OK);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sys::reboot() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_RESTART)
			cmd=LINUX_REBOOT_CMD_RESTART;
		#elif defined(RB_AUTOBOOT)
			cmd=RB_AUTOBOOT;
		#elif defined(RBT_REBOOT)
			cmd=RBT_REBOOT;
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
		// bootstr is const char * on some platforms and char * on
		// others.  This works for both.
		char	*bootstr=(char *)"";
		return (::reboot(cmd,bootstr)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_CHAR)
		return (uadmin(A_SHUTDOWN,AD_BOOT,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_INT)
		return (uadmin(A_SHUTDOWN,AD_BOOT,0)!=-1);
	#elif defined(RUDIMENTS_HAVE_BROSTER__SHUTDOWN)
		BRoster			roster;
		BRoster::Private	rosterprivate(&roster);
		return (rosterprivate.ShutDown(true,false,true)==B_OK);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int64_t sys::getMaxCommandLineArgumentLength() {
	#if defined(_SC_ARG_MAX)
		return sysConf(_SC_ARG_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxProcessesPerUser() {
	#if defined(_SC_CHILD_MAX)
		return sysConf(_SC_CHILD_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxHostNameLength() {
	#if defined(_SC_HOST_NAME_MAX)
		return sysConf(_SC_HOST_NAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxLoginNameLength() {
	#if defined(_SC_LOGIN_NAME_MAX)
		return sysConf(_SC_LOGIN_NAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getClockTicksPerSecond() {
	#if defined(_SC_CLK_TCK)
		return sysConf(_SC_CLK_TCK);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxOpenFilesPerProcess() {
	#if defined(_SC_OPEN_MAX)
		return sysConf(_SC_OPEN_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int32_t sys::getPageSize() {
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

int64_t sys::getMaxOpenStreamsPerProcess() {
	#if defined(_SC_STREAM_MAX)
		return sysConf(_SC_STREAM_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxSymlinkLoops() {
	#if defined(_SC_SYMLOOP_MAX)
		return sysConf(_SC_SYMLOOP_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxTerminalDeviceNameLength() {
	#if defined(_SC_TTY_NAME_MAX)
		return sysConf(_SC_TTY_NAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxTimezoneNameLength() {
	#if defined(_SC_TZNAME_MAX)
		return sysConf(_SC_TZNAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxLineLength() {
	#if defined(_SC_LINE_MAX)
		return sysConf(_SC_LINE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getPhysicalPageCount() {
	#if defined(_SC_PHYS_PAGES)
		return sysConf(_SC_PHYS_PAGES);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getAvailablePhysicalPageCount() {
	#if defined(_SC_AVPHYS_PAGES)
		return sysConf(_SC_AVPHYS_PAGES);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getProcessorCount() {
	#if defined(_SC_NPROCESSORS_CONF)
		return sysConf(_SC_NPROCESSORS_CONF);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxProcessorCount() {
	#if defined(_SC_NPROCESSORS_MAX)
		return sysConf(_SC_NPROCESSORS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getProcessorsOnline() {
	#if defined(_SC_NPROCESSORS_ONLN)
		return sysConf(_SC_NPROCESSORS_ONLN);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxSupplementalGroupsPerUser() {
	#if defined(_SC_NGROUPS_MAX)
		return sysConf(_SC_NGROUPS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxDelayTimerExpirations() {
	#if defined(_SC_DELAYTIMER_MAX)
		return sysConf(_SC_DELAYTIMER_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxRealtimeSignals() {
	#if defined(_SC_RTSIG_MAX)
		return sysConf(_SC_RTSIG_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxSemaphoresPerProcess() {
	#if defined(_SC_SEM_NSEMS_MAX)
		return sysConf(_SC_SEM_NSEMS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxSemaphoreValue() {
	#if defined(_SC_SEM_VALUE_MAX)
		return sysConf(_SC_SEM_VALUE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxSignalQueueLength() {
	#if defined(_SC_SIGQUEUE_MAX)
		return sysConf(_SC_SIGQUEUE_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxTimersPerProcess() {
	#if defined(_SC_TIMER_MAX)
		return sysConf(_SC_TIMER_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getSuggestedGroupEntryBufferSize() {
	#if defined(_SC_GETGR_R_SIZE_MAX)
		return sysConf(_SC_GETGR_R_SIZE_MAX);
	#else
		return 1024;
	#endif
}

int64_t sys::getSuggestedPasswordEntryBufferSize() {
	#if defined(_SC_GETPW_R_SIZE_MAX)
		return sysConf(_SC_GETPW_R_SIZE_MAX);
	#else
		return 1024;
	#endif
}

int64_t sys::getMinThreadStackSize() {
	#if defined(_SC_THREAD_STACK_MIN)
		return sysConf(_SC_THREAD_STACK_MIN);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxThreadsPerProcess() {
	#if defined(_SC_THREAD_THREADS_MAX)
		return sysConf(_SC_THREAD_THREADS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getThreadDestructorIterations() {
	#if defined(_SC_THREAD_DESTRUCTOR_ITERATIONS)
		return sysConf(_SC_THREAD_DESTRUCTOR_ITERATIONS);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxThreadKeys() {
	#if defined(_SC_THREAD_KEYS_MAX)
		return sysConf(_SC_THREAD_KEYS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxAtExitFunctions() {
	#if defined(_SC_ATEXIT_MAX)
		return sysConf(_SC_ATEXIT_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getCPUSetSize() {
	#if defined(_SC_CPUSET_SIZE)
		return sysConf(_SC_CPUSET_SIZE);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxPasswordLength() {
	#if defined(_SC_PASS_MAX)
		return sysConf(_SC_PASS_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxLogNameLength() {
	#if defined(_SC_LOGNAME_MAX)
		return sysConf(_SC_LOGNAME_MAX);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::getMaxProcessID() {
	#if defined(_SC_MAXPID)
		return sysConf(_SC_MAXPID);
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t sys::sysConf(int32_t name) {
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
