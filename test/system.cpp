// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/system.h>
#include <rudiments/charstring.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	char	*name=rudiments::system::getOperatingSystemName();
	printf("OS Name		: %s\n",name);
	delete[] name;

	char	*release=rudiments::system::getOperatingSystemRelease();
	printf("OS Release	: %s\n",release);
	delete[] release;

	char	*version=rudiments::system::getOperatingSystemVersion();
	printf("OS Version	: %s\n",version);
	delete[] version;

	char	*arch=rudiments::system::getOperatingSystemArchitecture();
	printf("OS Arch		: %s\n",arch);
	delete[] arch;

	double	onemin;
	double	fivemin;
	double	fifteenmin;
	rudiments::system::getLoadAverages(&onemin,&fivemin,&fifteenmin);
	printf("Load Averages	: %0.2f  %0.2f  %0.2f\n",
				onemin,fivemin,fifteenmin);

	char	*hostname=rudiments::system::getHostName();
	printf("Host Name	: %s\n",hostname);
	delete[] hostname;

	printf("Max Cmd Line Arg Length			: %lld\n",
		(long long)rudiments::system::getMaxCommandLineArgumentLength());

	printf("Max Processes Per User			: %lld\n",
			(long long)rudiments::system::getMaxProcessesPerUser());

	printf("Max Host Name Length			: %lld\n",
			(long long)rudiments::system::getMaxHostNameLength());

	printf("Max Login Name Length			: %lld\n",
			(long long)rudiments::system::getMaxLoginNameLength());

	printf("Clock Ticks Per Second			: %lld\n",
			(long long)rudiments::system::getClockTicksPerSecond());

	printf("Max Open Files Per Process		: %lld\n",
		(long long)rudiments::system::getMaxOpenFilesPerProcess());

	printf("Page Size				: %lld\n",
			(long long)rudiments::system::getPageSize());

	printf("Max Open Streams Per Process		: %lld\n",
		(long long)rudiments::system::getMaxOpenStreamsPerProcess());

	printf("Max Symlink Loops			: %lld\n",
			(long long)rudiments::system::getMaxSymlinkLoops());

	printf("Max Terminal Device Name Length		: %lld\n",
		(long long)rudiments::system::getMaxTerminalDeviceNameLength());

	printf("Max Timezone Name Length		: %lld\n",
		(long long)rudiments::system::getMaxTimezoneNameLength());

	printf("Max Line Length				: %lld\n",
			(long long)rudiments::system::getMaxLineLength());

	printf("Physical Page Count			: %lld\n",
			(long long)rudiments::system::getPhysicalPageCount());

	printf("Available Physical Page Count		: %lld\n",
		(long long)rudiments::system::getAvailablePhysicalPageCount());

	printf("Processor Count				: %lld\n",
			(long long)rudiments::system::getProcessorCount());

	printf("Max Processor Count			: %lld\n",
			(long long)rudiments::system::getMaxProcessorCount());

	printf("Processors Online			: %lld\n",
			(long long)rudiments::system::getProcessorsOnline());

	printf("Max I/O Vector Elements			: %lld\n",
			(long long)rudiments::system::getMaxIOVectorElements());

	printf("Max Supplemental Groups Per User	: %lld\n",
	(long long)rudiments::system::getMaxSupplementalGroupsPerUser());

	printf("Max AIO Operations Per List		: %lld\n",
		(long long)rudiments::system::getMaxAIOOperationsPerList());

	printf("Max Outstanding AIO Operations		: %lld\n",
		(long long)rudiments::system::getMaxOutstandingAIOOperations());

	printf("Max AIO Priority Delta			: %lld\n",
			(long long)rudiments::system::getMaxAIOPriorityDelta());

	printf("Max Delay Timer Expirations		: %lld\n",
		(long long)rudiments::system::getMaxDelayTimerExpirations());

	printf("Max Message Queues Per Process		: %lld\n",
		(long long)rudiments::system::getMaxMessageQueuesPerProcess());

	printf("Max Message Priorities			: %lld\n",
		(long long)rudiments::system::getMaxMessagePriorities());

	printf("Max Realtime Signals			: %lld\n",
			(long long)rudiments::system::getMaxRealtimeSignals());

	printf("Max Sempahores Per Process		: %lld\n",
		(long long)rudiments::system::getMaxSemaphoresPerProcess());

	printf("Max Semaphore Value			: %lld\n",
			(long long)rudiments::system::getMaxSemaphoreValue());

	printf("Max Signal Queue Length			: %lld\n",
		(long long)rudiments::system::getMaxSignalQueueLength());

	printf("Max Timers Per Process			: %lld\n",
			(long long)rudiments::system::getMaxTimersPerProcess());

	printf("Suggested Group Entry Buffer Size	: %lld\n",
	(long long)rudiments::system::getSuggestedGroupEntryBufferSize());

	printf("Suggested Passwd Entry Buffer Size	: %lld\n",
	(long long)rudiments::system::getSuggestedPasswordEntryBufferSize());

	printf("Min Thread Stack Size			: %lld\n",
			(long long)rudiments::system::getMinThreadStackSize());

	printf("Max Threads Per Process			: %lld\n",
			(long long)rudiments::system::getMaxThreadsPerProcess());

	printf("Thread Destructor Iterations		: %lld\n",
		(long long)rudiments::system::getThreadDestructorIterations());

	printf("Max Thread Keys				: %lld\n",
			(long long)rudiments::system::getMaxThreadKeys());

	printf("Max At-Exit Functions			: %lld\n",
			(long long)rudiments::system::getMaxAtExitFunctions());

	printf("CPUSet Size				: %lld\n",
			(long long)rudiments::system::getCPUSetSize());

	printf("Max Password Length			: %lld\n",
			(long long)rudiments::system::getMaxPasswordLength());

	printf("Max Log Name Length			: %lld\n",
			(long long)rudiments::system::getMaxLogNameLength());

	printf("Max Process ID				: %lld\n",
			(long long)rudiments::system::getMaxProcessID());
}
