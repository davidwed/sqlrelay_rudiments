// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

class unixsocketclientprivate {
	friend class unixsocketclient;
	private:
};

unixsocketclient::unixsocketclient() : socketclient(), unixsocketutil() {
	pvt=new unixsocketclientprivate;
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
	}
	return *this;
}

unixsocketclient::~unixsocketclient() {
	delete pvt;
}

int32_t unixsocketclient::connect(const char *filename,
						int32_t timeoutsec,
						int32_t timeoutusec,
						uint32_t retrywait,
						uint32_t retrycount) {
	initialize(filename,timeoutsec,timeoutusec,retrywait,retrycount);
	return connect();
}

void unixsocketclient::initialize(const char *filename,
						int32_t timeoutsec,
						int32_t timeoutusec,
						uint32_t retrywait,
						uint32_t retrycount) {
	unixsocketutil::initialize(filename);
	client::initialize(NULL,timeoutsec,timeoutusec,retrywait,retrycount);
}

void unixsocketclient::initialize(constnamevaluepairs *cd) {

	if (cd) {
		const char	*filename;
		cd->getData("filename",&filename);
		const char	*timeoutsec;
		cd->getData("timeoutsec",&timeoutsec);
		const char	*timeoutusec;
		cd->getData("timeoutusec",&timeoutusec);
		const char	*retrywait;
		cd->getData("retrywait",&retrywait);
		const char	*retrycount;
		cd->getData("retrycount",&retrycount);

		initialize(filename?filename:"",
			charstring::toInteger(timeoutsec?timeoutsec:"0"),
			charstring::toInteger(timeoutusec?timeoutusec:"0"),
			charstring::toUnsignedInteger(retrywait?retrywait:"0"),
			charstring::toUnsignedInteger(retrycount?
							retrycount:"0"));
	}
}

int32_t unixsocketclient::connect() {

#ifdef _WIN32

	// Windows doesn't support unix sockets.  Ideally, I'd just let one
	// of the methods below fail but reportedly, with some compilers on
	// some versions of windows, the code below won't even compile.  Most
	// likely AF_UNIX isn't defined but I'm not sure so for now I'm just
	// disabling this for windows in general.
	return RESULT_ERROR;

#else

	// set the filename to connect to
	_sun()->sun_family=AF_UNIX;
	charstring::copy(_sun()->sun_path,_filename());

	// create a unix socket
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
	for (uint32_t counter=0;
			counter<_retrycount() || !_retrycount();
			counter++) {

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
