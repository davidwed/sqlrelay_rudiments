// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROTOCOLENTRY_H
#define RUDIMENTS_PROTOCOLENTRY_H

#include <rudiments/private/protocolentryincludes.h>

// The protocolentry class provides methods for retrieving
// entries from /etc/protocols

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class protocolentryprivate;

class protocolentry {
	public:

		// If you need to quickly look up a specific field, use one of
		// these methods.
		//
		// These methods return true on success and false on failure.
		static	bool	getAliasList(const char *protocolname,
						char ***aliaslist);
		static	bool	getNumber(const char *protocolname,
						int32_t *number);

		static	bool	getName(int32_t number, char **name);
		static	bool	getAliasList(int32_t number, char ***aliaslist);


		// If you need to look up a protocol entry and refer to multiple
		// fields, use these methods.
				protocolentry();
				protocolentry(const protocolentry &p);
		protocolentry	&operator=(const protocolentry &p);
				~protocolentry();

		bool	initialize(const char *protocolname);
			// Looks up a protocol entry by name.
			// Returns true on success and false on failure.
		bool	initialize(int32_t number);
			// Looks up a protocol entry by number.
			// Returns true on success and false on failure.

		const char		*getName() const;
		const char * const	*getAliasList() const;
		int32_t			getNumber() const;

		void	print() const;
			// Prints out the protocol entry.

		static	bool	needsMutex();
			// If your system doesn't support getprotobyname_r()
			// and getprotobynumber_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/protocolentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
