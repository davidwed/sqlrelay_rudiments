// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

INLINE int unixclientsocket::connectToServer(const char *filename,
				unsigned int retrywait, int retrycount) {
	initialize(filename,retrywait,retrycount);
	return connect();
}

INLINE void unixclientsocket::initialize(const char *filename,
				unsigned int retrywait, int retrycount) {
	unixsocket::initialize(filename);
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}
