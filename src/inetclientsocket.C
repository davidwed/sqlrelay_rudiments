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

void	inetclientsocket::initialize(connectiondata *cd) {

	char	*host=cd->getValue("host");
	char	*port=cd->getValue("port");
	char	*retrywait=cd->getValue("retrywait");
	char	*retrycount=cd->getValue("retrycount");

	initialize(host?host:"",
			atoi(port?port:"0"),
			atoi(retrywait?retrywait:"0"),
			atoi(retrycount?retrycount:"0"));
}

int	inetclientsocket::connect() {

	// get the host entry
	hostentry=NULL;
	if (!(hostentry=gethostbyname(address))) {
		return 0;
	}

	// set the address type and port to connect to
	memset((void *)&sin,0,sizeof(sin));
	sin.sin_family=hostentry->h_addrtype;
	sin.sin_port=htons(port);

	// use tcp protocol
	protoent	*protocol;
	if (!(protocol=getprotobyname("tcp"))) {
		return 0;
	}

	// create an inet socket
	if ((filedescriptor=socket(AF_INET,SOCK_STREAM,
						protocol->p_proto))==-1) {
		filedescriptor=-1;
		return 0;
	}

	// if create failed, show this error

	// try to connect, over and over for the specified number of times
	for (int counter=0; counter<retrycount || !retrycount; counter++) {

		// try to connect to each of the addresses
		// that came back from the address lookup
		int	addressindex=0;
		while (hostentry->h_addr_list[addressindex]) {

			// set which host to connect to
			bcopy((void *)hostentry->h_addr_list[addressindex],
				(void *)&sin.sin_addr,
				hostentry->h_length);
			addressindex++;
	
			// attempt to connect
			if (::connect(filedescriptor,
				(struct sockaddr *)&sin,sizeof(sin))!=-1) {
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
