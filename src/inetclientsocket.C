// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetclientsocket.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/inetclientsocketinlines.h>
#endif

#include <rudiments/hostentry.h>
#include <rudiments/protocolentry.h>

#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

void inetclientsocket::initialize(namevaluepairs *cd) {

	char	*host;
	cd->getData("host",&host);
	char	*port;
	cd->getData("port",&port);
	char	*retrywait;
	cd->getData("retrywait",&retrywait);
	char	*retrycount;
	cd->getData("retrycount",&retrycount);

	initialize(host?host:"",
			atoi(port?port:"0"),
			atoi(retrywait?retrywait:"0"),
			atoi(retrycount?retrycount:"0"));
}

int inetclientsocket::connect() {

	// get the host entry
	hostentry	he;
	if (!he.initialize(address)) {
		return 0;
	}

	// set the address type and port to connect to
	memset((void *)&sin,0,sizeof(sin));
	sin.sin_family=he.getAddressType();
	sin.sin_port=htons(port);

	// use tcp protocol
	protocolentry	pe;
	if (!pe.initialize("tcp")) {
		return 0;
	}

	// create an inet socket
	if ((fd=socket(AF_INET,SOCK_STREAM,pe.getNumber()))==-1) {
		fd=-1;
		return 0;
	}

	// if create failed, show this error

	// try to connect, over and over for the specified number of times
	for (int counter=0; counter<retrycount || !retrycount; counter++) {

		// try to connect to each of the addresses
		// that came back from the address lookup
		for (int addressindex=0;
				he.getAddressList()[addressindex];
				addressindex++) {

			// set which host to connect to
			memcpy((void *)&sin.sin_addr,
				(void *)he.getAddressList()[addressindex],
				he.getAddressLength());
	
			// attempt to connect
			if (::connect(fd,(struct sockaddr *)&sin,
							sizeof(sin))!=-1) {
					return 1;
			}
		}

		// wait the specified amount of time between reconnect tries
		sleep(retrywait);
	}

	// if we're here, the connect failed
	close();

	return 0;
}
