// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENT_H
#define RUDIMENTS_CLIENT_H

#include <rudiments/private/clientincludes.h>

class clientprivate;

/** The client class provides a base class for classes that implement the
 *  client side of a client-server communication model. */
class RUDIMENTS_DLLSPEC client : public filedescriptor {
	public:
		/** Creates an instance of the client class. */
		client();

		/** Creates an instance of the client class
		 *  that is a copy of "c". */
		client(const client &c);

		/** Makes this instance of the client class
		 *  identical to "c". */
		client	&operator=(const client &c);

		/** Deletes this instance of the client class. */
		virtual ~client();

		/** This method may be implemented by a child class to
		 *  initialize this instance with the parameters defined in
		 *  "cd".
		 *
		 *  This implementation does nothing. */
		virtual void	initialize(constnamevaluepairs *cd);

		/** This method may be implemnted by a child class to open a
		 *  connection to a server, presumably specified in the "cd"
		 *  parameter of the initalize() method.
		 *
		 *  This method return an int32_t instead of a bool so child
		 *  classes that have non atomic connection procedures (such
		 *  as a modem) can implement return codes other than success
		 *  or failure (such as abort).
		 *
		 *  This implementation just returns RESULT_SUCCESS. */
		virtual int32_t	connect();

		/** This method returns NULL by default but may be implemented
		 *  by a child class to return a detailed reason for a connect
		 *  failure. */
		virtual	const char	*getVerboseConnectError();

		/** This method may be used to set the verbose connect error. */
		virtual	void	setVerboseConnectError(const char *error);

	#include <rudiments/private/client.h>
};

#endif
