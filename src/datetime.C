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
