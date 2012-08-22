// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/system.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	printf("OS Name		: %s\n",
		system::getOperatingSystemName());
	printf("OS Release	: %s\n",
		system::getOperatingSystemRelease());
	printf("OS Version	: %s\n",
		system::getOperatingSystemVersion());
	printf("OS Arch		: %s\n",
		system::getOperatingSystemArchitecture());
	printf("Page Size	: %d\n",
		system::getPageSize());
	double	onemin;
	double	fivemin;
	double	fifteenmin;
	system::getLoadAverages(&onemin,&fivemin,&fifteenmin);
	printf("Load Averages	: %0.2f  %0.2f  %0.2f\n",
				onemin,fivemin,fifteenmin);
}
