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

		/** Sets "password" to the encrypted password for
		 *  "username".  Note that "password" is allocated
		 *  internally and returned.  The calling program must free
		 *  the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getEncryptedPassword(const char *username,
							char **password);

		/** Sets "lstchg" to the last time the password for
		 *  "username" was changed.  The time is returned as the
		 *  number of seconds since 1970.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getLastChangeDate(const char *username,
							long *lstchg);

		/** Sets "min" to the number of days before the password
		 *  may be changed for "username".
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getDaysBeforeChangeAllowed(
						const char *username,
						int32_t *min);

		/** Sets "min" to the number of days before the password
		 *  is required to be changed for "username".
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getDaysBeforeChangeRequired(
						const char *username,
						int32_t *max);

		/** Sets "min" to the number of days before a warning
		 *  should be displayed that the password for "username"
		 *  will expire.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getDaysBeforeExpirationWarning(
						const char *username,
						int32_t *warn);

		/** Sets "inact" to the number of days of inactivity that
		 *  are allowed for "username" before the password will expire.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getDaysOfInactivityAllowed(
						const char *username,
						int32_t *inact);

		/** Sets "expire" to the date that the password for "username"
		 *  will expire.  The date is given as the number of seconds
		 *  since 1970.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getExpirationDate(const char *username,
							int32_t *expire);

		/** Sets "flag" to the system-specific set of flags that
		 *  are set for "username".
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFlag(const char *username,
						int32_t *flag);

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

		/** Prints out a representation of the shadow entry. */
		void	print() const;

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(mutex *mtx);

	#include <rudiments/private/shadowentry.h>
};

#endif
