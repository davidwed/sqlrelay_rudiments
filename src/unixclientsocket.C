// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/unixclientsocket.h>
#include <rudiments/charstring.h>

#include <stdlib.h>
#include <unistd.h>

unixclientsocket::unixclientsocket() : clientsocket(), unixsocketutil() {}

unixclientsocket::~unixclientsocket() {}

int unixclientsocket::connect(const char *filename,
						long timeoutsec,
						long timeoutusec,
						unsigned int retrywait,
						unsigned int retrycount) {
	initialize(filename,timeoutsec,timeoutusec,retrywait,retrycount);
	return connect();
}

void unixclientsocket::initialize(const char *filename,
						long timeoutsec,
						long timeoutusec,
						unsigned int retrywait,
						unsigned int retrycount) {
	unixsocketutil::initialize(filename);
	this->timeoutsec=timeoutsec;
	this->timeoutusec=timeoutusec;
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

void unixclientsocket::initialize(namevaluepairs *cd) {

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
			atoi(timeoutsec?timeoutsec:"0"),
			atoi(timeoutusec?timeoutusec:"0"),
			atoi(retrywait?retrywait:"0"),
			atoi(retrycount?retrycount:"0"));
}

int unixclientsocket::connect() {

	// set the filename to connect to
	sockaddrun.sun_family=AF_UNIX;
	charstring::copy(sockaddrun.sun_path,filename);

	// create a unix socket
	if ((fd=::socket(AF_UNIX,SOCK_STREAM,0))==-1) {
		fd=-1;
		return RESULT_ERROR;
	}

	// try to connect, over and over for the specified number of times
	for (unsigned int counter=0;
			counter<retrycount || !retrycount; counter++) {

		// wait the specified amount of time between reconnect tries
		// unless we're on the very first try
		if (counter) {
			sleep(retrywait);
		}

		// attempt to connect
		if (clientsocket::connect((struct sockaddr *)&sockaddrun,
				sizeof(sockaddrun),
				timeoutsec,timeoutusec)==RESULT_SUCCESS) {
			return RESULT_SUCCESS;
		}
	}

	// if we're here, the connect failed
	close();
	return RESULT_ERROR;
}
