// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef UNIXSOCKET_H
#define UNIXSOCKET_H

#include <sys/un.h>

#include <rudiments/transport.h>

class unixsocket : virtual public transport {
	public:
			unixsocket();
			unixsocket(int filedescriptor);
		void	initialize(const char *filename);
		int	passFileDescriptor(int descriptor);
		int	receiveFileDescriptor(int *descriptor);
	protected:
		char		*filename;
		sockaddr_un	sun;
};

#ifdef USE_INLINES
	#include <rudiments/private/unixsocketinlines.h>
#endif

#endif
