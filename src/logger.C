// Copyright (c) 1999-2001  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>

#include <rudiments/datetime.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>

#include <stdio.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

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
	::write(1,string,charstring::getLength(string));
}

void stderrdestination::write(const char *string) {
	::write(2,string,charstring::getLength(string));
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
	char	*dtstring=dt.getString();
	char	*retval=new char[charstring::getLength(dtstring)+
					charstring::getLength(name)+16];
	sprintf(retval,"%s %s [%d]",dtstring,name,getpid());
	return retval;
}

void logger::write(const char *header, int tabs, const char *string) const {
	char	logentry[charstring::getLength(header)+3+tabs+
				charstring::getLength(string)+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%s\n\n",logentry,string);
	write(logentry);
}

void logger::write(const char *header, int tabs, char character) const {
	char	logentry[charstring::getLength(header)+3+tabs+1+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%c\n\n",logentry,character);
	write(logentry);
}

void logger::write(const char *header, int tabs, long number) const {
	char	logentry[charstring::getLength(header)+3+tabs+20+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%ld\n\n",logentry,number);
	write(logentry);
}

void logger::write(const char *header, int tabs, double number) const {
	char	logentry[charstring::getLength(header)+3+tabs+21+2+1];
	sprintf(logentry,"%s : ",header);
	for (int i=0; i<tabs; i++) {
		sprintf(logentry,"%s%c",logentry,'	');
	}
	sprintf(logentry,"%s%f\n\n",logentry,number);
	write(logentry);
}

void logger::write(const char *logentry) const {
	loggerlistnode	*current=logdestlist.getNodeByIndex(0);
	while (current) {
		current->getData()->write(logentry);
		current=(loggerlistnode *)current->getNext();
	}
}
