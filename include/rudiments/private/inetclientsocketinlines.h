// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdio.h>
#include <stdlib.h>

inline int inetclientsocket::connectToServer(const char *host,
					unsigned short port,
					unsigned int retrywait,
					int retrycount) {
	initialize(host,port,retrywait,retrycount);
	return connect();
}

inline void inetclientsocket::initialize(const char *host,
						unsigned short port,
						unsigned int retrywait,
						int retrycount) {
	inetsocket::initialize(host,port);
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}
