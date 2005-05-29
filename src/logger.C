// Copyright (c) 1999-2001  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#include <rudiments/datetime.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#include <stdio.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

filedestination::filedestination() : logdestination() {}

filedestination::filedestination(const filedestination &f) :
						logdestination(f) {
	logfile=f.logfile;
}

filedestination &filedestination::operator=(const filedestination &f) {
	if (this!=&f) {
		logfile=f.logfile;
	}
	return *this;
}

filedestination::~filedestination() {
	close();
}

bool filedestination::open(const char *filename) {
	return logfile.open(filename,O_CREAT|O_WRONLY|O_APPEND,
				permissions::ownerReadWrite());
}

void filedestination::close() {
	logfile.close();
}

void filedestination::write(const char *string) {
	logfile.write(string);
}

void stdoutdestination::write(const char *string) {
	int	result;
	do {
		result=::write(1,string,charstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR);
}

void stderrdestination::write(const char *string) {
	int	result;
	do {
		result=::write(2,string,charstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR);
}

syslogdestination::syslogdestination() : logdestination() {}

syslogdestination::syslogdestination(const syslogdestination &s) :
						logdestination(s) {}

syslogdestination &syslogdestination::operator=(const syslogdestination &s) {
	if (this!=&s) {
		priority=s.priority;
	}
	return *this;
}

syslogdestination::~syslogdestination() {
	close();
}

void syslogdestination::open(const char *ident, int option,
					int facility, int priority) {
	openlog(ident,option,facility);
	this->priority=priority;
}

void syslogdestination::close() {
	closelog();
}

void syslogdestination::write(const char *string) {
	syslog(priority,string);
}

logger::logger() {
}

logger::~logger() {
	removeAllLogDestinations();
}

void logger::addLogDestination(logdestination *logdest) {
	logdestlist.append(logdest);
}

void logger::removeLogDestination(logdestination *logdest) {
	logdestlist.removeByData(logdest);
}

void logger::removeAllLogDestinations() {
	logdestlist.clear();
}

char *logger::logHeader(const char *name) {
	datetime	dt;
	dt.getSystemDateAndTime();
	const char	*dtstring=dt.getString();
	char		*retval=new char[charstring::length(dtstring)+
						charstring::length(name)+16];
	sprintf(retval,"%s %s [%d]",dtstring,name,getpid());
	return retval;
}

void logger::write(const char *header, int32_t tabs, const char *string) {
	char	logentry[charstring::length(header)+3+tabs+
				charstring::length(string)+2+1];
	sprintf(logentry,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%s\n\n",logentry,string);
	write(logentry);
}

void logger::write(const char *header, int32_t tabs, char character) {
	char	logentry[charstring::length(header)+3+tabs+1+2+1];
	sprintf(logentry,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%c\n\n",logentry,character);
	write(logentry);
}

void logger::write(const char *header, int32_t tabs, int32_t number) {
	char	logentry[charstring::length(header)+3+tabs+20+2+1];
	sprintf(logentry,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%ld\n\n",logentry,number);
	write(logentry);
}

void logger::write(const char *header, int32_t tabs, double number) {
	char	logentry[charstring::length(header)+3+tabs+21+2+1];
	sprintf(logentry,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%f\n\n",logentry,number);
	write(logentry);
}

void logger::write(const char *logentry) {
	loggerlistnode	*current=logdestlist.getNodeByIndex(0);
	while (current) {
		current->getData()->write(logentry);
		current=(loggerlistnode *)current->getNext();
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
