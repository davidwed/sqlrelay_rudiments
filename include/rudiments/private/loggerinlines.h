// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <unistd.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#include <rudiments/permissions.h>

INLINE int filedestination::open(const char *filename) {
	return ((logfile=::open(filename,O_CREAT|O_WRONLY|O_APPEND,
				permissions::ownerReadWrite()))!=-1);
}

INLINE void filedestination::close() {
	::close(logfile);
	logfile=-1;
}

INLINE void syslogdestination::open(const char *ident,
					int option, int facility,
					int priority) {
	openlog(ident,option,facility);
	this->priority=priority;
}

INLINE void syslogdestination::close() {
	closelog();
}

INLINE void syslogdestination::write(const char *string) {
	syslog(priority,string);
}

INLINE filedestination::filedestination() {
	logfile=-1;
}

INLINE void filedestination::write(const char *string) {
	::write(logfile,string,strlen(string));
}

INLINE void stdoutdestination::write(const char *string) {
	::write(1,string,strlen(string));
}

INLINE void stderrdestination::write(const char *string) {
	::write(2,string,strlen(string));
}



INLINE logger::logger() {
}

INLINE logger::~logger() {
	removeAllLogDestinations();
}

INLINE void logger::addLogDestination(logdestination *logdest) {
	logdestlist.append(logdest);
}

INLINE void logger::removeLogDestination(logdestination *logdest) {
	logdestlist.removeByData(logdest);
}

INLINE void logger::removeAllLogDestinations() {
	logdestlist.clear();
}
