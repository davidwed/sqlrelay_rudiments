// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE void inetclientsocket::initialize(const char *host,
						unsigned short port,
						unsigned int retrywait,
						unsigned int retrycount) {
	inetsocket::initialize(host,port);
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}
