// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVER_H
#define RUDIMENTS_SERVER_H

#include <rudiments/private/serverincludes.h>

// The server class provides a base class for classes that implement the server
// side of a client-server communication model.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serverprivate;

class server : public filedescriptor {
	public:
			server();
			server(const server &s);
		server	&operator=(const server &s);
		virtual	~server();

		virtual	bool	bind()=0;
		virtual	bool	listen(int backlog)=0;
		virtual	filedescriptor	*accept()=0;

	#include <rudiments/private/server.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
