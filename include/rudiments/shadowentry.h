// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHADOWENTRY_H
#define RUDIMENTS_SHADOWENTRY_H

#include <rudiments/private/shadowentryincludes.h>

class shadowentryprivate;

/** The shadowentry class provides methods for retrieving entries from the
 *  shadow password file (/etc/shadow on unix). */
class RUDIMENTS_DLLSPEC shadowentry {
	public:

		/** Creates an instance of the shadowentry class. */
		shadowentry();

		/** Creates an instance of the shadowentry class
		 *  that is a copy of "s". */
		shadowentry(const shadowentry &s);

		/** Makes this instance of the shadowentry class
		 *  identical to "s". */
		shadowentry	&operator=(const shadowentry &s);

		/** Deletes this instance of the shadowentry class. */
		~shadowentry();

		/** Looks up a shadow entry by name.
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *username);

		/** Returns the name of the shadow entry. */
		const char	*getName() const;

		/** Returns the encrypted password of the shadow entry. */
		const char	*getEncryptedPassword() const;

		/** Returns the last time that the password for the shadow
		 *  entry was changed.  The time is given as the number of
		 *  seconds since 1970. */
		long		getLastChangeDate() const;

		/** Returns the number of days before the password for the
 		 *  shadow entry may be changed. */
		int32_t		getDaysBeforeChangeAllowed() const;

		/** Returns the number of days before the password for the
 		 *  shadow entry is required to be changed. */
		int32_t		getDaysBeforeChangeRequired() const;

		/** Returns the number of days before a warning that the
		 *  password for the shadow entry will expire should be
		 *  displayed. */
		int32_t		getDaysBeforeExpirationWarning() const;

		/** Returns the number of days of inactivity that are allowed
		 *  for the shadow entry before the password will expire. */
		int32_t		getDaysOfInactivityAllowed() const;

		/** Returns the expiration date of the shadown entry.  The date
		 *  is returned as the number of seconds since 1970. */
		int32_t		getExpirationDate() const;

		/** Returns the system-specific set of flags for the shadow
		 *  entry. */
		int32_t		getFlag() const;

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/shadowentry.h>
};

#endif
