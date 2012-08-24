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

		static	int64_t	getCDevelopmentSupport();

		static	int64_t	getCLanguageBindingsSupport();

		static	int64_t	getCLanguageSupport();

		static	int64_t	getFortranDevelopmentSupport();

		static	int64_t	getFortranRuntimeSupport();

		static	int64_t	getLocaleCreationSupport();

		static	int64_t	getSoftwareDevelopmentSupport();

		static	int64_t	getPhysicalPageCount();

		static	int64_t	getAvailablePhysicalPageCount();

		static	int64_t	getProcessorCount();

		static	int64_t	getMaxProcessorCount();

		static	int64_t	getProcessorsOnline();

		static	int64_t	getMaxIOVectorElements();

		static	int64_t	getMaxSupplementalGroupsPerUser();

		static	int64_t	getJobControlSupport();

		static	int64_t	getSavedIDsSupport();

		static	int64_t	getCharacterTerminalSupport();

		static	int64_t	getUserPortabilityUtilitiesSupport();

		static	int64_t	getAIOSupport();

		static	int64_t	getMaxAIOOperationsPerList();

		static	int64_t	getMaxOutstandingAIOOperations();

		static	int64_t	getMaxAIOPriorityDelta();

		static	int64_t	getMaxDelayTimerExpirations();

		static	int64_t	getMaxMessageQueuesPerProcess();

		static	int64_t	getMaxMessagePriorities();

		static	int64_t	getMaxRealtimeSignals();

		static	int64_t	getMaxSemaphoresPerProcess();

		static	int64_t	getMaxSemaphoreValue();

		static	int64_t	getMaxSignalQueueLength();

		static	int64_t	getMaxTimersPerProcess();

		static	int64_t	getSuggestedGroupEntryBufferSize();

		static	int64_t	getSuggestedPasswordEntryBufferSize();

		static	int64_t	getMinThreadStackSize();

		static	int64_t	getMaxThreadsPerProcess();

		static	int64_t	getThreadStackAddressOptionSupport();

		static	int64_t	getThreadStackSizeOptionSupport();

		static	int64_t	getThreadDestructorIterations();

		static	int64_t	getMaxThreadKeys();

		static	int64_t	getThreadPriorityInheritanceSupport();

		static	int64_t	getThreadPriorityProtectionSupport();

		static	int64_t	getThreadPrioritySchedulingSupport();

		static	int64_t	getThreadProcessSharedSyncSupport();

		static	int64_t	getThreadSafeFunctionsSupport();

		static	int64_t	getMaxAtExitFunctions();

		static	int64_t	getXOpenVersion();

		static	int64_t	getXCUVersion();

		static	int64_t	getCPUSetSize();

		static	int64_t	getBarriersSupport();

		static	int64_t	getClockSelectionSupport();

		static	int64_t	getFSyncSupport();

		static	int64_t	getMemoryMappedFilesSupport();

		static	int64_t	getProcessMemoryLockingSupport();

		static	int64_t	getRangeMemoryLockingSupport();

		static	int64_t	getMemoryProtectionSupport();

		static	int64_t	getMonotonicClockSupport();

		static	int64_t	getMaxPasswordLength();

		static	int64_t	getReaderWriterLocksSupport();

		static	int64_t	getRegexSupport();

		static	int64_t	getSemaphoresSupport();

		static	int64_t	getPosixShellSupport();

		static	int64_t	getSpinLocksSupport();

		static	int64_t	getSynchronizedIOSupport();

		static	int64_t	getThreadsSupport();

		static	int64_t	getTimersSupport();

		static	int64_t	getSharedMemorySupport();

		static	int64_t	getSharedMemoryObjectsSupport();

		static	int64_t	getMaxLogNameLength();

		static	int64_t	getMaxProcessID();

		static	int64_t	getMessagePassingSupport();

		static	int64_t	getPrioritizedIOSupport();

		static	int64_t	getRawSocketsSupport();

		static	int64_t	getRealtimeSignalsSupport();

		static	int64_t	getSpawnSupport();

		static	int64_t	getStackProtectionSupport();

		static	int64_t	getTimeoutsSupport();

	#include <rudiments/private/system.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
