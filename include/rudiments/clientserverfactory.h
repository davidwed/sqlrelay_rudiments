// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENTSERVER_FACTORY_H
#define RUDIMENTS_CLIENTSERVER_FACTORY_H

#include <rudiments/private/clientserverfactoryincludes.h>

/** The clientserverfactory class provides factory methods for creating
 *  clients and servers. */
class RUDIMENTS_DLLSPEC clientserverfactory {
	public:
		/** Returns an instance of a child of the client
		 *  class as follows:
		 *	
		 *     type              class
		 *  "inet"             inetclientsocket
		 *  "unix"             unixclientsocket
		 *  "modem"            modemclient
		 *  anything else      NULL */
		static client	*getClient(const char *type);

		/** Returns an instance of a child of the server
		 *  class as follows:
		 * 
		 *     type              class
		 *  "inet"             inetserversocket
		 *  "unix"             unixserversocket
		 *  "modem"            modemserver
		 *  anything else      NULL */
		static server	*getServer(const char *type);
};

#endif
