// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROTOCOLENTRY_H
#define RUDIMENTS_PROTOCOLENTRY_H

#include <rudiments/private/protocolentryincludes.h>

class protocolentryprivate;

/** The protocolentry class provides methods for retrieving entries from the
 *  protocol file (/etc/protocols on unix). */
class RUDIMENTS_DLLSPEC protocolentry {
	public:

		/** Creates an instance of the protocolentry class. */
		protocolentry();

		/** Creates an instance of the protocolentry class.
		 *  that is identical to "p". */
		protocolentry(const protocolentry &p);

		/** Makes this instance of the protocolentry class
		 *  identical to "p". */
		protocolentry	&operator=(const protocolentry &p);

		/** Deletes this instance of the protocolentry class. */
		~protocolentry();

		/** Looks up a protocol entry by name.
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *protocolname);

		/** Looks up a protocol entry by number.
		 *  Returns true on success and false on failure. */
		bool	initialize(int32_t number);

		/** Returns the name of the protocol entry. */
		const char		*getName() const;

		/** Returns a NULL-terminated list of aliases for the protocol
		 *  entry. */
		const char * const	*getAliasList() const;

		/** Returns the number associated with the protocol entry. */
		int32_t			getNumber() const;

		/** Convenience method.
		 *  Returns the number associated with "protocolname".
		 *
		 *  Returns -1 if an error occurred or if "protocolname"
		 *  is invalid. */
		static	int32_t	getNumber(const char *protocolname);

		/** Convenience method.
		 *  Returns the name of the protocol associated with "number".
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or if "number" is invalid.
		 *  */
		static	char	*getName(int32_t number);

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/protocolentry.h>
};

#endif
