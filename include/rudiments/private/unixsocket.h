// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKET_H
#define RUDIMENTS_UNIXSOCKET_H

#include <sys/types.h>
#include <sys/un.h>

#include <rudiments/datatransport.h>

class unixsocket : virtual public datatransport {
	public:
			unixsocket();
			unixsocket(int filedescriptor);
		void	initialize(const char *filename);

		bool	passFileDescriptor(int descriptor);
		bool	receiveFileDescriptor(int *descriptor);
	protected:
		char		*filename;
		sockaddr_un	sockaddrun;
		#ifdef RUDIMENTS_HAS_SSL
		BIO	*newSSLBIO();
		#endif
};

#endif
