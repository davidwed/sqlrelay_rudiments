// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENT_H
#define RUDIMENTS_CLIENT_H

#include <rudiments/private/clientincludes.h>

// The client class provides a base class for classes that implement the client
// side of a client-server communication model.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class client : public filedescriptor {
	public:

			client();
		virtual ~client();

		virtual void	initialize(namevaluepairs *cd)=0;
		virtual int	connect()=0;
				// This method return an int instead of a bool
				// because we want child classes that have non
				// atomic connection procedures (such as a
				// modem) to be able to implement return codes
				// other than success or failure (such as
				// abort).
		virtual	char	*getVerboseConnectError();

	#include <rudiments/private/client.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
