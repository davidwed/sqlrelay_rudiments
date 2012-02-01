// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/timezonefile.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, char **argv) {

	// parse the timezone file specified in the first argument
	timezonefile	tz;
	if (!tz.parseFile(argv[1])) {
		printf("usage: tzn file\n");
		exit(1);
	}

	// print using built-in method
	tz.print();
	printf("\n\n");

	// print manually
	printf("ttisgmtcnt: %lld\n",tz.getIsGMTCount());
	printf("ttisstdcnt: %lld\n",tz.getIsSTDCount());
	printf("leapcnt: %lld\n",tz.getLeapCount());
	printf("timecnt: %lld\n",tz.getTimeCount());
	printf("typecnt: %lld\n",tz.getTypeCount());
	printf("charcnt: %lld\n",tz.getCharacterCount());
	uint64_t	i;
	for (i=0; i<tz.getTimeCount(); i++) {
		printf("transitiontime[%lld]: %lld\n",
					i,tz.getTransitionTime(i));
	}
	for (i=0; i<tz.getTimeCount(); i++) {
		printf("localtime[%lld]: %d\n",i,tz.getLocalTimes(i));
	}
	for (i=0; i<tz.getTypeCount(); i++) {
		printf("ttinfo[%lld] {\n",i);
		printf("	tt_gmtoff: %lld\n",
					tz.getTimeTypeInfo(i)->tt_gmtoff);
		printf("	tt_isdst: %d\n",
					tz.getTimeTypeInfo(i)->tt_isdst);
		printf("	tt_abbrind: %d\n",
					tz.getTimeTypeInfo(i)->tt_abbrind);
		printf("}\n");
	}
	printf("rawtimezonestring: ");
	for (i=0; i<tz.getCharacterCount(); i++) {
		if (tz.getRawTimeZoneString()[i]==(char)NULL) {
			printf(" ");
		}
		printf("%c",tz.getRawTimeZoneString()[i]);
	}
	printf("\n");
	for (i=0; i<tz.getLeapCount(); i++) {
		printf("leapsecondtime[%lld]: %lld\n",i,
					tz.getLeapSecondTime(i));
		printf("totalleapseconds[%lld]: %lld\n",i,
					tz.getTotalLeapSeconds(i));
	}
	for (uint64_t counter=0; counter<tz.getTypeCount(); counter++) {
		printf("timezonestrings[%lld]=%s\n",counter,
					tz.getTimeZoneString(counter));
	}
	for (i=0; i<tz.getIsSTDCount(); i++) {
		printf("transstdwall[%lld]: %d\n",i,
					tz.getStandardOrWallIndicator(i));
	}
	for (i=0; i<tz.getIsGMTCount(); i++) {
		printf("transutclocal[%lld]: %d\n",i,
					tz.getUTCOrLocalIndicator(i));
	}
}
