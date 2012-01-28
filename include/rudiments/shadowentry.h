// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHADOWENTRY_H
#define RUDIMENTS_SHADOWENTRY_H

#include <rudiments/private/shadowentryincludes.h>

// The shadowentry class provides methods for retrieving
// entries from /etc/shadow

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class shadowentryprivate;

class shadowentry {
	public:

		// If you need to quickly look up ap specific field, use one
		// of these mthods.
		//
		// These methods return true on success and false on failure.
		static bool	getEncryptedPassword(const char *username,
							char **password);
		static bool	getLastChangeDate(
						const char *username,
						long *lstchg);
		static bool	getDaysBeforeChangeAllowed(
						const char *username,
						int32_t *min);
		static bool	getDaysBeforeChangeRequired(
						const char *username,
						int32_t *max);
		static bool	getDaysBeforeExpirationWarning(
						const char *username,
						int32_t *warn);
		static bool	getDaysOfInactivityAllowed(
						const char *username,
						int32_t *inact);
		static bool	getExpirationDate(
						const char *username,
						int32_t *expire);
		static bool	getFlag(const char *username,
						int32_t *flag);

		// If you need to look up a shadow entry and refer to multiple
		// fields, use these methods.
				shadowentry();
				shadowentry(const shadowentry &s);
		shadowentry	&operator=(const shadowentry &s);
				~shadowentry();

		bool	initialize(const char *username);
			// Looks up a shadow entry by name.
			// Returns true on success and false on failure.

		const char	*getName() const;
		const char	*getEncryptedPassword() const;
		long		getLastChangeDate() const;
		int32_t		getDaysBeforeChangeAllowed() const;
		int32_t		getDaysBeforeChangeRequired() const;
		int32_t		getDaysBeforeExpirationWarning() const;
		int32_t		getDaysOfInactivityAllowed() const;
		int32_t		getExpirationDate() const;
		int32_t		getFlag() const;

		void	print() const;
			// Prints out the shadow entry.

		static	bool	needsMutex();
			// If your system doesn't support getpwnam_r() and
			// getpwuid_r() then this class needs a mutex to assure
			// thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/shadowentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
