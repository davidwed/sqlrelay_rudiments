// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef CLIENT_H
#define CLIENT_H

#include <rudiments/datatransport.h>
#include <rudiments/connectiondata.h>

// The client class provides a base class for classes that implement the client
// side of a client-server communication model.

class client : virtual public datatransport {
	public:
		virtual void	initialize(connectiondata *cd)=0;
		virtual int	connect()=0;

	#include <rudiments/private/client.h>
};

#endif
