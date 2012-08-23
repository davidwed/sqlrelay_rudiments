// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SYSTEM_H
#define RUDIMENTS_SYSTEM_H

#include <rudiments/private/systemincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// @cond

class systemprivate;

// wrap:
//	unistd.h - sysconf()
//			confstr() - use sysctl() on bsd
//			gethostid(),sethostid() - use sysctl() on bsd
//			sync()
//	sys/raw.h - structs/defines for /dev/rawctl ioctl()'s
//	not in solaris - 
//		sys/sysctl.h - sysctl()
//	linux only - 
//		sys/kdaemon.h - bdflush()
//		sys/klog.h - klogctl()
//		sys/personality.h - personality() - set linux kernel personality
//		sys/reboot.h - reboot()
//		sys/swap.h - swapon()/swapoff()
//		sys/sysinfo.h - functions for gettting overall system statistics
//				sysinfo(),
//				get_nprocs_conf(),get_nprocs(),
//				get_phys_pages,get_avphys_pages()
class RUDIMENTS_DLLSPEC system {
	public:

		static	char	*getOperatingSystemName();

		static	char	*getOperatingSystemRelease();

		static	char	*getOperatingSystemVersion();

		static	char	*getOperatingSystemArchitecture();

		static	char	*getHostName();
		static	bool	setHostName(const char *hostname);
		static	bool	setHostName(const char *hostname,
						uint64_t hostnamelen);

		static	char	*getNISDomainName();
		static	bool	setNISDomainName(const char *domainname);
		static	bool	setNISDomainName(const char *domainname,
						uint64_t domainnamelen);

		static	bool	getLoadAverages(double *oneminuteaverage,
						double *fiveminuteaverage,
						double *fifteenminuteaverage);

		static	void	sync();

		static	int64_t	getMaxCommandLineArgumentLength();

		static	int64_t	getMaxProcessesPerUser();

		static	int64_t	getMaxHostNameLength();

		static	int64_t	getMaxLoginNameLength();

		static	int64_t	getClockTicksPerSecond();

		static	int64_t	getMaxOpenFilesPerProcess();

		static	int32_t	getPageSize();

		static	int64_t	getMaxRegexDups();

		static	int64_t	getMaxOpenStreamsPerProcess();

		static	int64_t	getMaxSymlinkLoops();

		static	int64_t	getMaxTerminalDeviceNameLength();

		static	int64_t	getMaxTimezoneNameLength();

		static	int64_t	getPosixVersion();

		static	int64_t	getMaxCalculatorBase();

		static	int64_t	getMaxCalculatorValue();

		static	int64_t	getMaxCalculatorScale();

		static	int64_t	getMaxCalculatorStringLength();

		static	int64_t	getMaxCollationWeights();

		static	int64_t	getMaxNestedExpressions();

		static	int64_t	getMaxLineLength();

		static	int64_t	getPosix2Version();

		static	int64_t	getCDevelopmentSupported();

		static	int64_t	getFortranDevelopmentSupported();

		static	int64_t	getFortranRuntimeSupported();

		static	int64_t	getLocaleCreationSupported();

		static	int64_t	getSoftwareDevelopmentSupported();

		static	int64_t	getPhysicalPageCount();

		static	int64_t	getAvailablePhysicalPageCount();

		static	int64_t	getProcessorCount();

		static	int64_t	getProcessorsOnline();

	#include <rudiments/private/system.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
