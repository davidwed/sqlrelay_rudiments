// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PASSWDENTRY_H
#define RUDIMENTS_PASSWDENTRY_H

#include <rudiments/private/passwdentryincludes.h>

class passwdentryprivate;

/** The passwdentry class provides methods for retrieving information about
 *  users. */
class RUDIMENTS_DLLSPEC passwdentry {
	public:

		/** Creates an instance of the passwdentry class. */
		passwdentry();

		/** Creates an instance of the passwdentry class.
		 *  that is a copy of "p". */
		passwdentry(const passwdentry &p);

		/** Makes this instance of the passwdentry class
 		 *  identical to "p". */
		passwdentry	&operator=(const passwdentry &p);

		/** Deletes this instance of the passwdentry class. */
		~passwdentry();

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

		/** Returns the id of this user. */
		uid_t		getUserId() const;

		/** Returns the primary group id of this user. */
		gid_t		getPrimaryGroupId() const;

		/** Returns the real name of this user. */
		const char	*getRealName() const;

		/** Returns the home directory of this user. */
		const char	*getHomeDirectory() const;

		/** Returns the shell of this user. */
		const char	*getShell() const;

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
		 *
		 *  Returns -1 if an error occurred or if "username" is invalid.
		 *  */
		static uid_t	getUserId(const char *username);

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/passwdentry.h>
};

#endif
