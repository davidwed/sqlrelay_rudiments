// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/timezonefile.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include <rudiments/null.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, char **argv) {

	// parse the timezone file specified in the first argument
	timezonefile	tz;
	if (!tz.parseFile(argv[1])) {
		stdoutput.printf("usage: tzn file\n");
		process::exit(1);
	}

	// print using built-in method
	tz.print();
	stdoutput.printf("\n\n");

	// print manually
	stdoutput.printf("ttisgmtcnt: %lld\n",tz.getIsGMTCount());
	stdoutput.printf("ttisstdcnt: %lld\n",tz.getIsSTDCount());
	stdoutput.printf("leapcnt: %lld\n",tz.getLeapCount());
	stdoutput.printf("timecnt: %lld\n",tz.getTimeCount());
	stdoutput.printf("typecnt: %lld\n",tz.getTypeCount());
	stdoutput.printf("charcnt: %lld\n",tz.getCharacterCount());
	uint64_t	i;
	for (i=0; i<tz.getTimeCount(); i++) {
		stdoutput.printf("transitiontime[%lld]: %lld\n",
					i,tz.getTransitionTime(i));
	}
	for (i=0; i<tz.getTimeCount(); i++) {
		stdoutput.printf("localtime[%lld]: %d\n",i,tz.getLocalTimes(i));
	}
	for (i=0; i<tz.getTypeCount(); i++) {
		stdoutput.printf("ttinfo[%lld] {\n",i);
		stdoutput.printf("	tt_gmtoff: %lld\n",
					tz.getTimeTypeInfo(i)->tt_gmtoff);
		stdoutput.printf("	tt_isdst: %d\n",
					tz.getTimeTypeInfo(i)->tt_isdst);
		stdoutput.printf("	tt_abbrind: %d\n",
					tz.getTimeTypeInfo(i)->tt_abbrind);
		stdoutput.printf("}\n");
	}
	stdoutput.printf("rawtimezonestring: ");
	for (i=0; i<tz.getCharacterCount(); i++) {
		if (tz.getRawTimeZoneString()[i]==(char)NULL) {
			stdoutput.printf(" ");
		}
		stdoutput.printf("%c",tz.getRawTimeZoneString()[i]);
	}
	stdoutput.printf("\n");
	for (i=0; i<tz.getLeapCount(); i++) {
		stdoutput.printf("leapsecondtime[%lld]: %lld\n",i,
					tz.getLeapSecondTime(i));
		stdoutput.printf("totalleapseconds[%lld]: %lld\n",i,
					tz.getTotalLeapSeconds(i));
	}
	for (uint64_t counter=0; counter<tz.getTypeCount(); counter++) {
		stdoutput.printf("timezonestrings[%lld]=%s\n",counter,
					tz.getTimeZoneString(counter));
	}
	for (i=0; i<tz.getIsSTDCount(); i++) {
		stdoutput.printf("transstdwall[%lld]: %d\n",i,
					tz.getStandardOrWallIndicator(i));
	}
	for (i=0; i<tz.getIsGMTCount(); i++) {
		stdoutput.printf("transutclocal[%lld]: %d\n",i,
					tz.getUTCOrLocalIndicator(i));
	}
}
