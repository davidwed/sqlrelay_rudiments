// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixclientsocket.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

int unixclientsocket::connectToServer(const char *filename,
						unsigned int retrywait,
						unsigned int retrycount) {
	initialize(filename,retrywait,retrycount);
	return connect();
}

void unixclientsocket::initialize(const char *filename,
						unsigned int retrywait,
						unsigned int retrycount) {
	unixsocket::initialize(filename);
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

void unixclientsocket::initialize(namevaluepairs *cd) {

	char	*filename;
	cd->getData("filename",&filename);
	char	*retrywait;
	cd->getData("retrywait",&retrywait);
	char	*retrycount;
	cd->getData("retrycount",&retrycount);

	initialize(filename?filename:"",
			atoi(retrywait?retrywait:"0"),
			atoi(retrycount?retrycount:"0"));
}

int unixclientsocket::connect() {

	// set the filename to connect to
	sockaddrun.sun_family=AF_UNIX;
	strcpy(sockaddrun.sun_path,filename);

	// create a unix socket
	if ((fd=socket(AF_UNIX,SOCK_STREAM,0))==-1) {
		fd=-1;
		return RESULT_ERROR;
	}

	// try to connect, over and over for the specified number of times
	for (unsigned int counter=0;
			counter<retrycount || !retrycount; counter++) {

		// attempt to connect
		if (::connect(fd,(struct sockaddr *)&sockaddrun,
						sizeof(sockaddrun))!=-1) {
			#ifdef RUDIMENTS_HAS_SSL
			if (ctx) {
				if (!initializeSSL() ||
					SSL_connect(ssl)!=1) {
					close();
					return RESULT_ERROR;
				}
			}
			#endif
			return RESULT_SUCCESS;
		}

		// wait the specified amount of time between reconnect tries
		sleep(retrywait);
	}

	// if we're here, the connect failed
	close();
	return RESULT_ERROR;
}
