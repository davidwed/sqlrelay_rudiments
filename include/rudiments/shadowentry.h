// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHADOWENTRY_H
#define RUDIMENTS_SHADOWENTRY_H

#include <rudiments/private/shadowentryincludes.h>

// The shadowentry class provides methods for retrieving
// entries from /etc/shadow

class shadowentry {
	public:

		// if you need to quickly look up ap specific field, use one
		// of these mthods
		static int	getEncryptedPassword(const char *username,
							char **password);
		static int	getLastChangeDate(
						const char *username,
							long *lstchg);
		static int	getDaysBeforeChangeAllowed(
						const char *username,
							int *min);
		static int	getDaysBeforeChangeRequired(
						const char *username,
							int *max);
		static int	getDaysBeforeExpirationWarning(
						const char *username,
							int *warn);
		static int	getDaysOfInactivityAllowed(
						const char *username,
							int *inact);
		static int	getExpirationDate(
						const char *username,
							int *expire);
		static int	getFlag(const char *username, int *flag);

		// if you need to look up a shadow entry and refer to multiple
		// fields, use these methods
			shadowentry();
			~shadowentry();

		int	initialize(const char *username);
			// looks up a shadow entry by name

		char	*getName() const;
		char	*getEncryptedPassword() const;
		long	getLastChangeDate() const;
		int	getDaysBeforeChangeAllowed() const;
		int	getDaysBeforeChangeRequired() const;
		int	getDaysBeforeExpirationWarning() const;
		int	getDaysOfInactivityAllowed() const;
		int	getExpirationDate() const;
		int	getFlag() const;

		void	print() const;
			// prints out the shadow entry

		static	int	needsMutex();
			// If your system doesn't support getpwnam_r() and
			// getpwuid_r() then this class needs a mutex to assure
			// thread safety.
			//
			// This method returns 1 if this class needs a mutex to
			// operate safely in a threaded environment and 0
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/shadowentry.h>
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/shadowentryinlines.h>
#endif

#endif
