// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVER_H
#define RUDIMENTS_SERVER_H

#include <rudiments/private/serverincludes.h>

// The server class provides a base class for classes that implement the server
// side of a client-server communication model.

class server : virtual public datatransport {
	public:
		virtual	bool	bind();
		virtual	bool	listen(int backlog);
		virtual	bool	accept();
};

#endif
