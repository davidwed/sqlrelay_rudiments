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

	printf("Max Open Streams Per Process		: %lld\n",
			(long long)system::getMaxOpenStreamsPerProcess());

	printf("Max Symlink Loops			: %lld\n",
			(long long)system::getMaxSymlinkLoops());

	printf("Max Terminal Device Name Length		: %lld\n",
			(long long)system::getMaxTerminalDeviceNameLength());

	printf("Max Timezone Name Length		: %lld\n",
			(long long)system::getMaxTimezoneNameLength());

	printf("Max Line Length				: %lld\n",
			(long long)system::getMaxLineLength());

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

	printf("Thread Destructor Iterations		: %lld\n",
			(long long)system::getThreadDestructorIterations());

	printf("Max Thread Keys				: %lld\n",
			(long long)system::getMaxThreadKeys());

	printf("Max At-Exit Functions			: %lld\n",
			(long long)system::getMaxAtExitFunctions());

	printf("CPUSet Size				: %lld\n",
			(long long)system::getCPUSetSize());

	printf("Max Password Length			: %lld\n",
			(long long)system::getMaxPasswordLength());

	printf("Max Log Name Length			: %lld\n",
			(long long)system::getMaxLogNameLength());

	printf("Max Process ID				: %lld\n",
			(long long)system::getMaxProcessID());
}
