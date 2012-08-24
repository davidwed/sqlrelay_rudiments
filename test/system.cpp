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

	printf("Max Cmd Line Arg Length			: %lld\n",
			(long long)system::getMaxCommandLineArgumentLength());

	printf("Max Processes Per User			: %lld\n",
			(long long)system::getMaxProcessesPerUser());

	printf("Max Host Name Length			: %lld\n",
			(long long)system::getMaxHostNameLength());

	printf("Max Login Name Length			: %lld\n",
			(long long)system::getMaxLoginNameLength());

	printf("Clock Ticks Per Second			: %lld\n",
			(long long)system::getClockTicksPerSecond());

	printf("Max Open Files Per Process		: %lld\n",
			(long long)system::getMaxOpenFilesPerProcess());

	printf("Page Size				: %lld\n",
			(long long)system::getPageSize());

	printf("Max Regex Dups				: %lld\n",
			(long long)system::getMaxRegexDups());

	printf("Max Open Streams Per Process		: %lld\n",
			(long long)system::getMaxOpenStreamsPerProcess());

	printf("Max Symlink Loops			: %lld\n",
			(long long)system::getMaxSymlinkLoops());

	printf("Max Terminal Device Name Length		: %lld\n",
			(long long)system::getMaxTerminalDeviceNameLength());

	printf("Max Timezone Name Length		: %lld\n",
			(long long)system::getMaxTimezoneNameLength());

	printf("Posix Version				: %lld\n",
			(long long)system::getPosixVersion());

	printf("Max Calculator Base			: %lld\n",
			(long long)system::getMaxCalculatorBase());

	printf("Max Calculator Value			: %lld\n",
			(long long)system::getMaxCalculatorValue());

	printf("Max Calculator Scale			: %lld\n",
			(long long)system::getMaxCalculatorScale());

	printf("Max Calculator String Length		: %lld\n",
			(long long)system::getMaxCalculatorStringLength());

	printf("Max Collation Weights			: %lld\n",
			(long long)system::getMaxCollationWeights());

	printf("Max Nested Expressions			: %lld\n",
			(long long)system::getMaxNestedExpressions());

	printf("Max Line Length				: %lld\n",
			(long long)system::getMaxLineLength());

	printf("Posix 2 Version				: %lld\n",
			(long long)system::getPosix2Version());

	printf("C Development Support			: %lld\n",
			(long long)system::getCDevelopmentSupport());

	printf("C Language Bindings Support		: %lld\n",
			(long long)system::getCLanguageBindingsSupport());

	printf("C Language Verson			: %lld\n",
			(long long)system::getCLanguageSupport());

	printf("Fortran Development Support		: %lld\n",
			(long long)system::getFortranDevelopmentSupport());

	printf("Fortran Runtime Support			: %lld\n",
			(long long)system::getFortranRuntimeSupport());

	printf("Locale Creation Support			: %lld\n",
			(long long)system::getLocaleCreationSupport());

	printf("Software Development Support		: %lld\n",
			(long long)system::getSoftwareDevelopmentSupport());

	printf("Physical Page Count			: %lld\n",
			(long long)system::getPhysicalPageCount());

	printf("Available Physical Page Count		: %lld\n",
			(long long)system::getAvailablePhysicalPageCount());

	printf("Processor Count				: %lld\n",
			(long long)system::getProcessorCount());

	printf("Max Processor Count			: %lld\n",
			(long long)system::getMaxProcessorCount());

	printf("Processors Online			: %lld\n",
			(long long)system::getProcessorsOnline());

	printf("Max I/O Vector Elements			: %lld\n",
			(long long)system::getMaxIOVectorElements());

	printf("Max Supplemental Groups Per User	: %lld\n",
			(long long)system::getMaxSupplementalGroupsPerUser());

	printf("Job Control Support			: %lld\n",
			(long long)system::getJobControlSupport());

	printf("Saved ID's Support			: %lld\n",
			(long long)system::getSavedIDsSupport());

	printf("Character Terminal Support		: %lld\n",
			(long long)system::getCharacterTerminalSupport());

	printf("User Portabliltiy Utilities Support	: %lld\n",
		(long long)system::getUserPortabilityUtilitiesSupport());

	printf("AIO Support				: %lld\n",
			(long long)system::getAIOSupport());

	printf("Max AIO Operations Per List		: %lld\n",
			(long long)system::getMaxAIOOperationsPerList());

	printf("Max Outstanding AIO Operations		: %lld\n",
			(long long)system::getMaxOutstandingAIOOperations());

	printf("Max AIO Priority Delta			: %lld\n",
			(long long)system::getMaxAIOPriorityDelta());

	printf("Max Delay Timer Expirations		: %lld\n",
			(long long)system::getMaxDelayTimerExpirations());

	printf("Max Message Queues Per Process		: %lld\n",
			(long long)system::getMaxMessageQueuesPerProcess());

	printf("Max Message Priorities			: %lld\n",
			(long long)system::getMaxMessagePriorities());

	printf("Max Realtime Signals			: %lld\n",
			(long long)system::getMaxRealtimeSignals());

	printf("Max Sempahores Per Process		: %lld\n",
			(long long)system::getMaxSemaphoresPerProcess());

	printf("Max Semaphore Value			: %lld\n",
			(long long)system::getMaxSemaphoreValue());

	printf("Max Signal Queue Length			: %lld\n",
			(long long)system::getMaxSignalQueueLength());

	printf("Max Timers Per Process			: %lld\n",
			(long long)system::getMaxTimersPerProcess());

	printf("Suggested Group Entry Buffer Size	: %lld\n",
			(long long)system::getSuggestedGroupEntryBufferSize());

	printf("Suggested Passwd Entry Buffer Size	: %lld\n",
		(long long)system::getSuggestedPasswordEntryBufferSize());

	printf("Min Thread Stack Size			: %lld\n",
			(long long)system::getMinThreadStackSize());

	printf("Max Threads Per Process			: %lld\n",
			(long long)system::getMaxThreadsPerProcess());

	printf("Thread Stack Address Option Support	: %lld\n",
		(long long)system::getThreadStackAddressOptionSupport());

	printf("Thread Stack Size Option Support	: %lld\n",
		(long long)system::getThreadStackSizeOptionSupport());

	printf("Thread Destructor Iterations		: %lld\n",
			(long long)system::getThreadDestructorIterations());

	printf("Max Thread Keys				: %lld\n",
			(long long)system::getMaxThreadKeys());

	printf("Thread Priority Inheritance Support	: %lld\n",
		(long long)system::getThreadPriorityInheritanceSupport());

	printf("Thread Priority Protection Support	: %lld\n",
		(long long)system::getThreadPriorityProtectionSupport());

	printf("Thread Priority Scheduling Support	: %lld\n",
		(long long)system::getThreadPrioritySchedulingSupport());

	printf("Thread Process Shared Sync Support	: %lld\n",
		(long long)system::getThreadProcessSharedSyncSupport());

	printf("Thread Safe Functions Support		: %lld\n",
		(long long)system::getThreadSafeFunctionsSupport());

	printf("Max At-Exit Functions			: %lld\n",
			(long long)system::getMaxAtExitFunctions());

	printf("XOpen Version				: %lld\n",
			(long long)system::getXOpenVersion());

	printf("XOpen XCU Version			: %lld\n",
			(long long)system::getXOpenXCUVersion());

	printf("CPUSet Size				: %lld\n",
			(long long)system::getCPUSetSize());

	printf("Barriers Support			: %lld\n",
			(long long)system::getBarriersSupport());

	printf("Clock Selection Support			: %lld\n",
			(long long)system::getClockSelectionSupport());

	printf("FSync Support 				: %lld\n",
			(long long)system::getFSyncSupport());

	printf("Memory Mapped Files Support		: %lld\n",
			(long long)system::getMemoryMappedFilesSupport());

	printf("Process Memory Locking Support		: %lld\n",
			(long long)system::getProcessMemoryLockingSupport());

	printf("Range Memory Locking Support		: %lld\n",
			(long long)system::getRangeMemoryLockingSupport());

	printf("Memory Protection Support		: %lld\n",
			(long long)system::getMemoryProtectionSupport());

	printf("Monotonic Clock Support			: %lld\n",
			(long long)system::getMonotonicClockSupport());

	printf("Max Password Length			: %lld\n",
			(long long)system::getMaxPasswordLength());

	printf("Reader-Writer Locks Support		: %lld\n",
			(long long)system::getReaderWriterLocksSupport());

	printf("Regex Support				: %lld\n",
			(long long)system::getRegexSupport());

	printf("Semaphores Support			: %lld\n",
			(long long)system::getSemaphoresSupport());

	printf("Posix Shell Support			: %lld\n",
			(long long)system::getPosixShellSupport());

	printf("Spin Locks Support			: %lld\n",
			(long long)system::getSpinLocksSupport());

	printf("Synchronized IO Support			: %lld\n",
			(long long)system::getSynchronizedIOSupport());

	printf("Threads Support				: %lld\n",
			(long long)system::getThreadsSupport());

	printf("Timers Support				: %lld\n",
			(long long)system::getTimersSupport());

	printf("XOpen Shared Memory Support		: %lld\n",
			(long long)system::getXOpenSharedMemorySupport());

	printf("Shared Memory Objects Support		: %lld\n",
			(long long)system::getSharedMemoryObjectsSupport());

	printf("Max Log Name Length			: %lld\n",
			(long long)system::getMaxLogNameLength());

	printf("Max Process ID				: %lld\n",
			(long long)system::getMaxProcessID());

	printf("Message Passing Support			: %lld\n",
			(long long)system::getMessagePassingSupport());

	printf("Prioritized IO Support			: %lld\n",
			(long long)system::getPrioritizedIOSupport());

	printf("Raw Sockets Support			: %lld\n",
			(long long)system::getRawSocketsSupport());

	printf("Realtime Signals Support		: %lld\n",
			(long long)system::getRealtimeSignalsSupport());

	printf("Spawn Support				: %lld\n",
			(long long)system::getSpawnSupport());

	printf("Stack Protection Support		: %lld\n",
			(long long)system::getStackProtectionSupport());

	printf("Timeouts Support			: %lld\n",
			(long long)system::getTimeoutsSupport());

	printf("XOpen ILP32 With 32-Bit Offset Support	: %lld\n",
		(long long)system::getXOpenILP32With32BitOffsetSupport());

	printf("XOpen ILP32 With 64-Bit Offset Support	: %lld\n",
		(long long)system::getXOpenILP32With64BitOffsetSupport());

	printf("XOpen Crypt Support			: %lld\n",
			(long long)system::getXOpenCryptSupport());

	printf("XOpen Enhanced I18N			: %lld\n",
	(long long)system::getXOpenEnhancedInternationalizationSupport());

	printf("XOpen Legacy Support			: %lld\n",
			(long long)system::getXOpenLegacySupport());

	printf("XOpen Realtime Support			: %lld\n",
			(long long)system::getXOpenRealtimeSupport());

	printf("XOpen Realtime Threads Support		: %lld\n",
			(long long)system::getXOpenRealtimeThreadsSupport());

	printf("XOpen Streams Support			: %lld\n",
			(long long)system::getXOpenStreamsSupport());

	printf("XOpen Unix Support			: %lld\n",
			(long long)system::getXOpenUnixSupport());
}
