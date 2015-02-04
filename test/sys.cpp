// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/sys.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>

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

	//sys::sync();

	stdoutput.printf("Max Cmd Line Arg Length			: %lld\n",
		(long long)sys::getMaxCommandLineArgumentLength());

	stdoutput.printf("Max Processes Per User			: %lld\n",
			(long long)sys::getMaxProcessesPerUser());

	stdoutput.printf("Max Host Name Length			: %lld\n",
			(long long)sys::getMaxHostNameLength());

	stdoutput.printf("Max Login Name Length			: %lld\n",
			(long long)sys::getMaxLoginNameLength());

	stdoutput.printf("Clock Ticks Per Second			: %lld\n",
			(long long)sys::getClockTicksPerSecond());

	stdoutput.printf("Max Open Files Per Process		: %lld\n",
		(long long)sys::getMaxOpenFilesPerProcess());

	stdoutput.printf("Page Size				: %lld\n",
			(long long)sys::getPageSize());

	stdoutput.printf("Max Open Streams Per Process		: %lld\n",
		(long long)sys::getMaxOpenStreamsPerProcess());

	stdoutput.printf("Max Symlink Loops			: %lld\n",
			(long long)sys::getMaxSymlinkLoops());

	stdoutput.printf("Max Terminal Device Name Length		: %lld\n",
		(long long)sys::getMaxTerminalDeviceNameLength());

	stdoutput.printf("Max Timezone Name Length		: %lld\n",
		(long long)sys::getMaxTimezoneNameLength());

	stdoutput.printf("Max Line Length				: %lld\n",
			(long long)sys::getMaxLineLength());

	stdoutput.printf("Physical Page Count			: %lld\n",
			(long long)sys::getPhysicalPageCount());

	stdoutput.printf("Available Physical Page Count		: %lld\n",
		(long long)sys::getAvailablePhysicalPageCount());

	stdoutput.printf("Processor Count				: %lld\n",
			(long long)sys::getProcessorCount());

	stdoutput.printf("Max Processor Count			: %lld\n",
			(long long)sys::getMaxProcessorCount());

	stdoutput.printf("Processors Online			: %lld\n",
			(long long)sys::getProcessorsOnline());

	stdoutput.printf("Max Supplemental Groups Per User	: %lld\n",
	(long long)sys::getMaxSupplementalGroupsPerUser());

	stdoutput.printf("Max Delay Timer Expirations		: %lld\n",
		(long long)sys::getMaxDelayTimerExpirations());

	stdoutput.printf("Max Realtime Signals			: %lld\n",
			(long long)sys::getMaxRealtimeSignals());

	stdoutput.printf("Max Sempahores Per Process		: %lld\n",
		(long long)sys::getMaxSemaphoresPerProcess());

	stdoutput.printf("Max Semaphore Value			: %lld\n",
			(long long)sys::getMaxSemaphoreValue());

	stdoutput.printf("Max Signal Queue Length			: %lld\n",
		(long long)sys::getMaxSignalQueueLength());

	stdoutput.printf("Max Timers Per Process			: %lld\n",
			(long long)sys::getMaxTimersPerProcess());

	stdoutput.printf("Suggested Group Entry Buffer Size	: %lld\n",
	(long long)sys::getSuggestedGroupEntryBufferSize());

	stdoutput.printf("Suggested Passwd Entry Buffer Size	: %lld\n",
	(long long)sys::getSuggestedPasswordEntryBufferSize());

	stdoutput.printf("Min Thread Stack Size			: %lld\n",
			(long long)sys::getMinThreadStackSize());

	stdoutput.printf("Max Threads Per Process			: %lld\n",
			(long long)sys::getMaxThreadsPerProcess());

	stdoutput.printf("Thread Destructor Iterations		: %lld\n",
		(long long)sys::getThreadDestructorIterations());

	stdoutput.printf("Max Thread Keys				: %lld\n",
			(long long)sys::getMaxThreadKeys());

	stdoutput.printf("Max At-Exit Functions			: %lld\n",
			(long long)sys::getMaxAtExitFunctions());

	stdoutput.printf("CPUSet Size				: %lld\n",
			(long long)sys::getCpuSetSize());

	stdoutput.printf("Max Password Length			: %lld\n",
			(long long)sys::getMaxPasswordLength());

	stdoutput.printf("Max Log Name Length			: %lld\n",
			(long long)sys::getMaxLogNameLength());

	stdoutput.printf("Max Process ID				: %lld\n",
			(long long)sys::getMaxProcessId());
}
