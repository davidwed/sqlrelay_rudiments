// Copyright (c) 1999-2001  David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#include <rudiments/datetime.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/process.h>

#include <stdio.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

logdestination::~logdestination() {}

class filedestinationprivate {
	friend class filedestination;
	private:
		file	_logfile;
};

filedestination::filedestination() : logdestination() {
	pvt=new filedestinationprivate;
}

filedestination::filedestination(const filedestination &f) :
						logdestination(f) {
	pvt=new filedestinationprivate;
	pvt->_logfile=f.pvt->_logfile;
}

filedestination &filedestination::operator=(const filedestination &f) {
	if (this!=&f) {
		pvt->_logfile=f.pvt->_logfile;
	}
	return *this;
}

filedestination::~filedestination() {
	close();
	delete pvt;
}

bool filedestination::open(const char *filename) {
	return pvt->_logfile.open(filename,O_CREAT|O_WRONLY|O_APPEND,
				permissions::ownerReadWrite());
}

void filedestination::close() {
	pvt->_logfile.close();
}

void filedestination::write(const char *string) {
	pvt->_logfile.write(string);
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

class syslogdestinationprivate {
	friend class syslogdestination;
	private:
		int	_priority;
};

syslogdestination::syslogdestination() : logdestination() {
	pvt=new syslogdestinationprivate;
}

syslogdestination::syslogdestination(const syslogdestination &s) :
						logdestination(s) {
	pvt=new syslogdestinationprivate;
}

syslogdestination &syslogdestination::operator=(const syslogdestination &s) {
	if (this!=&s) {
		pvt->_priority=s.pvt->_priority;
	}
	return *this;
}

syslogdestination::~syslogdestination() {
	close();
	delete pvt;
}

void syslogdestination::open(const char *ident, int option,
					int facility, int priority) {
	openlog(ident,option,facility);
	pvt->_priority=priority;
}

void syslogdestination::close() {
	closelog();
}

void syslogdestination::write(const char *string) {
	syslog(pvt->_priority,string);
}

class loggerprivate {
	friend class logger;
	private:
		loggerlist	_logdestlist;
};

logger::logger() {
	pvt=new loggerprivate;
}

logger::~logger() {
	removeAllLogDestinations();
	delete pvt;
}

void logger::addLogDestination(logdestination *logdest) {
	pvt->_logdestlist.append(logdest);
}

void logger::removeLogDestination(logdestination *logdest) {
	pvt->_logdestlist.removeByData(logdest);
}

void logger::removeAllLogDestinations() {
	pvt->_logdestlist.clear();
}

char *logger::logHeader(const char *name) {
	datetime	dt;
	dt.getSystemDateAndTime();
	pid_t		pid=process::getProcessId();
	const char	*dtstring=dt.getString();
	size_t		retvallen=charstring::length(dtstring)+1+
				charstring::length(name)+2+
				charstring::integerLength((uint64_t)pid)+2;
	char		*retval=new char[retvallen];
	snprintf(retval,retvallen,"%s %s [%d]",dtstring,name,pid);
	return retval;
}

void logger::write(const char *header, int32_t tabs, const char *string) {
	size_t	logentrylen=charstring::length(header)+3+tabs+
				charstring::length(string)+2+1;
	char	*logentry=new char[logentrylen];
	snprintf(logentry,logentrylen,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		snprintf(logentry,logentrylen,"%s%c",logentry,'	');
	}
	snprintf(logentry,logentrylen,"%s%s\n\n",logentry,string);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *header, int32_t tabs, char character) {
	size_t	logentrylen=charstring::length(header)+3+tabs+1+2+1;
	char	*logentry=new char[logentrylen];
	snprintf(logentry,logentrylen,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		snprintf(logentry,logentrylen,"%s%c",logentry,'	');
	}
	snprintf(logentry,logentrylen,"%s%c\n\n",logentry,character);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *header, int32_t tabs, int32_t number) {
	size_t	logentrylen=charstring::length(header)+3+tabs+20+2+1;
	char	*logentry=new char[logentrylen];
	snprintf(logentry,logentrylen,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		snprintf(logentry,logentrylen,"%s%c",logentry,'	');
	}
	snprintf(logentry,logentrylen,"%s%d\n\n",logentry,number);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *header, int32_t tabs, double number) {
	size_t	logentrylen=charstring::length(header)+3+tabs+21+2+1;
	char	*logentry=new char[logentrylen];
	snprintf(logentry,logentrylen,"%s : ",header);
	for (int32_t i=0; i<tabs; i++) {
		snprintf(logentry,logentrylen,"%s%c",logentry,'	');
	}
	snprintf(logentry,logentrylen,"%s%f\n\n",logentry,number);
	write(logentry);
	delete[] logentry;
}

void logger::write(const char *logentry) {
	loggerlistnode	*current=pvt->_logdestlist.getNodeByIndex(0);
	while (current) {
		current->getData()->write(logentry);
		current=(loggerlistnode *)current->getNext();
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
