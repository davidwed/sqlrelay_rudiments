// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENTSERVER_FACTORY_H
#define RUDIMENTS_CLIENTSERVER_FACTORY_H

#include <rudiments/private/clientserverfactoryincludes.h>

// The clientserverfactory class provides factory methods for getting clients
// and servers.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class DLLSPEC clientserverfactory {
	public:
		static client	*getClient(const char *type);
			// Returns an instance of a child of the client
			// class as follows:
			//
			//    type              class
			// "inet"             inetclientsocket
			// "unix"             unixclientsocket
			// anything else      NULL

		static server	*getServer(const char *type);
			// Returns an instance of a child of the server
			// class as follows:
			//
			//    type              class
			// "inet"             inetserversocket
			// "unix"             unixserversocket
			// anything else      NULL
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
