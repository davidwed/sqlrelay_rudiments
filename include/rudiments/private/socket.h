// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKET_H
#define RUDIMENTS_SOCKET_H

#include <rudiments/datatransport.h>

class socket : virtual public datatransport {
	public:
			socket();
			socket(int filedescriptor);
#ifdef RUDIMENTS_HAS_SSL
	protected:
		BIO	*newSSLBIO();
#endif
};

#endif
