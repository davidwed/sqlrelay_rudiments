// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetclientsocket.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/inetclientsocketinlines.h>
#endif

#include <rudiments/hostentry.h>
#include <rudiments/protocolentry.h>
#ifdef HAVE_GETADDRINFO
	#include <rudiments/string.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

int inetclientsocket::connectToServer(const char *host,
						unsigned short port,
						unsigned int retrywait,
						unsigned int retrycount) {
	initialize(host,port,retrywait,retrycount);
	return connect();
}

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

	#ifndef HAVE_GETADDRINFO

		// get the host entry
		hostentry	he;
		if (!he.initialize(address)) {
			return RESULT_ERROR;
		}

		// use tcp protocol
		protocolentry	pe;
		if (!pe.initialize("tcp")) {
			return RESULT_ERROR;
		}

		// set the address type and port to connect to
		memset((void *)&sin,0,sizeof(sin));
		sin.sin_family=he.getAddressType();
		sin.sin_port=htons(port);

		// create an inet socket
		if ((fd=socket(AF_INET,SOCK_STREAM,pe.getNumber()))==-1) {
			fd=-1;
			return RESULT_ERROR;
		}

	#else

		// create a hint indicating that SOCK_STREAM should be used
		addrinfo	hints;
		memset(&hints,0,sizeof(addrinfo));
		hints.ai_socktype=SOCK_STREAM;

		// get a string representing the port number
		char	*portstr=string::parseNumber((long)port);

		// get the address info for the given address/port
		addrinfo	*ai;
		if (getaddrinfo(address,portstr,&hints,&ai)) {
			delete[] portstr;
			return RESULT_ERROR;
		}
		delete[] portstr;

	#endif

	// if create failed, show this error

	// try to connect, over and over for the specified number of times
	for (unsigned int counter=0;
			counter<retrycount || !retrycount; counter++) {

		#ifndef HAVE_GETADDRINFO

			// try to connect to each of the addresses
			// that came back from the address lookup
			for (int addressindex=0;
					he.getAddressList()[addressindex];
					addressindex++) {

				// set which host to connect to
				memcpy((void *)&sin.sin_addr,
					(void *)he.getAddressList()
							[addressindex],
					he.getAddressLength());
	
				// attempt to connect
				if (::connect(fd,(struct sockaddr *)&sin,
							sizeof(sin))!=-1) {
					return RESULT_SUCCESS;
				}
			}

		#else

			// try to connect to each of the addresses
			// that came back from the address lookup
			for (addrinfo *ainfo=ai; ainfo; ainfo=ainfo->ai_next) {

				// create an inet socket
				if ((fd=socket(ai->ai_family,
						ai->ai_socktype,
						ai->ai_protocol))==-1) {
					continue;
				}

				// attempt to connect to the socket
				if (::connect(fd,
					(struct sockaddr *)ainfo->ai_addr,
						ainfo->ai_addrlen)==-1) {
					close();
				} else {
					freeaddrinfo(ai);
					return RESULT_SUCCESS;
				}
			}

		#endif

		// wait the specified amount of time between reconnect tries
		sleep(retrywait);
	}

	// if we're here, the connect failed
	#ifdef HAVE_GETADDRINFO
		freeaddrinfo(ai);
	#else
		close();
	#endif

	return RESULT_ERROR;
}
