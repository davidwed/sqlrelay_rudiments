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

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE int filedestination::open(const char *filename) {
	return ((logfile=::open(filename,O_CREAT|O_WRONLY|O_APPEND,
				permissions::ownerReadWrite()))!=-1);
}

RUDIMENTS_INLINE void filedestination::close() {
	::close(logfile);
	logfile=-1;
}

RUDIMENTS_INLINE void syslogdestination::open(const char *ident,
					int option, int facility,
					int priority) {
	openlog(ident,option,facility);
	this->priority=priority;
}

RUDIMENTS_INLINE void syslogdestination::close() {
	closelog();
}

RUDIMENTS_INLINE void syslogdestination::write(const char *string) {
	syslog(priority,string);
}

RUDIMENTS_INLINE filedestination::filedestination() {
	logfile=-1;
}

RUDIMENTS_INLINE void filedestination::write(const char *string) {
	::write(logfile,string,strlen(string));
}

RUDIMENTS_INLINE void stdoutdestination::write(const char *string) {
	::write(1,string,strlen(string));
}

RUDIMENTS_INLINE void stderrdestination::write(const char *string) {
	::write(2,string,strlen(string));
}



RUDIMENTS_INLINE logger::logger() {
}

RUDIMENTS_INLINE logger::~logger() {
	removeAllLogDestinations();
}

RUDIMENTS_INLINE void logger::addLogDestination(logdestination *logdest) {
	logdestlist.append(logdest);
}

RUDIMENTS_INLINE void logger::removeLogDestination(logdestination *logdest) {
	logdestlist.removeByData(logdest);
}

RUDIMENTS_INLINE void logger::removeAllLogDestinations() {
	logdestlist.clear();
}
