// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HOSTENTRY_H
#define RUDIMENTS_HOSTENTRY_H

#include <rudiments/private/hostentryincludes.h>

class hostentryprivate;

/** The hostentry class provides methods for retrieving entries from the host
 *  file (/etc/hosts on unix). */
class RUDIMENTS_DLLSPEC hostentry {
	public:

		/** Creates an instance of the hostentry class. */
		hostentry();

		/** Creates an instance of the hostentry class
		 *  that is a copy of "h". */
		hostentry(const hostentry &h);

		/** Makes this instance of the hostentry class
		 *  identical to "h". */
		hostentry	&operator=(const hostentry &h);

		/** Deletes this instance of the hostentry class. */
		~hostentry();

		/** Looks up a host entry by name. */
		bool	initialize(const char *hostname);

		/** Looks up a host entry by address.
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required. */
		bool	initialize(const char *address,
					int32_t len, int32_t type);

		/** Returns the primary hostname of the host entry. */
		const char		*getName() const;

		/** Returns a NULL-terminated list of aliases for the
		 *  host entry. */
		const char * const	*getAliasList() const;

		/** Returns the address type of the host entry. */
		int32_t			getAddressType() const;

		/** Returns the address length of the host entry. */
		int32_t			getAddressLength() const;

		/** Returns a NULL-terminated list of addresses for the
		 *  host entry. */
		const char * const	*getAddressList() const;

		/** Returns the "index"'th address of the host entry.
		 *
		 *  Note that this method allocated a buffer internally and
		 *  returns it.  The calling program must free the buffer. */
		char	*getAddressString(int32_t index) const;

		/** Convenience method.
		 *  Returns the address of "hostname".
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or "hostname" is invalid.
		 *  */
		static	char	*getAddressString(const char *hostname);

		/** Convenience method.
		 *  Returns the primary hostname of "address" of length
		 *  "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or "address" is invalid.
		 *  */
		static	char	*getName(const char *address,
						int32_t len, int32_t type);

		/** Convenience method.
		 *  Returns a string representation of "address" of length
		 *  "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or "address" is invalid.
		 *  */
		static	char	*getAddressString(const char *address,
						int32_t len, int32_t type);

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/hostentry.h>
};

#endif
