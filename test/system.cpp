// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/system.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	char	*name=rudiments::system::getOperatingSystemName();
	stdoutput.printf("OS Name		: %s\n",name);
	delete[] name;

	char	*release=rudiments::system::getOperatingSystemRelease();
	stdoutput.printf("OS Release	: %s\n",release);
	delete[] release;

	char	*version=rudiments::system::getOperatingSystemVersion();
	stdoutput.printf("OS Version	: %s\n",version);
	delete[] version;

	char	*arch=rudiments::system::getOperatingSystemArchitecture();
	stdoutput.printf("OS Arch		: %s\n",arch);
	delete[] arch;

	double	onemin;
	double	fivemin;
	double	fifteenmin;
	rudiments::system::getLoadAverages(&onemin,&fivemin,&fifteenmin);
	stdoutput.printf("Load Averages	: %0.2f  %0.2f  %0.2f\n",
					onemin,fivemin,fifteenmin);

	char	*hostname=rudiments::system::getHostName();
	stdoutput.printf("Host Name	: %s\n",hostname);
	delete[] hostname;

	stdoutput.printf("Max Cmd Line Arg Length			: %lld\n",
		(long long)rudiments::system::getMaxCommandLineArgumentLength());

	stdoutput.printf("Max Processes Per User			: %lld\n",
			(long long)rudiments::system::getMaxProcessesPerUser());

	stdoutput.printf("Max Host Name Length			: %lld\n",
			(long long)rudiments::system::getMaxHostNameLength());

	stdoutput.printf("Max Login Name Length			: %lld\n",
			(long long)rudiments::system::getMaxLoginNameLength());

	stdoutput.printf("Clock Ticks Per Second			: %lld\n",
			(long long)rudiments::system::getClockTicksPerSecond());

	stdoutput.printf("Max Open Files Per Process		: %lld\n",
		(long long)rudiments::system::getMaxOpenFilesPerProcess());

	stdoutput.printf("Page Size				: %lld\n",
			(long long)rudiments::system::getPageSize());

	stdoutput.printf("Max Open Streams Per Process		: %lld\n",
		(long long)rudiments::system::getMaxOpenStreamsPerProcess());

	stdoutput.printf("Max Symlink Loops			: %lld\n",
			(long long)rudiments::system::getMaxSymlinkLoops());

	stdoutput.printf("Max Terminal Device Name Length		: %lld\n",
		(long long)rudiments::system::getMaxTerminalDeviceNameLength());

	stdoutput.printf("Max Timezone Name Length		: %lld\n",
		(long long)rudiments::system::getMaxTimezoneNameLength());

	stdoutput.printf("Max Line Length				: %lld\n",
			(long long)rudiments::system::getMaxLineLength());

	stdoutput.printf("Physical Page Count			: %lld\n",
			(long long)rudiments::system::getPhysicalPageCount());

	stdoutput.printf("Available Physical Page Count		: %lld\n",
		(long long)rudiments::system::getAvailablePhysicalPageCount());

	stdoutput.printf("Processor Count				: %lld\n",
			(long long)rudiments::system::getProcessorCount());

	stdoutput.printf("Max Processor Count			: %lld\n",
			(long long)rudiments::system::getMaxProcessorCount());

	stdoutput.printf("Processors Online			: %lld\n",
			(long long)rudiments::system::getProcessorsOnline());

	stdoutput.printf("Max Supplemental Groups Per User	: %lld\n",
	(long long)rudiments::system::getMaxSupplementalGroupsPerUser());

	stdoutput.printf("Max Delay Timer Expirations		: %lld\n",
		(long long)rudiments::system::getMaxDelayTimerExpirations());

	stdoutput.printf("Max Realtime Signals			: %lld\n",
			(long long)rudiments::system::getMaxRealtimeSignals());

	stdoutput.printf("Max Sempahores Per Process		: %lld\n",
		(long long)rudiments::system::getMaxSemaphoresPerProcess());

	stdoutput.printf("Max Semaphore Value			: %lld\n",
			(long long)rudiments::system::getMaxSemaphoreValue());

	stdoutput.printf("Max Signal Queue Length			: %lld\n",
		(long long)rudiments::system::getMaxSignalQueueLength());

	stdoutput.printf("Max Timers Per Process			: %lld\n",
			(long long)rudiments::system::getMaxTimersPerProcess());

	stdoutput.printf("Suggested Group Entry Buffer Size	: %lld\n",
	(long long)rudiments::system::getSuggestedGroupEntryBufferSize());

	stdoutput.printf("Suggested Passwd Entry Buffer Size	: %lld\n",
	(long long)rudiments::system::getSuggestedPasswordEntryBufferSize());

	stdoutput.printf("Min Thread Stack Size			: %lld\n",
			(long long)rudiments::system::getMinThreadStackSize());

	stdoutput.printf("Max Threads Per Process			: %lld\n",
			(long long)rudiments::system::getMaxThreadsPerProcess());

	stdoutput.printf("Thread Destructor Iterations		: %lld\n",
		(long long)rudiments::system::getThreadDestructorIterations());

	stdoutput.printf("Max Thread Keys				: %lld\n",
			(long long)rudiments::system::getMaxThreadKeys());

	stdoutput.printf("Max At-Exit Functions			: %lld\n",
			(long long)rudiments::system::getMaxAtExitFunctions());

	stdoutput.printf("CPUSet Size				: %lld\n",
			(long long)rudiments::system::getCPUSetSize());

	stdoutput.printf("Max Password Length			: %lld\n",
			(long long)rudiments::system::getMaxPasswordLength());

	stdoutput.printf("Max Log Name Length			: %lld\n",
			(long long)rudiments::system::getMaxLogNameLength());

	stdoutput.printf("Max Process ID				: %lld\n",
			(long long)rudiments::system::getMaxProcessID());
}
