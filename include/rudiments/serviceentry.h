// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVICEENTRY_H
#define RUDIMENTS_SERVICEENTRY_H

#include <rudiments/private/serviceentryincludes.h>

class serviceentryprivate;

/** The serviceentry class provides methods for retrieving entries from the
 *  service file (/etc/services on unix). */
class RUDIMENTS_DLLSPEC serviceentry {
	public:

		/** Creates an instance of the serviceentry class. */
		serviceentry();

		/** Creates an instance of the serviceentry class that is
		 *  a copy of "s". */
		serviceentry(const serviceentry &s);

		/** Makes this instance of the serviceentry class
		 *  identical to "s". */
		serviceentry	&operator=(const serviceentry &s);

		/** Deletes this instance of the serviceentry class. */
		~serviceentry();

		/** Looks up a service entry by "servicename" and "protocol"
		 *  (tcp, udp, etc.).
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *servicename,
					const char *protocol);

		/** Looks up a service entry by the "port" that a server for
		 *  would listen on and the "protocol" that it would use
		 *  (tcp, udp, etc.).
		 *  Returns true on success and false on failure. */
		bool	initialize(int32_t port, const char *protocol);

		/** Returns the name of the service entry. */
		const char		*getName() const;

		/** Returns the port that a server for this service entry
 		 *  would listen on. */
		int32_t			getPort() const;

		/** Returns the protocol (tcp, udp, etc.) for this service
		 *  entry. */
		const char		*getProtocol() const;

		/** Returns a NULL-terminated list of aliases for the service
		 *  entry. */
		const char * const *	getAliasList() const;

		/** Convenience method.
		 *  Returns the port that the server for "servicename"
		 *  using "protocol" (tcp, udp, etc.) would listen on.
		 *
		 *  Returns -1 if an error occurred or one of the paramters
		 *  was invalid. */
		static	int32_t	getPort(const char *servicename,
						const char *protocol);

		/** Convenience method.
		 *  Sets "name" to the name of the service that a server
		 *  listening on "port", using "protocol" (tcp, udp, etc.)
		 *  would be serving.  Note that "name" is allocated internally
		 *  and returned.  The calling program must free the buffer.
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or one of the paramters
		 *  was invalid. */
		static	char	*getName(int32_t port,
						const char *protocol);

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/serviceentry.h>
};

#endif
