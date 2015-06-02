// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/sys.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>

#ifdef RUDIMENTS_HAVE_LONG_LONG
	#define LONG_LONG long long
#else
	#define LONG_LONG long
#endif

int main(int argc, const char **argv) {

	char	*osname=sys::getOperatingSystemName();
	stdoutput.printf("OS Name		: %s\n",osname);
	delete[] osname;

	char	*release=sys::getOperatingSystemRelease();
	stdoutput.printf("OS Release	: %s\n",release);
	delete[] release;

	char	*version=sys::getOperatingSystemVersion();
	stdoutput.printf("OS Version	: %s\n",version);
	delete[] version;

	char	*arch=sys::getOperatingSystemArchitecture();
	stdoutput.printf("OS Arch		: %s\n",arch);
	delete[] arch;

	char	*hostname=sys::getHostName();
	stdoutput.printf("Host Name	: %s\n",hostname);
	delete[] hostname;

	double	onemin;
	double	fivemin;
	double	fifteenmin;
	sys::getLoadAverages(&onemin,&fivemin,&fifteenmin);
	stdoutput.printf("Load Averages	: %0.2f  %0.2f  %0.2f\n",
					onemin,fivemin,fifteenmin);

	sys::sync();

	stdoutput.printf("Max Cmd Line Arg Length			: %lld\n",
		(LONG_LONG)sys::getMaxCommandLineArgumentLength());

	stdoutput.printf("Max Processes Per User			: %lld\n",
			(LONG_LONG)sys::getMaxProcessesPerUser());

	stdoutput.printf("Max Host Name Length			: %lld\n",
			(LONG_LONG)sys::getMaxHostNameLength());

	stdoutput.printf("Max Login Name Length			: %lld\n",
			(LONG_LONG)sys::getMaxLoginNameLength());

	stdoutput.printf("Clock Ticks Per Second			: %lld\n",
			(LONG_LONG)sys::getClockTicksPerSecond());

	stdoutput.printf("Max Open Files Per Process		: %lld\n",
		(LONG_LONG)sys::getMaxOpenFilesPerProcess());

	stdoutput.printf("Page Size				: %lld\n",
			(LONG_LONG)sys::getPageSize());

	stdoutput.printf("Max Open Streams Per Process		: %lld\n",
		(LONG_LONG)sys::getMaxOpenStreamsPerProcess());

	stdoutput.printf("Max Symlink Loops			: %lld\n",
			(LONG_LONG)sys::getMaxSymlinkLoops());

	stdoutput.printf("Max Terminal Device Name Length		: %lld\n",
		(LONG_LONG)sys::getMaxTerminalDeviceNameLength());

	stdoutput.printf("Max Timezone Name Length		: %lld\n",
		(LONG_LONG)sys::getMaxTimezoneNameLength());

	stdoutput.printf("Max Line Length				: %lld\n",
			(LONG_LONG)sys::getMaxLineLength());

	stdoutput.printf("Physical Page Count			: %lld\n",
			(LONG_LONG)sys::getPhysicalPageCount());

	stdoutput.printf("Available Physical Page Count		: %lld\n",
		(LONG_LONG)sys::getAvailablePhysicalPageCount());

	stdoutput.printf("Processor Count				: %lld\n",
			(LONG_LONG)sys::getProcessorCount());

	stdoutput.printf("Max Processor Count			: %lld\n",
			(LONG_LONG)sys::getMaxProcessorCount());

	stdoutput.printf("Processors Online			: %lld\n",
			(LONG_LONG)sys::getProcessorsOnline());

	stdoutput.printf("Max Supplemental Groups Per User	: %lld\n",
	(LONG_LONG)sys::getMaxSupplementalGroupsPerUser());

	stdoutput.printf("Max Delay Timer Expirations		: %lld\n",
		(LONG_LONG)sys::getMaxDelayTimerExpirations());

	stdoutput.printf("Max Realtime Signals			: %lld\n",
			(LONG_LONG)sys::getMaxRealtimeSignals());

	stdoutput.printf("Max Sempahores Per Process		: %lld\n",
		(LONG_LONG)sys::getMaxSemaphoresPerProcess());

	stdoutput.printf("Max Semaphore Value			: %lld\n",
			(LONG_LONG)sys::getMaxSemaphoreValue());

	stdoutput.printf("Max Signal Queue Length			: %lld\n",
		(LONG_LONG)sys::getMaxSignalQueueLength());

	stdoutput.printf("Max Timers Per Process			: %lld\n",
			(LONG_LONG)sys::getMaxTimersPerProcess());

	stdoutput.printf("Suggested Group Entry Buffer Size	: %lld\n",
	(LONG_LONG)sys::getSuggestedGroupEntryBufferSize());

	stdoutput.printf("Suggested Passwd Entry Buffer Size	: %lld\n",
	(LONG_LONG)sys::getSuggestedPasswordEntryBufferSize());

	stdoutput.printf("Min Thread Stack Size			: %lld\n",
			(LONG_LONG)sys::getMinThreadStackSize());

	stdoutput.printf("Max Threads Per Process			: %lld\n",
			(LONG_LONG)sys::getMaxThreadsPerProcess());

	stdoutput.printf("Thread Destructor Iterations		: %lld\n",
		(LONG_LONG)sys::getThreadDestructorIterations());

	stdoutput.printf("Max Thread Keys				: %lld\n",
			(LONG_LONG)sys::getMaxThreadKeys());

	stdoutput.printf("Max At-Exit Functions			: %lld\n",
			(LONG_LONG)sys::getMaxAtExitFunctions());

	stdoutput.printf("CPUSet Size				: %lld\n",
			(LONG_LONG)sys::getCpuSetSize());

	stdoutput.printf("Max Password Length			: %lld\n",
			(LONG_LONG)sys::getMaxPasswordLength());

	stdoutput.printf("Max Log Name Length			: %lld\n",
			(LONG_LONG)sys::getMaxLogNameLength());

	stdoutput.printf("Max Process ID				: %lld\n",
			(LONG_LONG)sys::getMaxProcessId());
}
