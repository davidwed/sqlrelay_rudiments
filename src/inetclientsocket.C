// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetclientsocket.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/inetclientsocketinlines.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

void inetclientsocket::initialize(connectiondata *cd) {

	char	*host=cd->getValue("host");
	char	*port=cd->getValue("port");
	char	*retrywait=cd->getValue("retrywait");
	char	*retrycount=cd->getValue("retrycount");

	initialize(host?host:"",
			atoi(port?port:"0"),
			atoi(retrywait?retrywait:"0"),
			atoi(retrycount?retrycount:"0"));
}

int inetclientsocket::connect() {

	// get the host entry
#ifdef HAVE_GETHOSTBYNAME_R
	hostent	*hostentry=new hostent;
	char	hostbuffer[1024];
	int	errnop;
	if (gethostbyname_r(address,hostentry,hostbuffer,1024,
						&hostentry,&errnop)) {
		return 0;
	}
#else
	hostent	*hostentry=NULL;
	if (!(hostentry=gethostbyname(address))) {
		return 0;
	}
#endif

	// set the address type and port to connect to
	memset((void *)&sin,0,sizeof(sin));
	sin.sin_family=hostentry->h_addrtype;
	sin.sin_port=htons(port);

	// use tcp protocol
#ifdef HAVE_GETPROTOBYNAME_R
	protoent	*protocol=new protoent;
	char		protobuffer[1024];
	if (getprotobyname_r("tcp",protocol,protobuffer,1024,&protocol)) {
		#ifdef HAVE_GETHOSTBYNAME_R
			delete hostentry;
		#endif
		return 0;
	}
#else
	protoent	*protocol=NULL;
	if (!(protocol=getprotobyname("tcp"))) {
		#ifdef HAVE_GETHOSTBYNAME_R
			delete hostentry;
		#endif
		return 0;
	}
#endif

	// create an inet socket
	if ((fd=socket(AF_INET,SOCK_STREAM,protocol->p_proto))==-1) {
		fd=-1;
		#ifdef HAVE_GETPROTOBYNAME_R
			delete protocol;
		#endif
		return 0;
	}
	#ifdef HAVE_GETPROTOBYNAME_R
		delete protocol;
	#endif

	// if create failed, show this error

	// try to connect, over and over for the specified number of times
	for (int counter=0; counter<retrycount || !retrycount; counter++) {

		// try to connect to each of the addresses
		// that came back from the address lookup
		int	addressindex=0;
		while (hostentry->h_addr_list[addressindex]) {

			// set which host to connect to
			memcpy((void *)&sin.sin_addr,
				(void *)hostentry->h_addr_list[addressindex],
				hostentry->h_length);
			addressindex++;
	
			// attempt to connect
			if (::connect(fd,(struct sockaddr *)&sin,
							sizeof(sin))!=-1) {
					#ifdef HAVE_GETHOSTBYNAME_R
						delete hostentry;
					#endif
					return 1;
			}
		}

		// wait the specified amount of time between reconnect tries
		sleep(retrywait);
	}

	// if we're here, the connect failed
	close();

#ifdef HAVE_GETHOSTBYNAME_R
	delete hostentry;
#endif
	return 0;
}
