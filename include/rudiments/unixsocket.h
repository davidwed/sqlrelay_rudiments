// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKET_H
#define RUDIMENTS_UNIXSOCKET_H

#include <rudiments/private/unixsocketincludes.h>

class unixsocket : public socket {
	public:
			unixsocket();
			unixsocket(int filedesc);
		virtual ~unixsocket();

		void	initialize(const char *filename);

		bool	passFileDescriptor(int filedesc);
		bool	receiveFileDescriptor(int *filedesc);

	#include <rudiments/private/unixsocket.h>
};

#endif
