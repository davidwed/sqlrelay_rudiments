// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/timezonefile.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/timezonefileinlines.h>
#endif
#include <rudiments/file.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>

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
	magic[4]=(char)NULL;
	if (strcmp(magic,"TZif")) {
		printf("magic not TZif\n");
		return false;
	}

	unsigned char	future[16];
	if (tzfile.read(future,16)!=16) {
		printf("read future failed\n");
		return false;
	}
	
	if (tzfile.read(&ttisgmtcnt,sizeof(long))!=sizeof(long)) {
		printf("read ttisgmtcnt failed\n");
		return false;
	}
	ttisgmtcnt=ntohl(ttisgmtcnt);

	if (tzfile.read(&ttisstdcnt,sizeof(long))!=sizeof(long)) {
		printf("read ttisstdcnt failed\n");
		return false;
	}
	ttisstdcnt=ntohl(ttisstdcnt);

	if (tzfile.read(&leapcnt,sizeof(long))!=sizeof(long)) {
		printf("read leapcnt failed\n");
		return false;
	}
	leapcnt=ntohl(leapcnt);

	if (tzfile.read(&timecnt,sizeof(long))!=sizeof(long)) {
		printf("read timecnt failed\n");
		return false;
	}
	timecnt=ntohl(timecnt);

	if (tzfile.read(&typecnt,sizeof(long))!=sizeof(long)) {
		printf("read typecnt failed\n");
		return false;
	}
	typecnt=ntohl(typecnt);

	if (tzfile.read(&charcnt,sizeof(long))!=sizeof(long)) {
		printf("read charcnt failed\n");
		return false;
	}
	charcnt=ntohl(charcnt);

	int	i;
	transitiontime=new long[timecnt];
	for (i=0; i<timecnt; i++) {
		if (tzfile.read(&transitiontime[i],sizeof(long))!=	
							sizeof(long)) {
			printf("read transitiontime[%d] failed\n",i);
			return false;
		}
		transitiontime[i]=ntohl(transitiontime[i]);
	}

	localtime=new unsigned char[timecnt];
	for (i=0; i<timecnt; i++) {
		if (tzfile.read(&localtime[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read localtime[%d] failed\n",i);
			return false;
		}
	}

	ti=new ttinfo[typecnt];
	for (i=0; i<typecnt; i++) {
		if (tzfile.read(&ti[i].tt_gmtoff,sizeof(long))!=sizeof(long)) {
			printf("read ttinfo.tt_gmtoff[%d] failed\n",i);
			return false;
		}
		ti[i].tt_gmtoff=ntohl(ti[i].tt_gmtoff);
		if (tzfile.read(&ti[i].tt_isdst,sizeof(char))!=sizeof(char)) {
			printf("read ttinfo.tt_isdst[%d] failed\n",i);
			return false;
		}
		if (tzfile.read(&ti[i].tt_abbrind,sizeof(unsigned char))!=
							sizeof(unsigned char)) {
			printf("read ttinfo.tt_abbrind[%d] failed\n",i);
			return false;
		}
	}

	rawtimezonestring=new unsigned char[charcnt+1];
	if (tzfile.read(rawtimezonestring,charcnt)!=charcnt) {
		printf("read rawtimezonestring failed\n");
		return false;
	}
	rawtimezonestring[charcnt]=(char)NULL;

	// set pointers to the timezones
	timezonestrings=new unsigned char *[typecnt];
	unsigned char	*null;
	unsigned char	*ptr=rawtimezonestring;
	int	counter;
	for (counter=0; counter<typecnt; counter++) {
		timezonestrings[counter]=&rawtimezonestring[
						ti[counter].tt_abbrind];
		ptr=null+1;
	}

	leapsecondtime=new long[leapcnt];
	totalleapseconds=new long[leapcnt];
	for (i=0; i<leapcnt; i++) {
		if (tzfile.read(&leapsecondtime[i],sizeof(long))!=
							sizeof(long)) {
			printf("read leapsecondtime[%d] failed\n",i);
			return false;
		}
		leapsecondtime[i]=ntohl(leapsecondtime[i]);
		if (tzfile.read(&totalleapseconds[i],sizeof(long))!=
							sizeof(long)) {
			printf("read totalleapseconds[%d] failed\n",i);
			return false;
		}
		totalleapseconds[i]=ntohl(totalleapseconds[i]);
	}

	transstdwall=new unsigned char[ttisstdcnt];
	for (i=0; i<ttisstdcnt; i++) {
		if (tzfile.read(&transstdwall[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read transstdwall[%d] failed\n",i);
			return false;
		}
	}

	transutclocal=new unsigned char[ttisgmtcnt];
	for (i=0; i<ttisgmtcnt; i++) {
		if (tzfile.read(&transutclocal[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read transutclocal[%d] failed\n",i);
			return false;
		}
	}

	tzfile.close();

	return true;
}

void timezonefile::print() {
	printf("ttisgmtcnt: %ld\n",ttisgmtcnt);
	printf("ttisstdcnt: %ld\n",ttisstdcnt);
	printf("leapcnt: %ld\n",leapcnt);
	printf("timecnt: %ld\n",timecnt);
	printf("typecnt: %ld\n",typecnt);
	printf("charcnt: %ld\n",charcnt);
	int	i;
	for (i=0; i<timecnt; i++) {
		printf("transitiontime[%d]: %ld\n",i,transitiontime[i]);
	}
	for (i=0; i<timecnt; i++) {
		printf("localtime[%d]: %d\n",i,localtime[i]);
	}
	for (i=0; i<typecnt; i++) {
		printf("ttinfo[%d] {\n",i);
		printf("	tt_gmtoff: %ld\n",ti[i].tt_gmtoff);
		printf("	tt_isdst: %d\n",ti[i].tt_isdst);
		printf("	tt_abbrind: %d\n",ti[i].tt_abbrind);
		printf("}\n");
	}
	printf("rawtimezonestring: ");
	for (int i=0; i<charcnt; i++) {
		if (rawtimezonestring[i]==(char)NULL) {
			printf(" ");
		}
		printf("%c",rawtimezonestring[i]);
	}
	printf("\n");
	for (i=0; i<leapcnt; i++) {
		printf("leapsecondtime[%d]: %ld\n",i,leapsecondtime[i]);
		printf("totalleapseconds[%d]: %ld\n",i,totalleapseconds[i]);
	}
	for (int counter=0; counter<typecnt; counter++) {
		printf("timezonestrings[%d]=%s\n",counter,
					timezonestrings[counter]);
	}
	for (i=0; i<ttisstdcnt; i++) {
		printf("transstdwall[%d]: %d\n",i,transstdwall[i]);
	}
	for (i=0; i<ttisgmtcnt; i++) {
		printf("transutclocal[%d]: %d\n",i,transutclocal[i]);
	}
}
