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

		static	bool	getLoadAverages(double *oneminuteaverage,
						double *fiveminuteaverage,
						double *fifteenminuteaverage);

		static	void	sync();

		static	bool	halt();

		static	bool	shutDown();

		static	bool	reboot();

		static	int64_t	getMaxCommandLineArgumentLength();

		static	int64_t	getMaxProcessesPerUser();

		static	int64_t	getMaxHostNameLength();

		static	int64_t	getMaxLoginNameLength();

		static	int64_t	getClockTicksPerSecond();

		static	int64_t	getMaxOpenFilesPerProcess();

		static	int32_t	getPageSize();

		static	int64_t	getMaxOpenStreamsPerProcess();

		static	int64_t	getMaxSymlinkLoops();

		static	int64_t	getMaxTerminalDeviceNameLength();

		static	int64_t	getMaxTimezoneNameLength();

		static	int64_t	getMaxLineLength();

		static	int64_t	getPhysicalPageCount();

		static	int64_t	getAvailablePhysicalPageCount();

		static	int64_t	getProcessorCount();

		static	int64_t	getMaxProcessorCount();

		static	int64_t	getProcessorsOnline();

		static	int64_t	getMaxIOVectorElements();

		static	int64_t	getMaxSupplementalGroupsPerUser();

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

		static	int64_t	getThreadDestructorIterations();

		static	int64_t	getMaxThreadKeys();

		static	int64_t	getMaxAtExitFunctions();

		static	int64_t	getCPUSetSize();

		static	int64_t	getMaxPasswordLength();

		static	int64_t	getMaxLogNameLength();

		static	int64_t	getMaxProcessID();

		static	int64_t	sysConf(int32_t name);

	#include <rudiments/private/system.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
