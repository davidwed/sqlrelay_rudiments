// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HOSTENTRY_H
#define RUDIMENTS_HOSTENTRY_H

#include <rudiments/private/hostentryincludes.h>

/** The hostentry class provides methods for retrieving entries from the host
 *  file (/etc/hosts on unix). */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class hostentryprivate;

class RUDIMENTS_DLLSPEC hostentry {
	public:

		/** Sets "aliaslist" to a NULL-terminated list of aliases for
		 *  "hostname".  Note that "aliaslist" is allocated internally
		 *  and returned.  The calling program must free each element
		 *  and the array itself.
		 *  
		 *  Returns true on success and false otherwise. */
		static	bool	getAliasList(const char *hostname,
						char ***aliaslist);

		/** Sets "addresstype" to the address type of "hostname". */
		static	bool	getAddressType(const char *hostname,
						int32_t *addresstype);

		/** Sets "addresslength" to the length of the address for
		 *  "hostname". */
		static	bool	getAddressLength(const char *hostname,
						int32_t *addresslength);

		/** Sets "addresslist" to a NULL-terminated list of addresses
		 *  for "hostname".
		 *
		 *  Note that "addresslist" is allocated
		 *  internally and returned.  The calling program must free
		 *  each element and the array itself.
		 *
		 *  Returns true on success and false otherwise. */
		static	bool	getAddressList(const char *hostname,
						char ***addresslist);

		/** Sets "addressstring" to the "index"'th address of
		 *  "hostname".  Note that "addressstring" is allocated
		 *  internally and returned.  The calling program must free
		 *  the buffer. */
		static	bool	getAddressString(const char *hostname,
						int32_t index,
						char **addressstring);

		/** Sets "name" to the primary hostname of "address" of length
		 *  "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that "name" is allocated internally and returned.  The
		 *  calling program must free the buffer. */
		static	bool	getName(const char *address,
						int32_t len, int32_t type,
						char **name);

		/** Sets "aliaslist" to a NULL-terminated list of aliases for
		 *  "address" of length "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that "aliaslist" is allocated internally and returned.
		 *  The calling program must free each element and the array
		 *  itself. */
		static	bool	getAliasList(const char *address,
						int32_t len, int32_t type,
						char ***aliaslist);

		/** Sets "addresslist" to a NULL-terminated list of addresses
		 *  for  "address" of length "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that "addresslist" is allocated internally and
		 *  returned.  The calling program must free each element and
		 *  the array itself. */
		static	bool	getAddressList(const char *address,
						int32_t len, int32_t type,
						char ***addresslist);

		/** Sets "addressstring" to a string representation of
		 *  "address" of length "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that "addressstring" is allocated internally and
		 *  returned.  The calling program must free the buffer. */
		static	bool	getAddressString(const char *address,
						int32_t len, int32_t type,
						int32_t index,
						char **addressstring);

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

		/** Prints out the representation of the host entry. */
		void	print() const;

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(mutex *mtx);

	#include <rudiments/private/hostentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
