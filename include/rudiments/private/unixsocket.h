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

		int	passFileDescriptor(int descriptor);
		int	receiveFileDescriptor(int *descriptor);
	protected:
		char		*filename;
		sockaddr_un	sun;
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/unixsocketinlines.h>
#endif

#endif
