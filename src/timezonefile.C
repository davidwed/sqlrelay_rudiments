// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/timezonefile.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class timezonefileprivate {
	friend class timezonefile;
	private:
		long		_ttisgmtcnt;
		long		_ttisstdcnt;
		long		_leapcnt;
		long		_timecnt;
		long		_typecnt;
		long		_charcnt;
		long		*_transitiontime;
		unsigned char	*_localtime;
		ttinfo		*_ti;
		long		*_leapsecondtime;
		long		*_totalleapseconds;
		unsigned char	*_transstdwall;
		unsigned char	*_transutclocal;
		unsigned char	*_rawtimezonestring;
		unsigned char	**_timezonestrings;
};

timezonefile::timezonefile() {
	pvt=new timezonefileprivate;
	pvt->_ttisgmtcnt=0;
	pvt->_ttisstdcnt=0;
	pvt->_leapcnt=0;
	pvt->_timecnt=0;
	pvt->_typecnt=0;
	pvt->_charcnt=0;
	pvt->_transitiontime=0;
	pvt->_localtime=NULL;
	pvt->_ti=NULL;
	pvt->_leapsecondtime=NULL;
	pvt->_totalleapseconds=NULL;
	pvt->_transstdwall=NULL;
	pvt->_transutclocal=NULL;
	pvt->_rawtimezonestring=NULL;
	pvt->_timezonestrings=NULL;
}

timezonefile::timezonefile(const timezonefile &t) {
	timezonefileClone(t);
}

timezonefile &timezonefile::operator=(const timezonefile &t) {
	if (this!=&t) {
		timezonefileClean();
		timezonefileClone(t);
	}
	return *this;
}

void timezonefile::timezonefileClean() {
	delete[] pvt->_timezonestrings;
	delete[] pvt->_rawtimezonestring;
	delete[] pvt->_transutclocal;
	delete[] pvt->_transstdwall;
	delete[] pvt->_leapsecondtime;
	delete[] pvt->_totalleapseconds;
	delete[] pvt->_transitiontime;
	delete[] pvt->_localtime;
	delete[] pvt->_ti;
	delete pvt;
}

void timezonefile::timezonefileClone(const timezonefile &t) {
	pvt=new timezonefileprivate;
	pvt->_ttisgmtcnt=t.pvt->_ttisgmtcnt;
	pvt->_ttisstdcnt=t.pvt->_ttisstdcnt;
	pvt->_leapcnt=t.pvt->_leapcnt;
	pvt->_timecnt=t.pvt->_timecnt;
	pvt->_typecnt=t.pvt->_typecnt;
	pvt->_charcnt=t.pvt->_charcnt;
	pvt->_transitiontime=new long[pvt->_timecnt];
	pvt->_localtime=new unsigned char[pvt->_timecnt];
	for (long i=0; i<pvt->_timecnt; i++) {
		pvt->_transitiontime[i]=t.pvt->_transitiontime[i];
		pvt->_localtime[i]=t.pvt->_localtime[i];
	}
	pvt->_ti=new ttinfo[pvt->_typecnt];
	for (long i=0; i<pvt->_timecnt; i++) {
		pvt->_ti[i]=t.pvt->_ti[i];
	}
	pvt->_leapsecondtime=new long[pvt->_leapcnt];
	pvt->_totalleapseconds=new long[pvt->_leapcnt];
	for (long i=0; i<pvt->_leapcnt; i++) {
		pvt->_leapsecondtime[i]=t.pvt->_leapsecondtime[i];
		pvt->_totalleapseconds[i]=t.pvt->_totalleapseconds[i];
	}
	pvt->_transstdwall=new unsigned char[pvt->_ttisstdcnt];
	for (long i=0; i<pvt->_ttisstdcnt; i++) {
		pvt->_transstdwall[i]=t.pvt->_transstdwall[i];
	}
	pvt->_transutclocal=new unsigned char[pvt->_ttisgmtcnt];
	for (long i=0; i<pvt->_ttisgmtcnt; i++) {
		pvt->_transutclocal[i]=t.pvt->_transutclocal[i];
	}
	pvt->_rawtimezonestring=new unsigned char[pvt->_charcnt+1];
	for (long i=0; i<pvt->_charcnt+1; i++) {
		pvt->_rawtimezonestring[i]=t.pvt->_rawtimezonestring[i];
	}
	pvt->_timezonestrings=new unsigned char *[pvt->_typecnt];
	for (long i=0; i<pvt->_typecnt; i++) {
		pvt->_timezonestrings[i]=
			&pvt->_rawtimezonestring[pvt->_ti[i].tt_abbrind];
	}
}

timezonefile::~timezonefile() {
	timezonefileClean();
}

