// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixclientsocket.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/unixclientsocketinlines.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

void unixclientsocket::initialize(connectiondata *cd) {

	char	*filename=cd->getValue("filename");
	char	*retrywait=cd->getValue("retrywait");
	char	*retrycount=cd->getValue("retrycount");

	initialize(filename?filename:"",
			atoi(retrywait?retrywait:"0"),
			atoi(retrycount?retrycount:"0"));
}

int unixclientsocket::connect() {

	// set the filename to connect to
	sun.sun_family=AF_UNIX;
	strcpy(sun.sun_path,filename);

	// create a unix socket
	if ((fd=socket(AF_UNIX,SOCK_STREAM,0))==-1) {
		fd=-1;
		return 0;
	}

	// try to connect, over and over for the specified number of times
	for (int counter=0; counter<retrycount || !retrycount; counter++) {

		// attempt to connect
		if (::connect(fd,(struct sockaddr *)&sun,sizeof(sun))!=-1) {
			return 1;
		}

		// wait the specified amount of time between reconnect tries
		sleep(retrywait);
	}

	// if we're here, the connect failed
	close();
	return 0;
}
