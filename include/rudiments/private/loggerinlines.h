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

inline int	filedestination::open(const char *filename) {
	return ((logfile=::open(filename,O_CREAT|O_WRONLY|O_APPEND,
				permissions::ownerReadWrite()))!=-1);
}

inline void	filedestination::close() {
	if (logfile) {
		::close(logfile);
		logfile=-1;
	}
}

inline void	syslogdestination::open(const char *ident,
					int option, int facility,
					int priority) {
	openlog(ident,option,facility);
	this->priority=priority;
}

inline void	syslogdestination::close() {
	closelog();
}

inline void	syslogdestination::write(const char *string) {
	syslog(priority,string);
}

inline	filedestination::filedestination() {
	logfile=-1;
}

inline void	filedestination::write(const char *string) {
	if (logfile>-1) {
		::write(logfile,string,strlen(string));
	}
}

inline void	stdoutdestination::write(const char *string) {
	::write(1,string,strlen(string));
}

inline void	stderrdestination::write(const char *string) {
	::write(2,string,strlen(string));
}



inline logger::logger() {
	first=NULL;
	last=NULL;
}

inline logger::~logger() {
	removeAllLogDestinations();
}
