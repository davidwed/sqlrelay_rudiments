// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/unixclientsocket.h>
#include <rudiments/charstring.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

unixclientsocket::unixclientsocket() : clientsocket(), unixsocketutil() {
	type="unixclientsocket";
}

unixclientsocket::unixclientsocket(const unixclientsocket &u) :
					clientsocket(u), unixsocketutil(u) {
	type="unixclientsocket";
}

unixclientsocket &unixclientsocket::operator=(const unixclientsocket &u) {
	if (this!=&u) {
		clientsocket::operator=(u);
		unixsocketutil::operator=(u);
	}
	return *this;
}

unixclientsocket::~unixclientsocket() {}

int unixclientsocket::connect(const char *filename,
						long timeoutsec,
						long timeoutusec,
						unsigned long retrywait,
						unsigned long retrycount) {
	initialize(filename,timeoutsec,timeoutusec,retrywait,retrycount);
	return connect();
}

void unixclientsocket::initialize(const char *filename,
						long timeoutsec,
						long timeoutusec,
						unsigned long retrywait,
						unsigned long retrycount) {
	unixsocketutil::initialize(filename);
	this->timeoutsec=timeoutsec;
	this->timeoutusec=timeoutusec;
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

void unixclientsocket::initialize(namevaluepairs *cd) {

	if (cd) {
		char	*filename;
		cd->getData("filename",&filename);
		char	*timeoutsec;
		cd->getData("timeoutsec",&timeoutsec);
		char	*timeoutusec;
		cd->getData("timeoutusec",&timeoutusec);
		char	*retrywait;
		cd->getData("retrywait",&retrywait);
		char	*retrycount;
		cd->getData("retrycount",&retrycount);

		initialize(filename?filename:"",
			charstring::toLong(timeoutsec?timeoutsec:"0"),
			charstring::toLong(timeoutusec?timeoutusec:"0"),
			charstring::toUnsignedLong(retrywait?retrywait:"0"),
			charstring::toUnsignedLong(retrycount?retrycount:"0"));
	}
}

int unixclientsocket::connect() {

	// set the filename to connect to
	sockaddrun.sun_family=AF_UNIX;
	charstring::copy(sockaddrun.sun_path,filename);

	// create a unix socket
	do {
		fd=::socket(AF_UNIX,SOCK_STREAM,0);
	} while (fd==-1 && error::getErrorNumber()==EINTR);
	if (fd==-1) {
		return RESULT_ERROR;
	}

	// try to connect, over and over for the specified number of times
	for (unsigned long counter=0;
			counter<retrycount || !retrycount; counter++) {

		// wait the specified amount of time between reconnect tries
		// unless we're on the very first try
		if (counter) {
			snooze::macrosnooze(retrywait);
		}

		// attempt to connect
		if (clientsocket::connect(
			reinterpret_cast<struct sockaddr *>(&sockaddrun),
			sizeof(sockaddrun),
			timeoutsec,timeoutusec)==RESULT_SUCCESS) {
			return RESULT_SUCCESS;
		}
	}

	// if we're here, the connect failed
	close();
	return RESULT_ERROR;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
