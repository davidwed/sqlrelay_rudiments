// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline int	unixclientsocket::connectToServer(const char *filename,
				unsigned int retrywait, int retrycount) {
	initialize(filename,retrywait,retrycount);
	return connect();
}

inline void	unixclientsocket::initialize(const char *filename,
				unsigned int retrywait, int retrycount) {
	unixsocket::initialize(filename);
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

inline int	unixclientsocket::passFileDescriptor(int descriptor) {
	return unixsocket::passFileDescriptor(descriptor);
}

inline int	unixclientsocket::receiveFileDescriptor(int *descriptor) {
	return unixsocket::receiveFileDescriptor(descriptor);
}
