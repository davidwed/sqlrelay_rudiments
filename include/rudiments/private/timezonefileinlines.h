// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

long timezonefile::getIsGMTCount() {
	return ttisgmtcnt;
}

long timezonefile::getIsSTDCount() {
	return ttisstdcnt;
}

long timezonefile::getLeapCount() {
	return leapcnt;
}

long timezonefile::getTimeCount() {
	return timecnt;
}

long timezonefile::getTypeCount() {
	return typecnt;
}

long timezonefile::getCharacterCount() {
	return charcnt;
}

long *timezonefile::getTransitionTimes() {
	return transitiontime;
}

long timezonefile::getTransitionTime(int index) {
	return transitiontime[index];
}

unsigned char *timezonefile::getLocalTimes() {
	return localtime;
}

unsigned char timezonefile::getLocalTimes(int index) {
	return localtime[index];
}

ttinfo **timezonefile::getTimeTypeInfos() {
	return &ti;
}

ttinfo *timezonefile::getTimeTypeInfo(int index) {
	return &ti[index];
}

long *timezonefile::getLeapSecondTimes() {
	return leapsecondtime;
}

long timezonefile::getLeapSecondTime(int index) {
	return leapsecondtime[index];
}

long *timezonefile::getTotalLeapSeconds() {
	return totalleapseconds;
}

long timezonefile::getTotalLeapSeconds(int index) {
	return totalleapseconds[index];
}

unsigned char *timezonefile::getStandardOrWallIndicators() {
	return transstdwall;
}

unsigned char timezonefile::getStandardOrWallIndicator(int index) {
	return transstdwall[index];
}

unsigned char *timezonefile::getUTCOrLocalIndicators() {
	return transutclocal;
}

unsigned char timezonefile::getUTCOrLocalIndicator(int index) {
	return transutclocal[index];
}

unsigned char *timezonefile::getRawTimeZoneString() {
	return rawtimezonestring;
}

unsigned char **timezonefile::getTimeZoneStrings() {
	return timezonestrings;
}

unsigned char *timezonefile::getTimeZoneString(int index) {
	return timezonestrings[index];
}

timezonefile::timezonefile() {
	rawtimezonestring=NULL;
	timezonestrings=NULL;
	transutclocal=NULL;
	transstdwall=NULL;
	leapsecondtime=NULL;
	totalleapseconds=NULL;
	transitiontime=NULL;
	localtime=NULL;
	ti=NULL;
}

timezonefile::~timezonefile() {
	delete[] timezonestrings;
	delete[] rawtimezonestring;
	delete[] transutclocal;
	delete[] transstdwall;
	delete[] leapsecondtime;
	delete[] totalleapseconds;
	delete[] transitiontime;
	delete[] localtime;
	delete[] ti;
}
