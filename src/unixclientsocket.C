// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixclientsocket.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/unixclientsocketinlines.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

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

bool unixclientsocket::connect() {

	// set the filename to connect to
	sockaddrun.sun_family=AF_UNIX;
	strcpy(sockaddrun.sun_path,filename);

	// create a unix socket
	if ((fd=socket(AF_UNIX,SOCK_STREAM,0))==-1) {
		fd=-1;
		return false;
	}

	// try to connect, over and over for the specified number of times
	for (int counter=0; counter<retrycount || !retrycount; counter++) {

		// attempt to connect
		if (::connect(fd,(struct sockaddr *)&sockaddrun,
						sizeof(sockaddrun))!=-1) {
			return true;
		}

		// wait the specified amount of time between reconnect tries
		sleep(retrywait);
	}

	// if we're here, the connect failed
	close();
	return false;
}
