// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/system.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	char	*name=system::getOperatingSystemName();
	printf("OS Name		: %s\n",name);
	delete[] name;

	char	*release=system::getOperatingSystemRelease();
	printf("OS Release	: %s\n",release);
	delete[] release;

	char	*version=system::getOperatingSystemVersion();
	printf("OS Version	: %s\n",version);
	delete[] version;

	char	*arch=system::getOperatingSystemArchitecture();
	printf("OS Arch		: %s\n",arch);
	delete[] arch;

	double	onemin;
	double	fivemin;
	double	fifteenmin;
	system::getLoadAverages(&onemin,&fivemin,&fifteenmin);
	printf("Load Averages	: %0.2f  %0.2f  %0.2f\n",
				onemin,fivemin,fifteenmin);

	char	*hostname=system::getHostName();
	printf("Host Name	: %s\n",hostname);
	delete[] hostname;

	char	*nisdomainname=system::getNISDomainName();
	printf("NIS Domain Name	: %s\n",nisdomainname);
	delete[] nisdomainname;

	printf("Max Cmd Line Arg Length		: %lld\n",
			(long long)system::getMaxCommandLineArgumentLength());

	printf("Max Processes Per User		: %lld\n",
			(long long)system::getMaxProcessesPerUser());

	printf("Max Host Name Length		: %lld\n",
			(long long)system::getMaxHostNameLength());

	printf("Max Login Name Length		: %lld\n",
			(long long)system::getMaxLoginNameLength());

	printf("Clock Ticks Per Second		: %lld\n",
			(long long)system::getClockTicksPerSecond());

	printf("Max Open Files Per Process	: %lld\n",
			(long long)system::getMaxOpenFilesPerProcess());

	printf("Page Size			: %lld\n",
			(long long)system::getPageSize());

	printf("Max Regex Dups			: %lld\n",
			(long long)system::getMaxRegexDups());

	printf("Max Open Streams Per Process	: %lld\n",
			(long long)system::getMaxOpenStreamsPerProcess());

	printf("Max Symlink Loops		: %lld\n",
			(long long)system::getMaxSymlinkLoops());

	printf("Max Terminal Device Name Length	: %lld\n",
			(long long)system::getMaxTerminalDeviceNameLength());

	printf("Max Timezone Name Length	: %lld\n",
			(long long)system::getMaxTimezoneNameLength());

	printf("Posix Version			: %lld\n",
			(long long)system::getPosixVersion());

	printf("Max Calculator Base		: %lld\n",
			(long long)system::getMaxCalculatorBase());

	printf("Max Calculator Value		: %lld\n",
			(long long)system::getMaxCalculatorValue());

	printf("Max Calculator Scale		: %lld\n",
			(long long)system::getMaxCalculatorScale());

	printf("Max Calculator String Length	: %lld\n",
			(long long)system::getMaxCalculatorStringLength());

	printf("Max Collation Weights		: %lld\n",
			(long long)system::getMaxCollationWeights());

	printf("Max Nested Expressions		: %lld\n",
			(long long)system::getMaxNestedExpressions());

	printf("Max Line Length			: %lld\n",
			(long long)system::getMaxLineLength());

	printf("Posix 2 Version			: %lld\n",
			(long long)system::getPosix2Version());

	printf("C Development Supported		: %lld\n",
			(long long)system::getCDevelopmentSupported());

	printf("C LanguageBindings Supported	: %lld\n",
			(long long)system::getCLanguageBindingsSupported());

	printf("Fortran Development Supported	: %lld\n",
			(long long)system::getFortranDevelopmentSupported());

	printf("Fortran Runtime Supported	: %lld\n",
			(long long)system::getFortranRuntimeSupported());

	printf("Locale Creation Supported	: %lld\n",
			(long long)system::getLocaleCreationSupported());

	printf("Software Development Supported	: %lld\n",
			(long long)system::getSoftwareDevelopmentSupported());

	printf("Physical Page Count		: %lld\n",
			(long long)system::getPhysicalPageCount());

	printf("Available Physical Page Count	: %lld\n",
			(long long)system::getAvailablePhysicalPageCount());

	printf("Processor Count			: %lld\n",
			(long long)system::getProcessorCount());

	printf("Processors Online		: %lld\n",
			(long long)system::getProcessorsOnline());

	printf("Max I/O Vector Elements		: %lld\n",
			(long long)system::getMaxIOVectorElements());

	printf("Max Supplemental Groups Per User	: %lld\n",
			(long long)system::getMaxSupplementalGroupsPerUser());

	printf("Job Control Supported		: %lld\n",
			(long long)system::getJobControlSupported());

	printf("Saved ID's Supported		: %lld\n",
			(long long)system::getSavedIDsSupported());

	printf("Character Terminal Supported	: %lld\n",
			(long long)system::getCharacterTerminalSupported());

	printf("User Portabliltiy Utilities Supported	: %lld\n",
		(long long)system::getUserPortabilityUtilitiesSupported());

	printf("Max AIO Operations Per List	: %lld\n",
			(long long)system::getMaxAIOOperationsPerList());

	printf("Max Outstanding AIO Operations	: %lld\n",
			(long long)system::getMaxOutstandingAIOOperations());

	printf("Max AIO Priority Delta		: %lld\n",
			(long long)system::getMaxAIOPriorityDelta());

	printf("Max Delay Timer Expirations	: %lld\n",
			(long long)system::getMaxDelayTimerExpirations());

	printf("Max Message Queues Per Process	: %lld\n",
			(long long)system::getMaxMessageQueuesPerProcess());

	printf("Max Realtime Signals		: %lld\n",
			(long long)system::getMaxRealtimeSignals());

	printf("Max Sempahores Per Process	: %lld\n",
			(long long)system::getMaxSemaphoresPerProcess());

	printf("Max Semaphore Value		: %lld\n",
			(long long)system::getMaxSemaphoreValue());

	printf("Max Signal Queue Length		: %lld\n",
			(long long)system::getMaxSignalQueueLength());

	printf("Max Timers Per Process		: %lld\n",
			(long long)system::getMaxTimersPerProcess());

	printf("Suggested Group Entry Buffer Size	: %lld\n",
			(long long)system::getSuggestedGroupEntryBufferSize());

	printf("Suggested Passwd Entry Buffer Size	: %lld\n",
		(long long)system::getSuggestedPasswordEntryBufferSize());

	printf("Min Thread Stack Size		: %lld\n",
			(long long)system::getMinThreadStackSize());

	printf("Max Threads Per Process		: %lld\n",
			(long long)system::getMaxThreadsPerProcess());

	printf("Max At-Exit Functions		: %lld\n",
			(long long)system::getMaxAtExitFunctions());

	printf("XOpen Version			: %lld\n",
			(long long)system::getXOpenVersion());

	printf("XCU Version			: %lld\n",
			(long long)system::getXCUVersion());

	printf("CPUSet Size			: %lld\n",
			(long long)system::getCPUSetSize());

	printf("Barriers Version		: %lld\n",
			(long long)system::getBarriersVersion());

	printf("Clock Selection Version		: %lld\n",
			(long long)system::getClockSelectionVersion());

	printf("FSync Supported 		: %lld\n",
			(long long)system::getFSyncSupported());

	printf("Memory Mapped Files Supported	: %lld\n",
			(long long)system::getMemoryMappedFilesSupported());

	printf("Process Memory Locking Supported	: %lld\n",
			(long long)system::getProcessMemoryLockingSupported());

	printf("Range Memory Locking Supported	: %lld\n",
			(long long)system::getRangeMemoryLockingSupported());

	printf("Memory Protection Supported	: %lld\n",
			(long long)system::getMemoryProtectionSupported());

	printf("Monotonic Clock Version		: %lld\n",
			(long long)system::getMonotonicClockVersion());

	printf("Max Password Length		: %lld\n",
			(long long)system::getMaxPasswordLength());

	printf("Reader-Writer Locks Version	: %lld\n",
			(long long)system::getReaderWriterLocksVersion());

	printf("Regex Supported			: %lld\n",
			(long long)system::getRegexSupported());

	printf("Semaphores Version		: %lld\n",
			(long long)system::getSemaphoresVersion());

	printf("Posix Shell Supported		: %lld\n",
			(long long)system::getPosixShellSupported());

	printf("Spin Locks Version		: %lld\n",
			(long long)system::getSpinLocksVersion());

	printf("Synchronized IO Supported	: %lld\n",
			(long long)system::getSynchronizedIOSupported());

	printf("Threads Version			: %lld\n",
			(long long)system::getThreadsVersion());

	printf("Timers Version			: %lld\n",
			(long long)system::getTimersVersion());

	printf("Shared Memory Supported		: %lld\n",
			(long long)system::getSharedMemorySupported());
}
