// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/datetimeinlines.h>
#endif

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif
#include <stdio.h>
#include <sys/time.h>

#ifdef HAVE_RTC
	#include <linux/rtc.h>
	#include <sys/ioctl.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif

const datetime::tzone datetime::tzonelist[]={
	GMT,	// Greenwich Mean Time
	ECT,	// European Central Time
	EET,	// European Eastern Time
	ART,	// ???
	EAT,	// Saudi Arabia
	MET,	// Iran
	NET,	// ???
	PLT,	// West Asia
	IST,	// India
	BST,	// Central Asia
	VST,	// Bangkok
	CTT,	// China
	JST,	// Japan
	ACT,	// Central Australia
	AET,	// Eastern Australia
	SST,	// Central Pacific
	NST,	// New Zealand
	MIT,	// Samoa
	HST,	// Hawaii
	AST,	// Alaska
	PST,	// Pacific Standard Time
	PNT,	// Arizona
	MST,	// Mountain Standard Time
	CST,	// Central Standard Time
	EST,	// Eastern Standard Time
	IET,	// Indiana East
	PRT,	// Atlantic Standard Time
	CNT,	// Newfoundland
	AGT,	// Eastern South America
	BET,	// Eastern South America
	CAT	// Azores
};

const char *datetime::tzonestring[]={
	"GMT",	// Greenwich Mean Time
	"ECT",	// European Central Time
	"EET",	// European Eastern Time
	"ART",	// ???
	"EAT",	// Saudi Arabia
	"MET",	// Iran
	"NET",	// ???
	"PLT",	// West Asia
	"IST",	// India
	"BST",	// Central Asia
	"VST",	// Bangkok
	"CTT",	// China
	"JST",	// Japan
	"ACT",	// Central Australia
	"AET",	// Eastern Australia
	"SST",	// Central Pacific
	"NST",	// New Zealand
	"MIT",	// Samoa
	"HST",	// Hawaii
	"AST",	// Alaska
	"PST",	// Pacific Standard Time
	"PNT",	// Arizona
	"MST",	// Mountain Standard Time
	"CST",	// Central Standard Time
	"EST",	// Eastern Standard Time
	"IET",	// Indiana East
	"PRT",	// Atlantic Standard Time
	"CNT",	// Newfoundland
	"AGT",	// Eastern South America
	"BET",	// Eastern South America
	"CAT",	// Azores
	NULL
};

const long datetime::tzoneoffset[]={
	0,	// GMT Greenwich Mean Time	(+0 hours)
	3600,	// ECT European Central Time	(+1 hours)
	7200,	// EET European Eastern Time	(+2 hours)
	7200,	// ART ???			(+2 hours)
	10800,	// EAT Saudi Arabia		(+3 hours)
	12600,	// MET Iran			(+3.5 hours)
	14400,	// NET ???			(+4 hours)
	18000,	// PLT West Asia		(+5 hours)
	19800,	// IST India			(+5.5 hours)
	21600,	// BST Central Asia		(+6 hours)
	25200,	// VST Bangkok			(+7 hours)
	28800,	// CTT China			(+8 hours)
	32400,	// JST Japan			(+9 hours)
	34200,	// ACT Central Australia	(+9.5 hours)
	36000,	// AET Eastern Australia	(+10 hours)
	39600,	// SST Central Pacific		(+11 hours)
	43200,	// NST New Zealand		(+12 hours)
	-39600,	// MIT Samoa			(-11 hours)
	-36000,	// HST Hawaii			(-10 hours)
	-32400,	// AST Alaska			(-9 hours)
	-28800,	// PST Pacific Standard Time	(-8 hours)
	-25200,	// PNT Arizona			(-7 hours)
	-25200,	// MST Mountain Standard Time	(-7 hours)
	-21600,	// CST Central Standard Time	(-6 hours)
	-18000,	// EST Eastern Standard Time	(-5 hours)
	-18000,	// IET Indiana East		(-5 hours)
	-14400,	// PRT Atlantic Standard Time	(-4 hours)
	-12600,	// CNT Newfoundland		(-3.5 hours)
	-10800,	// AGT Eastern South America	(-3 hours)
	-10800,	// BET Eastern South America	(-3 hours)
	-3600	// CAT Azores			(-1 hours)
};

datetime::datetime() {
	timestring=NULL;
	timezone=GMT;
	epoch=0;
}

int datetime::initialize(const char *timestring) {

	initTimeString();
	this->timestring=strdup(timestring);

	// get the date
	timestruct.tm_mon=atoi(timestring)-1;
	if (timestruct.tm_mon<0) {
		timestruct.tm_mon=0;
	}
	timestruct.tm_mday=atoi(timestring+3);
	if (timestruct.tm_mday<1) {
		timestruct.tm_mday=1;
	}
	timestruct.tm_year=atoi(timestring+6)-1900;

	// get the time
	timestruct.tm_hour=atoi(timestring+11);
	if (timestruct.tm_hour<0) {
		timestruct.tm_hour=0;
	}
	timestruct.tm_min=atoi(timestring+14);
	if (timestruct.tm_min<1) {
		timestruct.tm_min=1;
	}
	timestruct.tm_sec=atoi(timestring+17);
	timestruct.tm_wday=0;
	timestruct.tm_yday=0;
	timestruct.tm_isdst=0;

	// get the time zone
	setTimeZone(determineTimeZone(timestring));

	return ((epoch=mktime(&timestruct))!=-1);
}

