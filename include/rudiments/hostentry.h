// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HOSTENTRY_H
#define RUDIMENTS_HOSTENTRY_H

#include <rudiments/private/hostentryincludes.h>

// The hostentry class provides methods for retrieving entries from /etc/hosts

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class hostentryprivate;

class RUDIMENTS_DLLSPEC hostentry {
	public:

		// If you need to quickly look up a specific field, use one of
		// these methods.
		//
		// These methods return true on success and false on failure.
		static	bool	getAliasList(const char *hostname,
						char ***aliaslist);
		static	bool	getAddressType(const char *hostname,
						int32_t *addresstype);
		static	bool	getAddressLength(const char *hostname,
						int32_t *addresslength);
		static	bool	getAddressList(const char *hostname,
						char ***addresslist);
		static	bool	getAddressString(const char *hostname,
						int32_t index,
						char **addressstring);

		static	bool	getName(const char *address,
						int32_t len, int32_t type,
						char **name);
		static	bool	getAliasList(const char *address,
						int32_t len, int32_t type,
						char ***aliaslist);
		static	bool	getAddressList(const char *address,
						int32_t len, int32_t type,
						char ***addresslist);
		static	bool	getAddressString(const char *address,
						int32_t len, int32_t type,
						int32_t index,
						char **addressstring);

		// If you need to look up a host entry and refer to multiple
		// fields, use these methods.
				hostentry();
				hostentry(const hostentry &h);
		hostentry	&operator=(const hostentry &h);
				~hostentry();

		bool	initialize(const char *hostname);
			// Looks up a host entry by name.
		bool	initialize(const char *address,
					int32_t len, int32_t type);
			// Looks up a host entry by address.

		const char		*getName() const;
		const char * const	*getAliasList() const;
		int32_t			getAddressType() const;
		int32_t			getAddressLength() const;
		const char * const	*getAddressList() const;
		char			*getAddressString(int32_t index) const;
					// This method allocates a buffer
					// internally and returns it.  The
					// calling program must deallocate the
					// buffer.

		void	print() const;
			// Prints out the host entry.

		static	bool	needsMutex();
			// If your system doesn't support gethostbyname_r()
			// and gethostbyaddr_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/hostentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
