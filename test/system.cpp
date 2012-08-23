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
}
