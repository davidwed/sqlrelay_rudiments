// Copyright (c) 2002-2014 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_USERENTRY_H
#define RUDIMENTS_USERENTRY_H

#include <rudiments/private/userentryincludes.h>

class userentryprivate;

/** The userentry class provides methods for retrieving information about
 *  users. */
class RUDIMENTS_DLLSPEC userentry {
	public:

		/** Creates an instance of the userentry class. */
		userentry();

		/** Creates an instance of the userentry class.
		 *  that is a copy of "u". */
		userentry(const userentry &u);

		/** Makes this instance of the userentry class
 		 *  identical to "u". */
		userentry	&operator=(const userentry &u);

		/** Deletes this instance of the userentry class. */
		~userentry();

		/** Looks up a user entry by name.
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *username);

		/** Looks up a user entry by user id.
		 *  Returns true on success and false on failure. */
		bool	initialize(uid_t userid);

		/** Returns the name of this user. */
		const char	*getName() const;

		/** Returns the encrypted password of this user. */
		const char	*getPassword() const;

		/** Returns the id of this user.
		 * 
		 *  Note: On platforms (like Windows) where the
		 *  platformSupportsFormalSid method returns true, users don't
		 *  have simple numeric ids.  On those platforms, the value
		 *  returned by this method is simply an index into an internal
		 *  structure that methods of other rudiments classes know how
		 *  to access and ultimately translate to a user.  It should
		 *  not be passed in to functions or methods of other libraries
		 *  that don't ultimately use rudiments methods. */
		uid_t		getUserId() const;

		/** Returns a string representation of the SID (security id) of
		 *  this user.
		 *
		 *  Note: On platforms (like non-Windows platforms) where the
		 *  platformSupportsFormalSid method returns false, the value
		 *  returned is just a string representation of the number
		 *  returned by getUserId(). */
		const char	*getSidString() const;
		const void	*getSid() const;
		uint64_t	getSidSize() const;

		/** Returns the primary group id of this user. */
		gid_t		getPrimaryGroupId() const;

		/** Returns the real name of this user. */
		const char	*getRealName() const;

		/** Returns the home directory of this user. */
		const char	*getHomeDirectory() const;

		/** Returns the shell of this user. */
		const char	*getShell() const;

		/** Returns the encrypted password of this user. */
		const char	*getEncryptedPassword() const;

		/** Returns the last time that the password for the user
		 *  was changed.  The time is given as the number of seconds
		 *  since 1970. */
		long		getLastChangeDate() const;

		/** Returns the number of days before the password for the
 		 *  user may be changed. */
		int32_t		getDaysBeforeChangeAllowed() const;

		/** Returns the number of days before the password for the
 		 *  user is required to be changed. */
		int32_t		getDaysBeforeChangeRequired() const;

		/** Returns the number of days before a warning that the
		 *  password for the user will expire should be displayed. */
		int32_t		getDaysBeforeExpirationWarning() const;

		/** Returns the number of days of inactivity that are allowed
		 *  for the user before the password will expire. */
		int32_t		getDaysOfInactivityAllowed() const;

		/** Returns the expiration date of the user.  The date
		 *  is returned as the number of seconds since 1970. */
		int32_t		getExpirationDate() const;

		/** Returns the system-specific set of flags for the user. */
		int32_t		getFlag() const;


		/** Convenience method.
		 *  Returns the name of the user specified by "userid".
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or if "userid" is invalid.
		 *  */
		static char	*getName(uid_t userid);

		/** Convenience method.
		 *  Returns the id of the user specified by "username".
		 *  See non-static version of this method for more information.
		 *
		 *  Returns -1 if an error occurred or if "username" is invalid.
		 *  */
		static uid_t	getUserId(const char *username);

		/** Convenience method.
		 *  Returns a string representation of the SID (security id) of
		 *  the user specified by "username".
		 *  See non-static version of this method for more information.
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or if "username" is
		 *  invalid.  */
		static char	*getSidString(const char *username);

		/** Returns true if the platform supports a formal user SID
		 *  (security id) and false if not.
		 *
		 *  Windows and windows-like platforms do.  Unix and unix-like
		 *  platforms (including Mac OS X) do not. */
		static bool	platformSupportsFormalSid();

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static void	setMutex(threadmutex *mtx);

	#include <rudiments/private/userentry.h>
};

#endif
