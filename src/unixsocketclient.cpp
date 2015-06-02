// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>
#if defined(_WIN32) || defined(__VMS)
	#include <rudiments/inetsocketclient.h>
#endif

#include <rudiments/private/winsock.h>

class unixsocketclientprivate {
	friend class unixsocketclient;
	private:
		#if defined(_WIN32) || defined(__VMS)
			inetsocketclient	_isc;
		#endif
};

unixsocketclient::unixsocketclient() : socketclient(), unixsocketutil() {
	pvt=new unixsocketclientprivate;
	#if defined(_WIN32) || defined(__VMS)
		translateByteOrder();
	#endif
	type("unixsocketclient");
}

unixsocketclient::unixsocketclient(const unixsocketclient &u) :
					socketclient(u), unixsocketutil(u) {
	pvt=new unixsocketclientprivate;
	type("unixsocketclient");
}

unixsocketclient &unixsocketclient::operator=(const unixsocketclient &u) {
	if (this!=&u) {
		socketclient::operator=(u);
		unixsocketutil::operator=(u);
		#if defined(_WIN32) || defined(__VMS)
			pvt->_isc=u.pvt->_isc;
		#endif
	}
	return *this;
}

unixsocketclient::~unixsocketclient() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class, especially on Windows where
	// closesocket() must be called rather than close() to prevent a crash.
	// If close() is called here, it will eventually call this method's
	// lowLevelClose() rather than filedescriptor::lowLevelClose().
	close();
	delete pvt;
	#if defined(_WIN32) || defined(__VMS)
		fd(-1);
	#endif
}

int32_t unixsocketclient::connect(const char *filename,
						int32_t timeoutsec,
						int32_t timeoutusec,
						uint32_t retrywait,
						uint32_t tries) {
	initialize(filename,timeoutsec,timeoutusec,retrywait,tries);
	return connect();
}

void unixsocketclient::initialize(const char *filename,
						int32_t timeoutsec,
						int32_t timeoutusec,
						uint32_t retrywait,
						uint32_t tries) {
	unixsocketutil::initialize(filename);
	client::initialize(NULL,timeoutsec,timeoutusec,retrywait,tries);
	#if defined(_WIN32) || defined(__VMS)
		pvt->_isc.initialize("127.0.0.1",
					filenameToPort(filename),
					timeoutsec,timeoutusec,
					retrywait,tries);
	#endif
}

void unixsocketclient::initialize(constnamevaluepairs *cd) {

	if (cd) {
		const char	*filename=NULL;
		cd->getValue("filename",&filename);
		const char	*timeoutsec=NULL;
		cd->getValue("timeoutsec",&timeoutsec);
		const char	*timeoutusec=NULL;
		cd->getValue("timeoutusec",&timeoutusec);
		const char	*retrywait=NULL;
		cd->getValue("retrywait",&retrywait);
		const char	*tries=NULL;
		cd->getValue("tries",&tries);

		initialize(filename?filename:"",
			charstring::toInteger(timeoutsec?timeoutsec:"0"),
			charstring::toInteger(timeoutusec?timeoutusec:"0"),
			charstring::toUnsignedInteger(retrywait?retrywait:"0"),
			charstring::toUnsignedInteger(tries?tries:"0"));
	}
}

int32_t unixsocketclient::connect() {

#if defined(_WIN32) || defined(__VMS)
	int32_t	result=pvt->_isc.connect();
	if (result>-1) {
		fd(pvt->_isc.getFileDescriptor());
	}
	return result;
#else
	// set the filename to connect to
	_sun()->sun_family=AF_UNIX;
	charstring::copy(_sun()->sun_path,_filename());

	// create a unix socket
	error::clearError();
	do {
		fd(::socket(AF_UNIX,SOCK_STREAM,0));
	} while (fd()==-1 && error::getErrorNumber()==EINTR);
	if (fd()==-1) {
		return RESULT_ERROR;
	}

	// Put the socket in blocking mode.  Most platforms create sockets in
	// blocking mode by default but OpenBSD doesn't appear to (at least in
	// version 4.9) so we'll force it to blocking-mode to be consistent.
	if (!useBlockingMode() &&
			error::getErrorNumber()
			#ifdef ENOTSUP
			&& error::getErrorNumber()!=ENOTSUP
			#endif
			#ifdef EOPNOTSUPP
			&& error::getErrorNumber()!=EOPNOTSUPP
			#endif
			) {
		close();
		return RESULT_ERROR;
	}

	int32_t	retval=RESULT_ERROR;

	// try to connect, over and over for the specified number of times
	for (uint32_t counter=0; counter<_tries() || !_tries(); counter++) {

		// wait the specified amount of time between reconnect tries
		// unless we're on the very first try
		if (counter) {
			snooze::macrosnooze(_retrywait());
		}

		// attempt to connect
		retval=socketclient::connect(
			reinterpret_cast<struct sockaddr *>(_sun()),
			sizeof(sockaddr_un),
			_timeoutsec(),_timeoutusec());
		if (retval==RESULT_SUCCESS) {
			return RESULT_SUCCESS;
		}
	}

	// if we're here, the connect failed
	close();
	return retval;
#endif
}

bool unixsocketclient::close() {
	#if defined(_WIN32) || defined(__VMS)
		pvt->_isc.close();
		fd(-1);
	#endif
	return filedescriptor::close();
}