int datetime::initialize(time_t epoch) {

	#ifdef HAVE_LOCALTIME_R
		if (!localtime_r(&epoch,&timestruct)) {
			return 0;
		}
	#else
		tm	*lcltm=localtime(&epoch);
		if (!lcltm) {
			return 0;
		}
		memcpy((void *)&timestruct,(void *)lcltm,sizeof(tm));
	#endif

	// get the time zone
	setTimeZone(determineTimeZone(&timestruct));

	initTimeString();

	this->epoch=epoch;
	return 1;
}

int datetime::initialize(const tm *timestruct) {

	memcpy((void *)&(this->timestruct),(void *)timestruct,sizeof(tm));

	setTimeZone(determineTimeZone(timestruct));

	initTimeString();

	return ((epoch=mktime(&this->timestruct))!=-1);
}

char *datetime::getString() {
	if (!timestring) {
		timestring=new char[24];
		sprintf(timestring,"%02d/%02d/%d %02d:%02d:%02d %s",
				timestruct.tm_mon+1,
				timestruct.tm_mday,
				timestruct.tm_year+1900,
				timestruct.tm_hour,
				timestruct.tm_min,
				timestruct.tm_sec,
				getTimeZoneString());
	}
	return timestring;
}

int datetime::getSystemDateAndTime() {
	return initialize(time(NULL));
}

int datetime::setSystemDateAndTime() {
	timeval	tv;
	tv.tv_sec=epoch;
	tv.tv_usec=0;
	return !settimeofday(&tv,NULL);
}

int datetime::getHardwareDateAndTime(tzone hwtz) {
	#ifdef HAVE_RTC

		// open the rtc
		int	devrtc;
		if ((devrtc=open("/dev/rtc",O_RDONLY))==-1) {
			return 0;
		}

		// get the time from the rtc
		rtc_time rt;
		if (ioctl(devrtc,RTC_RD_TIME,&rt)==-1) {
			close(devrtc);
			return 0;
		}
		close(devrtc);

		// set the local timestruct values
		timestruct.tm_mon=rt.tm_mon;
		timestruct.tm_mday=rt.tm_mday;
		timestruct.tm_year=rt.tm_year;
		timestruct.tm_hour=rt.tm_hour;
		timestruct.tm_min=rt.tm_min;
		timestruct.tm_sec=rt.tm_sec;
		timestruct.tm_wday=rt.tm_wday;
		timestruct.tm_yday=rt.tm_yday;
		timestruct.tm_isdst=rt.tm_isdst;

		// get the epoch
		epoch=mktime(&timestruct);

		// mktime() sets the timezone name and offset to the system's
		// timezone and offset, we want to override that.
		setTimeZone(hwtz);

		initTimeString();
		return 1;
	#else
		return 0;
	#endif
}

int datetime::getAdjustedHardwareDateAndTime(tzone hwtz) {

	if (!getHardwareDateAndTime(hwtz)) {
		return 0;
	}

	// get the timezone that the system is using
	datetime	dt;
	dt.getSystemDateAndTime();

	// adjust the current timezone to the timezone of the system clock
	adjustTimeZone(dt.getTimeZone());

	return 1;
}

int datetime::setHardwareDateAndTime(tzone hwtz) {
	#ifdef HAVE_RTC

		// open the rtc
		int	devrtc;
		if ((devrtc=open("/dev/rtc",O_WRONLY))==-1) {
			return 0;
		}

		// adjust the time zone
		adjustTimeZone(hwtz);

		// set the values to be stored in the rtc
		rtc_time rt;
		rt.tm_mon=timestruct.tm_mon;
		rt.tm_mday=timestruct.tm_mday;
		rt.tm_year=timestruct.tm_year;
		rt.tm_hour=timestruct.tm_hour;
		rt.tm_min=timestruct.tm_min;
		rt.tm_sec=timestruct.tm_sec;
		rt.tm_wday=timestruct.tm_wday;
		rt.tm_yday=timestruct.tm_yday;
		rt.tm_isdst=timestruct.tm_isdst;

		// set the rtc and clean up
		int	retval=(ioctl(devrtc,RTC_SET_TIME,&rt)!=-1);
		close(devrtc);

		return retval;
	#else
		return 0;
	#endif
}

void datetime::adjustTimeZone(tzone newtz) {
	timestruct.tm_sec=timestruct.tm_sec-
				getTimeZoneOffset()+tzoneoffset[newtz];
	mktime(&timestruct);
	setTimeZone(newtz);
}

datetime::tzone datetime::determineTimeZone(const tm *tmstruct) {

	#ifdef HAS___TM_GMTOFF
		long	offset=tmstruct->__tm_gmtoff;
	#elif HAS_TM_GMTOFF
		long	offset=tmstruct->tm_gmtoff;
	#elif HAS_TM_TZADJ
		long	offset=tmstruct->tm_tzadj;
	#else
		long	offset=0;
	#endif

	for (int index=0; tzonestring[index]; index++) {
		if (tzoneoffset[index]==offset) {
			return tzonelist[index];
		}
	}
	return GMT;
}

datetime::tzone datetime::determineTimeZone(const char *tmstring) {

	if (tmstring[18] && tmstring[19] && tmstring[20]) {
		char	*tzptr=(char *)tmstring+20;
		for (int index=0; tzonestring[index]; index++) {
			if (!strcmp(tzptr,tzonestring[index])) {
				return tzonelist[index];
			}
		}
	}
	return GMT;
}
