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

		static	bool	getCDevelopmentSupported();

		static	bool	getCLanguageBindingsSupported();

		static	bool	getFortranDevelopmentSupported();

		static	bool	getFortranRuntimeSupported();

		static	bool	getLocaleCreationSupported();

		static	bool	getSoftwareDevelopmentSupported();

		static	int64_t	getPhysicalPageCount();

		static	int64_t	getAvailablePhysicalPageCount();

		static	int64_t	getProcessorCount();

		static	int64_t	getProcessorsOnline();

		static	int64_t	getMaxIOVectorElements();

		static	int64_t	getMaxSupplementalGroupsPerUser();

		static	bool	getJobControlSupported();

		static	bool	getSavedIDsSupported();

		static	bool	getCharacterTerminalSupported();

		static	bool	getUserPortabilityUtilitiesSupported();

		static	int64_t	getMaxAIOOperationsPerList();

		static	int64_t	getMaxOutstandingAIOOperations();

		static	int64_t	getMaxAIOPriorityDelta();

		static	int64_t	getMaxDelayTimerExpirations();

		static	int64_t	getMaxMessageQueuesPerProcess();

		static	int64_t	getMaxRealtimeSignals();

		static	int64_t	getMaxSemaphoresPerProcess();

		static	int64_t	getMaxSemaphoreValue();

		static	int64_t	getMaxSignalQueueLength();

		static	int64_t	getMaxTimersPerProcess();

		static	int64_t	getSuggestedGroupEntryBufferSize();

		static	int64_t	getSuggestedPasswordEntryBufferSize();

		static	int64_t	getMinThreadStackSize();

		static	int64_t	getMaxThreadsPerProcess();

		static	int64_t	getMaxAtExitFunctions();

		static	int64_t	getXOpenVersion();

		static	int64_t	getXCUVersion();

		static	int64_t	getCPUSetSize();

		static	int64_t	getBarriersVersion();

		static	int64_t	getClockSelectionVersion();

		static	bool	getFSyncSupported();

		static	bool	getMemoryMappedFilesSupported();

		static	bool	getProcessMemoryLockingSupported();

		static	bool	getRangeMemoryLockingSupported();

		static	bool	getMemoryProtectionSupported();

		static	int64_t	getMonotonicClockVersion();

		static	int64_t	getMaxPasswordLength();

		static	int64_t	getReaderWriterLocksVersion();

		static	bool	getRegexSupported();

		static	int64_t	getSemaphoresVersion();

		static	bool	getPosixShellSupported();

		static	int64_t	getSpinLocksVersion();

		static	bool	getSynchronizedIOSupported();

		static	int64_t	getThreadsVersion();

		static	int64_t	getTimersVersion();

		static	bool	getSharedMemorySupported();

	#include <rudiments/private/system.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
