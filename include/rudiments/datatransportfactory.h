#ifndef DATATRANSPORT_FACTORY_H
#define DATATRANSPORT_FACTORY_H

#include <rudiments/client.h>
#include <rudiments/server.h>

// The datatransportfactory class provides factory methods for getting clients
// and servers.

class datatransportfactory {
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

#endif
