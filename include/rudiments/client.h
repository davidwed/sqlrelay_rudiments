// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENT_H
#define RUDIMENTS_CLIENT_H

#include <rudiments/private/clientincludes.h>

// The client class provides a base class for classes that implement the client
// side of a client-server communication model.

class client : virtual public datatransport {
	public:
		virtual void	initialize(namevaluepairs *cd)=0;
		virtual bool	connect()=0;

	#include <rudiments/private/client.h>
};

#endif
