// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/timezonefile.h>
#include <rudiments/file.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/charstring.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class timezonefileprivate {
	friend class timezonefile;
	private:
		uint64_t	_ttisgmtcnt;
		uint64_t	_ttisstdcnt;
		uint64_t	_leapcnt;
		uint64_t	_timecnt;
		uint64_t	_typecnt;
		uint64_t	_charcnt;
		uint64_t	*_transitiontime;
		unsigned char	*_localtime;
		ttinfo		*_ti;
		uint64_t	*_leapsecondtime;
		uint64_t	*_totalleapseconds;
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
	pvt->_transitiontime=new uint64_t[pvt->_timecnt];
	pvt->_localtime=new unsigned char[pvt->_timecnt];
	for (uint64_t i=0; i<pvt->_timecnt; i++) {
		pvt->_transitiontime[i]=t.pvt->_transitiontime[i];
		pvt->_localtime[i]=t.pvt->_localtime[i];
	}
	pvt->_ti=new ttinfo[pvt->_typecnt];
	for (uint64_t i=0; i<pvt->_timecnt; i++) {
		pvt->_ti[i]=t.pvt->_ti[i];
	}
	pvt->_leapsecondtime=new uint64_t[pvt->_leapcnt];
	pvt->_totalleapseconds=new uint64_t[pvt->_leapcnt];
	for (uint64_t i=0; i<pvt->_leapcnt; i++) {
		pvt->_leapsecondtime[i]=t.pvt->_leapsecondtime[i];
		pvt->_totalleapseconds[i]=t.pvt->_totalleapseconds[i];
	}
	pvt->_transstdwall=new unsigned char[pvt->_ttisstdcnt];
	for (uint64_t i=0; i<pvt->_ttisstdcnt; i++) {
		pvt->_transstdwall[i]=t.pvt->_transstdwall[i];
	}
	pvt->_transutclocal=new unsigned char[pvt->_ttisgmtcnt];
	for (uint64_t i=0; i<pvt->_ttisgmtcnt; i++) {
		pvt->_transutclocal[i]=t.pvt->_transutclocal[i];
	}
	pvt->_rawtimezonestring=new unsigned char[pvt->_charcnt+1];
	for (uint64_t i=0; i<pvt->_charcnt+1; i++) {
		pvt->_rawtimezonestring[i]=t.pvt->_rawtimezonestring[i];
	}
	pvt->_timezonestrings=new unsigned char *[pvt->_typecnt];
	for (uint64_t i=0; i<pvt->_typecnt; i++) {
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

	unsigned long	tmp;	
	if (tzfile.read(&tmp,sizeof(unsigned long))!=sizeof(unsigned long)) {
		printf("read ttisgmtcnt failed\n");
		return false;
	}
	pvt->_ttisgmtcnt=tzfile.netToHost((uint64_t)tmp);

	if (tzfile.read(&tmp,sizeof(unsigned long))!=sizeof(unsigned long)) {
		printf("read ttisstdcnt failed\n");
		return false;
	}
	pvt->_ttisstdcnt=tzfile.netToHost((uint64_t)tmp);

	if (tzfile.read(&tmp,sizeof(unsigned long))!=sizeof(unsigned long)) {
		printf("read leapcnt failed\n");
		return false;
	}
	pvt->_leapcnt=tzfile.netToHost((uint64_t)tmp);

	if (tzfile.read(&tmp,sizeof(unsigned long))!=sizeof(unsigned long)) {
		printf("read timecnt failed\n");
		return false;
	}
	pvt->_timecnt=tzfile.netToHost((uint64_t)tmp);

	if (tzfile.read(&tmp,sizeof(unsigned long))!=sizeof(unsigned long)) {
		printf("read typecnt failed\n");
		return false;
	}
	pvt->_typecnt=tzfile.netToHost((uint64_t)tmp);

	if (tzfile.read(&tmp,sizeof(unsigned long))!=sizeof(unsigned long)) {
		printf("read charcnt failed\n");
		return false;
	}
	pvt->_charcnt=tzfile.netToHost((uint64_t)tmp);

	uint64_t	i;
	pvt->_transitiontime=new uint64_t[pvt->_timecnt];
	for (i=0; i<pvt->_timecnt; i++) {
		if (tzfile.read(&tmp,
				sizeof(unsigned long))!=sizeof(unsigned long)) {
			printf("read transitiontime[%lld] failed\n",i);
			return false;
		}
		pvt->_transitiontime[i]=tzfile.netToHost((uint64_t)tmp);
	}

	pvt->_localtime=new unsigned char[pvt->_timecnt];
	for (i=0; i<pvt->_timecnt; i++) {
		if (tzfile.read(&pvt->_localtime[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read localtime[%lld] failed\n",i);
			return false;
		}
	}

	pvt->_ti=new ttinfo[pvt->_typecnt];
	for (i=0; i<pvt->_typecnt; i++) {
		long	signedtmp;
		if (tzfile.read(&signedtmp,sizeof(long))!=sizeof(long)) {
			printf("read ttinfo.tt_gmtoff[%lld] failed\n",i);
			return false;
		}
		pvt->_ti[i].tt_gmtoff=(int64_t)tzfile.netToHost(
						(uint64_t)signedtmp);
		if (tzfile.read(&pvt->_ti[i].tt_isdst,
					sizeof(char))!=sizeof(char)) {
			printf("read ttinfo.tt_isdst[%lld] failed\n",i);
			return false;
		}
		if (tzfile.read(&pvt->_ti[i].tt_abbrind,
				sizeof(unsigned char))!=sizeof(unsigned char)) {
			printf("read ttinfo.tt_abbrind[%lld] failed\n",i);
			return false;
		}
	}

	pvt->_rawtimezonestring=new unsigned char[pvt->_charcnt+1];
	if ((uint64_t)tzfile.read(pvt->_rawtimezonestring,pvt->_charcnt)!=
								pvt->_charcnt) {
		printf("read rawtimezonestring failed\n");
		return false;
	}
	pvt->_rawtimezonestring[pvt->_charcnt]='\0';

	// set pointers to the timezones
	pvt->_timezonestrings=new unsigned char *[pvt->_typecnt];
	uint64_t	counter;
	for (counter=0; counter<pvt->_typecnt; counter++) {
		pvt->_timezonestrings[counter]=
			&pvt->_rawtimezonestring[pvt->_ti[counter].tt_abbrind];
	}

	pvt->_leapsecondtime=new uint64_t[pvt->_leapcnt];
	pvt->_totalleapseconds=new uint64_t[pvt->_leapcnt];
	for (i=0; i<pvt->_leapcnt; i++) {
		if (tzfile.read(&tmp,sizeof(uint64_t))!=sizeof(uint64_t)) {
			printf("read leapsecondtime[%lld] failed\n",i);
			return false;
		}
		pvt->_leapsecondtime[i]=tzfile.netToHost((uint64_t)tmp);
		if (tzfile.read(&pvt->_totalleapseconds[i],
				sizeof(uint64_t))!=sizeof(uint64_t)) {
			printf("read totalleapseconds[%lld] failed\n",i);
			return false;
		}
		pvt->_totalleapseconds[i]=tzfile.netToHost((uint64_t)tmp);
	}

	pvt->_transstdwall=new unsigned char[pvt->_ttisstdcnt];
	for (i=0; i<pvt->_ttisstdcnt; i++) {
		if (tzfile.read(&pvt->_transstdwall[i],
				sizeof(unsigned char))!=sizeof(unsigned char)) {
			printf("read transstdwall[%lld] failed\n",i);
			return false;
		}
	}

	pvt->_transutclocal=new unsigned char[pvt->_ttisgmtcnt];
	for (i=0; i<pvt->_ttisgmtcnt; i++) {
		if (tzfile.read(&pvt->_transutclocal[i],
				sizeof(unsigned char))!=sizeof(unsigned char)) {
			printf("read transutclocal[%lld] failed\n",i);
			return false;
		}
	}

	tzfile.close();

	return true;
}


uint64_t timezonefile::getIsGMTCount() {
	return pvt->_ttisgmtcnt;
}

uint64_t timezonefile::getIsSTDCount() {
	return pvt->_ttisstdcnt;
}

uint64_t timezonefile::getLeapCount() {
	return pvt->_leapcnt;
}

uint64_t timezonefile::getTimeCount() {
	return pvt->_timecnt;
}

uint64_t timezonefile::getTypeCount() {
	return pvt->_typecnt;
}

uint64_t timezonefile::getCharacterCount() {
	return pvt->_charcnt;
}

uint64_t *timezonefile::getTransitionTimes() {
	return pvt->_transitiontime;
}

uint64_t timezonefile::getTransitionTime(int index) {
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

uint64_t *timezonefile::getLeapSecondTimes() {
	return pvt->_leapsecondtime;
}

uint64_t timezonefile::getLeapSecondTime(int index) {
	return pvt->_leapsecondtime[index];
}

uint64_t *timezonefile::getTotalLeapSeconds() {
	return pvt->_totalleapseconds;
}

uint64_t timezonefile::getTotalLeapSeconds(int index) {
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
	printf("ttisgmtcnt: %lld\n",pvt->_ttisgmtcnt);
	printf("ttisstdcnt: %lld\n",pvt->_ttisstdcnt);
	printf("leapcnt: %lld\n",pvt->_leapcnt);
	printf("timecnt: %lld\n",pvt->_timecnt);
	printf("typecnt: %lld\n",pvt->_typecnt);
	printf("charcnt: %lld\n",pvt->_charcnt);
	uint64_t	i;
	for (i=0; i<pvt->_timecnt; i++) {
		printf("transitiontime[%lld]: %lld\n",
				i,pvt->_transitiontime[i]);
	}
	for (i=0; i<pvt->_timecnt; i++) {
		printf("localtime[%lld]: %d\n",
				i,pvt->_localtime[i]);
	}
	for (i=0; i<pvt->_typecnt; i++) {
		printf("ttinfo[%lld] {\n",i);
		printf("	tt_gmtoff: %lld\n",pvt->_ti[i].tt_gmtoff);
		printf("	tt_isdst: %d\n",pvt->_ti[i].tt_isdst);
		printf("	tt_abbrind: %d\n",pvt->_ti[i].tt_abbrind);
		printf("}\n");
	}
	printf("rawtimezonestring: ");
	for (uint64_t i=0; i<pvt->_charcnt; i++) {
		if (pvt->_rawtimezonestring[i]=='\0') {
			printf(" ");
		}
		printf("%c",pvt->_rawtimezonestring[i]);
	}
	printf("\n");
	for (i=0; i<pvt->_leapcnt; i++) {
		printf("leapsecondtime[%lld]: %lld\n",i,
					pvt->_leapsecondtime[i]);
		printf("totalleapseconds[%lld]: %lld\n",i,
					pvt->_totalleapseconds[i]);
	}
	for (uint64_t counter=0; counter<pvt->_typecnt; counter++) {
		printf("timezonestrings[%lld]=%s\n",counter,
					pvt->_timezonestrings[counter]);
	}
	for (i=0; i<pvt->_ttisstdcnt; i++) {
		printf("transstdwall[%lld]: %d\n",i,pvt->_transstdwall[i]);
	}
	for (i=0; i<pvt->_ttisgmtcnt; i++) {
		printf("transutclocal[%lld]: %d\n",i,pvt->_transutclocal[i]);
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
