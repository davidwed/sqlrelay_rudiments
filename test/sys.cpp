// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/sys.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include "test.cpp"

#ifdef RUDIMENTS_HAVE_LONG_LONG
	#define LONG_LONG long long
#else
	#define LONG_LONG long
#endif

int main(int argc, const char **argv) {

	header("sys");

	char	*osname=sys::getOperatingSystemName();
	char	*release=sys::getOperatingSystemRelease();
	char	*version=sys::getOperatingSystemVersion();
	char	*arch=sys::getOperatingSystemArchitecture();
	char	*hostname=sys::getHostName();

#if 0
	stdoutput.printf("OS Name	: %s\n",osname);
	stdoutput.printf("OS Release	: %s\n",release);
	stdoutput.printf("OS Version	: %s\n",version);
	stdoutput.printf("OS Arch	: %s\n",arch);
	stdoutput.printf("Host Name	: %s\n",hostname);

	double	onemin;
	double	fivemin;
	double	fifteenmin;
	sys::getLoadAverages(&onemin,&fivemin,&fifteenmin);
	stdoutput.printf("Load Averages	: %0.2f  %0.2f  %0.2f\n",
					onemin,fivemin,fifteenmin);

	stdoutput.printf("Max Cmd Line Arg Length"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxCommandLineArgumentLength());

	stdoutput.printf("Max Processes Per User"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxProcessesPerUser());

	stdoutput.printf("Max Host Name Length"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxHostNameLength());

	stdoutput.printf("Max Login Name Length"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxLoginNameLength());

	stdoutput.printf("Clock Ticks Per Second"
			"			: %lld\n",
			(LONG_LONG)sys::getClockTicksPerSecond());

	stdoutput.printf("Max Open Files Per Process"
			"		: %lld\n",
			(LONG_LONG)sys::getMaxOpenFilesPerProcess());

	stdoutput.printf("Page Size"
			"				: %lld\n",
			(LONG_LONG)sys::getPageSize());

	stdoutput.printf("Allocation Granularity"
			"			: %lld\n",
			(LONG_LONG)sys::getAllocationGranularity());

	stdoutput.printf("Max Open Streams Per Process"
			"		: %lld\n",
			(LONG_LONG)sys::getMaxOpenStreamsPerProcess());

	stdoutput.printf("Max Symlink Loops"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxSymlinkLoops());

	stdoutput.printf("Max Terminal Device Name Length"
			"		: %lld\n",
			(LONG_LONG)sys::getMaxTerminalDeviceNameLength());

	stdoutput.printf("Max Timezone Name Length"
			"		: %lld\n",
			(LONG_LONG)sys::getMaxTimezoneNameLength());

	stdoutput.printf("Max Line Length"
			"				: %lld\n",
			(LONG_LONG)sys::getMaxLineLength());

	stdoutput.printf("Physical Page Count"
			"			: %lld\n",
			(LONG_LONG)sys::getPhysicalPageCount());

	stdoutput.printf("Available Physical Page Count"
			"		: %lld\n",
			(LONG_LONG)sys::getAvailablePhysicalPageCount());

	stdoutput.printf("Processor Count"
			"				: %lld\n",
			(LONG_LONG)sys::getProcessorCount());

	stdoutput.printf("Max Processor Count"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxProcessorCount());

	stdoutput.printf("Processors Online"
			"			: %lld\n",
			(LONG_LONG)sys::getProcessorsOnline());

	stdoutput.printf("Max Supplemental Groups Per User"
			"	: %lld\n",
			(LONG_LONG)sys::getMaxSupplementalGroupsPerUser());

	stdoutput.printf("Max Delay Timer Expirations"
			"		: %lld\n",
			(LONG_LONG)sys::getMaxDelayTimerExpirations());

	stdoutput.printf("Max Realtime Signals"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxRealtimeSignals());

	stdoutput.printf("Max Sempahores Per Process"
			"		: %lld\n",
			(LONG_LONG)sys::getMaxSemaphoresPerProcess());

	stdoutput.printf("Max Semaphore Value"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxSemaphoreValue());

	stdoutput.printf("Max Signal Queue Length"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxSignalQueueLength());

	stdoutput.printf("Max Timers Per Process"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxTimersPerProcess());

	stdoutput.printf("Suggested Group Entry Buffer Size"
			"	: %lld\n",
			(LONG_LONG)sys::getSuggestedGroupEntryBufferSize());

	stdoutput.printf("Suggested Passwd Entry Buffer Size"
			"	: %lld\n",
			(LONG_LONG)sys::getSuggestedPasswordEntryBufferSize());

	stdoutput.printf("Min Thread Stack Size"
			"			: %lld\n",
			(LONG_LONG)sys::getMinThreadStackSize());

	stdoutput.printf("Max Threads Per Process"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxThreadsPerProcess());

	stdoutput.printf("Thread Destructor Iterations"
			"		: %lld\n",
			(LONG_LONG)sys::getThreadDestructorIterations());

	stdoutput.printf("Max Thread Keys"
			"				: %lld\n",
			(LONG_LONG)sys::getMaxThreadKeys());

	stdoutput.printf("Max At-Exit Functions"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxAtExitFunctions());

	stdoutput.printf("CPUSet Size"
			"				: %lld\n",
			(LONG_LONG)sys::getCpuSetSize());

	stdoutput.printf("Max Password Length"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxPasswordLength());

	stdoutput.printf("Max Log Name Length"
			"			: %lld\n",
			(LONG_LONG)sys::getMaxLogNameLength());

	stdoutput.printf("Max Process ID"
			"				: %lld\n",
			(LONG_LONG)sys::getMaxProcessId());
#endif

	// these should be non-empty/non-null
	test("OS Name",!charstring::isNullOrEmpty(osname));
	test("OS Release",!charstring::isNullOrEmpty(release));
	test("OS Version",!charstring::isNullOrEmpty(version));
	test("OS Arch",!charstring::isNullOrEmpty(arch));
	test("Host Name",!charstring::isNullOrEmpty(hostname));

	// these should either return non-zero or -1 for unsupported
	test("Max Cmd Line Arg Length",
			sys::getMaxCommandLineArgumentLength()!=0);
	test("Max Processes Per User",
			sys::getMaxProcessesPerUser()!=0);
	test("Max Host Name Length",
			sys::getMaxHostNameLength()!=0);
	test("Max Login Name Length",
			sys::getMaxLoginNameLength()!=0);
	test("Clock Ticks Per Second",
			sys::getClockTicksPerSecond()!=0);
	test("Max Open Files Per Process",
			sys::getMaxOpenFilesPerProcess()!=0);
	test("Page Size",
			sys::getPageSize()!=0);
	test("Allocation Granularity",
			sys::getAllocationGranularity()!=0);
	test("Max Open Streams Per Process",
			sys::getMaxOpenStreamsPerProcess()!=0);
	test("Max Symlink Loops",
			sys::getMaxSymlinkLoops()!=0);
	test("Max Terminal Device Name Length",
			sys::getMaxTerminalDeviceNameLength()!=0);
	test("Max Timezone Name Length",
			sys::getMaxTimezoneNameLength()!=0);
	test("Max Line Length",
			sys::getMaxLineLength()!=0);
	test("Physical Page Count",
			sys::getPhysicalPageCount()!=0);
	test("Available Physical Page Count",
			sys::getAvailablePhysicalPageCount()!=0);
	test("Processor Count",
			sys::getProcessorCount()!=0);
	test("Max Processor Count",
			sys::getMaxProcessorCount()!=0);
	test("Processors Online",
			sys::getProcessorsOnline()!=0);
	test("Max Supplemental Groups Per User",
			sys::getMaxSupplementalGroupsPerUser()!=0);
	test("Max Delay Timer Expirations",
			sys::getMaxDelayTimerExpirations()!=0);
	test("Max Realtime Signals",
			sys::getMaxRealtimeSignals()!=0);
	test("Max Sempahores Per Process",
			sys::getMaxSemaphoresPerProcess()!=0);
	test("Max Semaphore Value",
			sys::getMaxSemaphoreValue()!=0);
	test("Max Signal Queue Length",
			sys::getMaxSignalQueueLength()!=0);
	test("Max Timers Per Process",
			sys::getMaxTimersPerProcess()!=0);
	test("Suggested Group Entry Buffer Size",
			sys::getSuggestedGroupEntryBufferSize()!=0);
	test("Suggested Passwd Entry Buffer Size",
			sys::getSuggestedPasswordEntryBufferSize()!=0);
	test("Min Thread Stack Size",
			sys::getMinThreadStackSize()!=0);
	test("Max Threads Per Process",
			sys::getMaxThreadsPerProcess()!=0);
	test("Thread Destructor Iterations",
			sys::getThreadDestructorIterations()!=0);
	test("Max Thread Keys",
			sys::getMaxThreadKeys()!=0);
	test("Max At-Exit Functions",
			sys::getMaxAtExitFunctions()!=0);
	test("CPUSet Size",
			sys::getCpuSetSize()!=0);
	test("Max Password Length",
			sys::getMaxPasswordLength()!=0);
	test("Max Log Name Length",
			sys::getMaxLogNameLength()!=0);
	test("Max Process ID",
			sys::getMaxProcessId()!=0);

	// this just shouldn't crash
	sys::sync();

	// clean up
	delete[] osname;
	delete[] release;
	delete[] version;
	delete[] arch;
	delete[] hostname;

	stdoutput.printf("\n");
}
