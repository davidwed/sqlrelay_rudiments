#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>

struct ttinfo {
	long		tt_gmtoff;
	int		tt_isdst;
	unsigned int	tt_abbrind;
};

class timezonefile {
	public:
			timezonefile();
			~timezonefile();
		int		parseFile(const char *filename);
		long		getIsGMTCount();
		long		getIsSTDCount();
		long		getLeapCount();
		long		getTimeCount();
		long		getTypeCount();
		long		getCharacterCount();
		long		*getTransitionTimes();
		long		getTransitionTime(int index);
		unsigned char	*getLocalTimes();
		unsigned char	getLocalTimes(int index);
		ttinfo		**getTimeTypeInfos();
		ttinfo		*getTimeTypeInfo(int index);
		long		*getLeapSecondTimes();
		long		getLeapSecondTime(int index);
		long		*getTotalLeapSeconds();
		long		getTotalLeapSeconds(int index);
		unsigned char	*getStandardOrWallIndicators();
		unsigned char	getStandardOrWallIndicator(int index);
		unsigned char	*getUTCOrLocalIndicators();
		unsigned char	getUTCOrLocalIndicator(int index);
		unsigned char	*getRawTimeZoneString();
		unsigned char	**getTimeZoneStrings();
		unsigned char	*getTimeZoneString(int index);
		void		print();
	private:
		long		ttisgmtcnt;
		long		ttisstdcnt;
		long		leapcnt;
		long		timecnt;
		long		typecnt;
		long		charcnt;
		long		*transitiontime;
		unsigned char	*localtime;
		ttinfo		*ti;
		long		*leapsecondtime;
		long		*totalleapseconds;
		unsigned char	*transstdwall;
		unsigned char	*transutclocal;
		unsigned char	*rawtimezonestring;
		unsigned char	**timezonestrings;
};

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

int	timezonefile::parseFile(const char *filename) {

	int	file=open(filename,O_RDONLY);

	char	magic[5];
	if (read(file,magic,4*sizeof(char))!=4*sizeof(char)) {
		printf("read magic failed\n");
		return 0;
	}
	magic[4]=(char)NULL;
	if (strcmp(magic,"TZif")) {
		printf("magic not TZif\n");
		return 0;
	}

	unsigned char	future[16];
	if (read(file,future,16)!=16) {
		printf("read future failed\n");
		return 0;
	}
	
	if (read(file,&ttisgmtcnt,sizeof(long))!=sizeof(long)) {
		printf("read ttisgmtcnt failed\n");
		return 0;
	}
	ttisgmtcnt=ntohl(ttisgmtcnt);

	if (read(file,&ttisstdcnt,sizeof(long))!=sizeof(long)) {
		printf("read ttisstdcnt failed\n");
		return 0;
	}
	ttisstdcnt=ntohl(ttisstdcnt);

	if (read(file,&leapcnt,sizeof(long))!=sizeof(long)) {
		printf("read leapcnt failed\n");
		return 0;
	}
	leapcnt=ntohl(leapcnt);

	if (read(file,&timecnt,sizeof(long))!=sizeof(long)) {
		printf("read timecnt failed\n");
		return 0;
	}
	timecnt=ntohl(timecnt);

	if (read(file,&typecnt,sizeof(long))!=sizeof(long)) {
		printf("read typecnt failed\n");
		return 0;
	}
	typecnt=ntohl(typecnt);

	if (read(file,&charcnt,sizeof(long))!=sizeof(long)) {
		printf("read charcnt failed\n");
		return 0;
	}
	charcnt=ntohl(charcnt);

	int	i;
	transitiontime=new long[timecnt];
	for (i=0; i<timecnt; i++) {
		if (read(file,&transitiontime[i],sizeof(long))!=sizeof(long)) {
			printf("read transitiontime[%d] failed\n",i);
			return 0;
		}
		transitiontime[i]=ntohl(transitiontime[i]);
	}

	localtime=new unsigned char[timecnt];
	for (i=0; i<timecnt; i++) {
		if (read(file,&localtime[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read localtime[%d] failed\n",i);
			return 0;
		}
	}

	ti=new ttinfo[typecnt];
	for (i=0; i<typecnt; i++) {
		if (read(file,&ti[i].tt_gmtoff,sizeof(long))!=sizeof(long)) {
			printf("read ttinfo.tt_gmtoff[%d] failed\n",i);
			return 0;
		}
		ti[i].tt_gmtoff=ntohl(ti[i].tt_gmtoff);
		if (read(file,&ti[i].tt_isdst,sizeof(char))!=sizeof(char)) {
			printf("read ttinfo.tt_isdst[%d] failed\n",i);
			return 0;
		}
		if (read(file,&ti[i].tt_abbrind,sizeof(unsigned char))!=
							sizeof(unsigned char)) {
			printf("read ttinfo.tt_abbrind[%d] failed\n",i);
			return 0;
		}
	}

	rawtimezonestring=new unsigned char[charcnt+1];
	if (read(file,rawtimezonestring,charcnt)!=charcnt) {
		printf("read rawtimezonestring failed\n");
		return 0;
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
		if (read(file,&leapsecondtime[i],sizeof(long))!=
							sizeof(long)) {
			printf("read leapsecondtime[%d] failed\n",i);
			return 0;
		}
		leapsecondtime[i]=ntohl(leapsecondtime[i]);
		if (read(file,&totalleapseconds[i],sizeof(long))!=
							sizeof(long)) {
			printf("read totalleapseconds[%d] failed\n",i);
			return 0;
		}
		totalleapseconds[i]=ntohl(totalleapseconds[i]);
	}

	transstdwall=new unsigned char[ttisstdcnt];
	for (i=0; i<ttisstdcnt; i++) {
		if (read(file,&transstdwall[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read transstdwall[%d] failed\n",i);
			return 0;
		}
	}

	transutclocal=new unsigned char[ttisgmtcnt];
	for (i=0; i<ttisgmtcnt; i++) {
		if (read(file,&transutclocal[i],sizeof(unsigned char))!=
						sizeof(unsigned char)) {
			printf("read transutclocal[%d] failed\n",i);
			return 0;
		}
	}

	close(file);

	return 1;
}

void timezonefile::print() {
	printf("ttisgmtcnt: %d\n",ttisgmtcnt);
	printf("ttisstdcnt: %d\n",ttisstdcnt);
	printf("leapcnt: %d\n",leapcnt);
	printf("timecnt: %d\n",timecnt);
	printf("typecnt: %d\n",typecnt);
	printf("charcnt: %d\n",charcnt);
	int	i;
	for (i=0; i<timecnt; i++) {
		printf("transitiontime[%d]: %d\n",i,transitiontime[i]);
	}
	for (i=0; i<timecnt; i++) {
		printf("localtime[%d]: %d\n",i,localtime[i]);
	}
	for (i=0; i<typecnt; i++) {
		printf("ttinfo[%d] {\n",i);
		printf("	tt_gmtoff: %d\n",ti[i].tt_gmtoff);
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
		printf("leapsecondtime[%d]: %d\n",i,leapsecondtime[i]);
		printf("totalleapseconds[%d]: %d\n",i,totalleapseconds[i]);
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

main(int argc, char **argv) {

	timezonefile	tz;
	if (tz.parseFile(argv[1])) {
		for (int i=0; i<tz.getTypeCount(); i++) {
			printf("%s ",tz.getTimeZoneString(i));
		}
		printf("\n");
	}
}
