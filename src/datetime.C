// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#ifndef ENABLE_INLINES
	#define inline
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

datetime::datetime() {
	epoch=time(NULL);
	memcpy((void *)&timestruct,(void *)localtime(&epoch),sizeof(struct tm));
	timestring=NULL;
}

datetime::datetime(const char *timestring) {

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

	epoch=mktime(&timestruct);
}

datetime::datetime(const tm *timestruct) {
	memcpy((void *)&(this->timestruct),(void *)timestruct,
						sizeof(struct tm));
	epoch=mktime(&this->timestruct);
	timestring=NULL;
}

datetime::datetime(const time_t *epoch) {
	memcpy((void *)&timestruct,(void *)localtime(epoch),
						sizeof(struct tm));
	this->epoch=*epoch;
	timestring=NULL;
}

char *datetime::getString() {
	if (!timestring) {
		timestring=new char[20];
		sprintf(timestring,"%02d/%02d/%d %02d:%02d:%02d",
				timestruct.tm_mon+1,
				timestruct.tm_mday,
				timestruct.tm_year+1900,
				timestruct.tm_hour,
				timestruct.tm_min,
				timestruct.tm_sec);
	}
	return timestring;
}

int datetime::setSystemDateAndTime() {
	timeval	tv;
	tv.tv_sec=epoch;
	tv.tv_usec=0;
	return !settimeofday(&tv,NULL);
}

int datetime::getHardwareDateAndTime() {
	#ifdef HAVE_RTC
		int	devrtc;
		if ((devrtc=open("/dev/rtc",O_RDWR))==-1) {
			return 0;
		}
		rtc_time rt;
		if (ioctl(devrtc,RTC_RD_TIME,&rt)==-1) {
			close(devrtc);
			return 0;
		}
		timestruct.tm_mon=rt.tm_mon;
		timestruct.tm_mday=rt.tm_mday;
		timestruct.tm_year=rt.tm_year;
		timestruct.tm_hour=rt.tm_hour;
		timestruct.tm_min=rt.tm_min;
		timestruct.tm_sec=rt.tm_sec;
		timestruct.tm_wday=rt.tm_wday;
		timestruct.tm_yday=rt.tm_yday;
		timestruct.tm_isdst=rt.tm_isdst;
		close(devrtc);
		return 1;
	#else
		return 0;
	#endif
}

int datetime::setHardwareDateAndTime() {
	#ifdef HAVE_RTC
		int	devrtc;
		if ((devrtc=open("/dev/rtc",O_RDWR))==-1) {
			return 0;
		}
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
		int	retval=ioctl(devrtc,RTC_RD_TIME,&rt)!=-1;
		close(devrtc);
		return retval;
	#else
		return 0;
	#endif
}
