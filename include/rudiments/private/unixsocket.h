// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKET_H
#define RUDIMENTS_UNIXSOCKET_H

#include <sys/types.h>
#include <sys/un.h>

#include <rudiments/private/socket.h>

class unixsocket : virtual public socket {
	public:
			unixsocket();
			unixsocket(int filedesc);
		void	initialize(const char *filename);

		bool	passFileDescriptor(int filedesc);
		bool	receiveFileDescriptor(int *filedesc);
	protected:
		char		*filename;
		sockaddr_un	sockaddrun;
};

#endif
