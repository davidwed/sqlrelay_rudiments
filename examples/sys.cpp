#include <rudiments/sys.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

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

	stdoutput.printf("Max Cmd Line Arg Length"
			"			: %lld\n",
			sys::getMaxCommandLineArgumentLength());

	stdoutput.printf("Max Processes Per User"
			"			: %lld\n",
			sys::getMaxProcessesPerUser());

	stdoutput.printf("Max Host Name Length"
			"			: %lld\n",
			sys::getMaxHostNameLength());

	stdoutput.printf("Max Login Name Length"
			"			: %lld\n",
			sys::getMaxLoginNameLength());

	stdoutput.printf("Clock Ticks Per Second"
			"			: %lld\n",
			sys::getClockTicksPerSecond());

	stdoutput.printf("Max Open Files Per Process"
			"		: %lld\n",
			sys::getMaxOpenFilesPerProcess());

	stdoutput.printf("Page Size"
			"				: %lld\n",
			sys::getPageSize());

	stdoutput.printf("Allocation Granularity"
			"			: %lld\n",
			sys::getAllocationGranularity());

	stdoutput.printf("Max Open Streams Per Process"
			"		: %lld\n",
			sys::getMaxOpenStreamsPerProcess());

	stdoutput.printf("Max Symlink Loops"
			"			: %lld\n",
			sys::getMaxSymlinkLoops());

	stdoutput.printf("Max Terminal Device Name Length"
			"		: %lld\n",
			sys::getMaxTerminalDeviceNameLength());

	stdoutput.printf("Max Timezone Name Length"
			"		: %lld\n",
			sys::getMaxTimezoneNameLength());

	stdoutput.printf("Max Line Length"
			"				: %lld\n",
			sys::getMaxLineLength());

	stdoutput.printf("Physical Page Count"
			"			: %lld\n",
			sys::getPhysicalPageCount());

	stdoutput.printf("Available Physical Page Count"
			"		: %lld\n",
			sys::getAvailablePhysicalPageCount());

	stdoutput.printf("Processor Count"
			"				: %lld\n",
			sys::getProcessorCount());

	stdoutput.printf("Max Processor Count"
			"			: %lld\n",
			sys::getMaxProcessorCount());

	stdoutput.printf("Processors Online"
			"			: %lld\n",
			sys::getProcessorsOnline());

	stdoutput.printf("Max Supplemental Groups Per User"
			"	: %lld\n",
			sys::getMaxSupplementalGroupsPerUser());

	stdoutput.printf("Max Delay Timer Expirations"
			"		: %lld\n",
			sys::getMaxDelayTimerExpirations());

	stdoutput.printf("Max Realtime Signals"
			"			: %lld\n",
			sys::getMaxRealtimeSignals());

	stdoutput.printf("Max Sempahores Per Process"
			"		: %lld\n",
			sys::getMaxSemaphoresPerProcess());

	stdoutput.printf("Max Semaphore Value"
			"			: %lld\n",
			sys::getMaxSemaphoreValue());

	stdoutput.printf("Max Signal Queue Length"
			"			: %lld\n",
			sys::getMaxSignalQueueLength());

	stdoutput.printf("Max Timers Per Process"
			"			: %lld\n",
			sys::getMaxTimersPerProcess());

	stdoutput.printf("Suggested Group Entry Buffer Size"
			"	: %lld\n",
			sys::getSuggestedGroupEntryBufferSize());

	stdoutput.printf("Suggested Passwd Entry Buffer Size"
			"	: %lld\n",
			sys::getSuggestedPasswordEntryBufferSize());

	stdoutput.printf("Min Thread Stack Size"
			"			: %lld\n",
			sys::getMinThreadStackSize());

	stdoutput.printf("Max Threads Per Process"
			"			: %lld\n",
			sys::getMaxThreadsPerProcess());

	stdoutput.printf("Thread Destructor Iterations"
			"		: %lld\n",
			sys::getThreadDestructorIterations());

	stdoutput.printf("Max Thread Keys"
			"				: %lld\n",
			sys::getMaxThreadKeys());

	stdoutput.printf("Max At-Exit Functions"
			"			: %lld\n",
			sys::getMaxAtExitFunctions());

	stdoutput.printf("CPUSet Size"
			"				: %lld\n",
			sys::getCpuSetSize());

	stdoutput.printf("Max Password Length"
			"			: %lld\n",
			sys::getMaxPasswordLength());

	stdoutput.printf("Max Log Name Length"
			"			: %lld\n",
			sys::getMaxLogNameLength());

	stdoutput.printf("Max Process ID"
			"				: %lld\n",
			sys::getMaxProcessId());

	stdoutput.printf("Directory Separator"
			"			: %c\n",
			sys::getDirectorySeparator());
}