bool timezonefile::parseFile(const char *filename) {

	file	tzfile;
	if (!tzfile.open(filename,O_RDONLY)) {
		return false;
	}

	char	magic[5];
	if (tzfile.read(magic,4*sizeof(char))!=4*sizeof(char)) {
		printf("read magic failed\n");
		return false;
	}
	magic[4]='\0';
	if (charstring::compare(magic,"TZif")) {
		printf("magic not TZif\n");
		return false;
	}

	unsigned char	future[16];
	if (tzfile.read(future,16)!=16) {
		printf("read future failed\n");
		return false;
	}
	
	if (tzfile.read(&pvt->_ttisgmtcnt,sizeof(long))!=sizeof(long)) {
		printf("read ttisgmtcnt failed\n");
		return false;
	}
	pvt->_ttisgmtcnt=ntohl(pvt->_ttisgmtcnt);

	if (tzfile.read(&pvt->_ttisstdcnt,sizeof(long))!=sizeof(long)) {
		printf("read ttisstdcnt failed\n");
		return false;
	}
	pvt->_ttisstdcnt=ntohl(pvt->_ttisstdcnt);

	if (tzfile.read(&pvt->_leapcnt,sizeof(long))!=sizeof(long)) {
		printf("read leapcnt failed\n");
		return false;
	}
	pvt->_leapcnt=ntohl(pvt->_leapcnt);

	if (tzfile.read(&pvt->_timecnt,sizeof(long))!=sizeof(long)) {
		printf("read timecnt failed\n");
		return false;
	}
	pvt->_timecnt=ntohl(pvt->_timecnt);

	if (tzfile.read(&pvt->_typecnt,sizeof(long))!=sizeof(long)) {
		printf("read typecnt failed\n");
		return false;
	}
	pvt->_typecnt=ntohl(pvt->_typecnt);

	if (tzfile.read(&pvt->_charcnt,sizeof(long))!=sizeof(long)) {
		printf("read charcnt failed\n");
		return false;
	}
	pvt->_charcnt=ntohl(pvt->_charcnt);

	int	i;
	pvt->_transitiontime=new long[pvt->_timecnt];
	for (i=0; i<pvt->_timecnt; i++) {
		if (tzfile.read(&pvt->_transitiontime[i],sizeof(long))!=	
							sizeof(long)) {
			printf("read transitiontime[%d] failed\n",i);
			return false;
		}
		pvt->_transitiontime[i]=ntohl(pvt->_transitiontime[i]);
	}

	pvt->_localtime=new unsigned char[pvt->_timecnt];
	for (i=0; i<pvt->_timecnt; i++) {
		if (tzfile.read(&pvt->_localtime[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read localtime[%d] failed\n",i);
			return false;
		}
	}

	pvt->_ti=new ttinfo[pvt->_typecnt];
	for (i=0; i<pvt->_typecnt; i++) {
		if (tzfile.read(&pvt->_ti[i].tt_gmtoff,
					sizeof(long))!=sizeof(long)) {
			printf("read ttinfo.tt_gmtoff[%d] failed\n",i);
			return false;
		}
		pvt->_ti[i].tt_gmtoff=ntohl(pvt->_ti[i].tt_gmtoff);
		if (tzfile.read(&pvt->_ti[i].tt_isdst,
					sizeof(char))!=sizeof(char)) {
			printf("read ttinfo.tt_isdst[%d] failed\n",i);
			return false;
		}
		if (tzfile.read(&pvt->_ti[i].tt_abbrind,
				sizeof(unsigned char))!=sizeof(unsigned char)) {
			printf("read ttinfo.tt_abbrind[%d] failed\n",i);
			return false;
		}
	}

	pvt->_rawtimezonestring=new unsigned char[pvt->_charcnt+1];
	if (tzfile.read(pvt->_rawtimezonestring,pvt->_charcnt)!=pvt->_charcnt) {
		printf("read rawtimezonestring failed\n");
		return false;
	}
	pvt->_rawtimezonestring[pvt->_charcnt]='\0';

	// set pointers to the timezones
	pvt->_timezonestrings=new unsigned char *[pvt->_typecnt];
	int	counter;
	for (counter=0; counter<pvt->_typecnt; counter++) {
		pvt->_timezonestrings[counter]=
			&pvt->_rawtimezonestring[pvt->_ti[counter].tt_abbrind];
	}

	pvt->_leapsecondtime=new long[pvt->_leapcnt];
	pvt->_totalleapseconds=new long[pvt->_leapcnt];
	for (i=0; i<pvt->_leapcnt; i++) {
		if (tzfile.read(&pvt->_leapsecondtime[i],
					sizeof(long))!=sizeof(long)) {
			printf("read leapsecondtime[%d] failed\n",i);
			return false;
		}
		pvt->_leapsecondtime[i]=ntohl(pvt->_leapsecondtime[i]);
		if (tzfile.read(&pvt->_totalleapseconds[i],
					sizeof(long))!=sizeof(long)) {
			printf("read totalleapseconds[%d] failed\n",i);
			return false;
		}
		pvt->_totalleapseconds[i]=ntohl(pvt->_totalleapseconds[i]);
	}

	pvt->_transstdwall=new unsigned char[pvt->_ttisstdcnt];
	for (i=0; i<pvt->_ttisstdcnt; i++) {
		if (tzfile.read(&pvt->_transstdwall[i],
				sizeof(unsigned char))!=sizeof(unsigned char)) {
			printf("read transstdwall[%d] failed\n",i);
			return false;
		}
	}

	pvt->_transutclocal=new unsigned char[pvt->_ttisgmtcnt];
	for (i=0; i<pvt->_ttisgmtcnt; i++) {
		if (tzfile.read(&pvt->_transutclocal[i],
				sizeof(unsigned char))!=sizeof(unsigned char)) {
			printf("read transutclocal[%d] failed\n",i);
			return false;
		}
	}

	tzfile.close();

	return true;
}


long timezonefile::getIsGMTCount() {
	return pvt->_ttisgmtcnt;
}

long timezonefile::getIsSTDCount() {
	return pvt->_ttisstdcnt;
}

long timezonefile::getLeapCount() {
	return pvt->_leapcnt;
}

long timezonefile::getTimeCount() {
	return pvt->_timecnt;
}

long timezonefile::getTypeCount() {
	return pvt->_typecnt;
}

long timezonefile::getCharacterCount() {
	return pvt->_charcnt;
}

long *timezonefile::getTransitionTimes() {
	return pvt->_transitiontime;
}

long timezonefile::getTransitionTime(int index) {
	return pvt->_transitiontime[index];
}

unsigned char *timezonefile::getLocalTimes() {
	return pvt->_localtime;
}

unsigned char timezonefile::getLocalTimes(int index) {
	return pvt->_localtime[index];
}

ttinfo **timezonefile::getTimeTypeInfos() {
	return &pvt->_ti;
}

ttinfo *timezonefile::getTimeTypeInfo(int index) {
	return &pvt->_ti[index];
}

long *timezonefile::getLeapSecondTimes() {
	return pvt->_leapsecondtime;
}

long timezonefile::getLeapSecondTime(int index) {
	return pvt->_leapsecondtime[index];
}

long *timezonefile::getTotalLeapSeconds() {
	return pvt->_totalleapseconds;
}

long timezonefile::getTotalLeapSeconds(int index) {
	return pvt->_totalleapseconds[index];
}

unsigned char *timezonefile::getStandardOrWallIndicators() {
	return pvt->_transstdwall;
}

unsigned char timezonefile::getStandardOrWallIndicator(int index) {
	return pvt->_transstdwall[index];
}

unsigned char *timezonefile::getUTCOrLocalIndicators() {
	return pvt->_transutclocal;
}

unsigned char timezonefile::getUTCOrLocalIndicator(int index) {
	return pvt->_transutclocal[index];
}

unsigned char *timezonefile::getRawTimeZoneString() {
	return pvt->_rawtimezonestring;
}

unsigned char **timezonefile::getTimeZoneStrings() {
	return pvt->_timezonestrings;
}

unsigned char *timezonefile::getTimeZoneString(int index) {
	return pvt->_timezonestrings[index];
}

void timezonefile::print() {
	printf("ttisgmtcnt: %ld\n",pvt->_ttisgmtcnt);
	printf("ttisstdcnt: %ld\n",pvt->_ttisstdcnt);
	printf("leapcnt: %ld\n",pvt->_leapcnt);
	printf("timecnt: %ld\n",pvt->_timecnt);
	printf("typecnt: %ld\n",pvt->_typecnt);
	printf("charcnt: %ld\n",pvt->_charcnt);
	int	i;
	for (i=0; i<pvt->_timecnt; i++) {
		printf("transitiontime[%d]: %ld\n",i,pvt->_transitiontime[i]);
	}
	for (i=0; i<pvt->_timecnt; i++) {
		printf("localtime[%d]: %d\n",i,pvt->_localtime[i]);
	}
	for (i=0; i<pvt->_typecnt; i++) {
		printf("ttinfo[%d] {\n",i);
		printf("	tt_gmtoff: %ld\n",pvt->_ti[i].tt_gmtoff);
		printf("	tt_isdst: %d\n",pvt->_ti[i].tt_isdst);
		printf("	tt_abbrind: %d\n",pvt->_ti[i].tt_abbrind);
		printf("}\n");
	}
	printf("rawtimezonestring: ");
	for (int i=0; i<pvt->_charcnt; i++) {
		if (pvt->_rawtimezonestring[i]=='\0') {
			printf(" ");
		}
		printf("%c",pvt->_rawtimezonestring[i]);
	}
	printf("\n");
	for (i=0; i<pvt->_leapcnt; i++) {
		printf("leapsecondtime[%d]: %ld\n",i,
					pvt->_leapsecondtime[i]);
		printf("totalleapseconds[%d]: %ld\n",i,
					pvt->_totalleapseconds[i]);
	}
	for (int counter=0; counter<pvt->_typecnt; counter++) {
		printf("timezonestrings[%d]=%s\n",counter,
					pvt->_timezonestrings[counter]);
	}
	for (i=0; i<pvt->_ttisstdcnt; i++) {
		printf("transstdwall[%d]: %d\n",i,pvt->_transstdwall[i]);
	}
	for (i=0; i<pvt->_ttisgmtcnt; i++) {
		printf("transutclocal[%d]: %d\n",i,pvt->_transutclocal[i]);
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
