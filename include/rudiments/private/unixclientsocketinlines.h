// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE void unixclientsocket::initialize(const char *filename,
						unsigned int retrywait,
						unsigned int retrycount) {
	unixsocket::initialize(filename);
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}
