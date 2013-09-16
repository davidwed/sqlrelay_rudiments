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

		/** Sets "name" to the name of the user specified by "userid".
		 *  Note that "name" is allocated internally and returned.  The
		 *  calling program must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getName(uid_t userid, char **name);

		/** Sets "password" to the password of the user specified by
		 *  "userid".  Note that "password" is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getPassword(uid_t userid, char **password);

		/** Sets "groupid" to the primary group id of the user
		 *  specified by "userid".
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getPrimaryGroupId(uid_t userid, gid_t *groupid);

		/** Sets "realname" to the real name of the user specified by
		 *  "userid".  Note that "realname" is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getRealName(uid_t userid, char **realname);

		/** Sets "homedir" to the full pathname of the home directory
		 *  of the user specified by "userid".  Note that "homedir" is
		 *  allocated internally and returned.  The calling program
		 *  must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getHomeDirectory(uid_t userid, char **homedir);

		/** Sets "shell" to the full pathname of the shell program
		 *  of the user specified by "userid".  Note that "shell" is
		 *  allocated internally and returned.  The calling program
		 *  must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getShell(uid_t userid, char **shell);


		/** Sets "password" to the password of the user specified by
		 *  "username".  Note that "password" is allocated internally
		 *  and returned.  The calling program must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getPassword(const char *username,
							char **password);

		/** Sets "userid" to the id of the user specified by "username".
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getUserId(const char *username,
							uid_t *userid);

		/** Sets "groupid" to the primary group id of the user
		 *  specified by "username".
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getPrimaryGroupId(const char *username,
							gid_t *groupid);

		/** Sets "realname" to the real name of the user specified by
		 *  "username".  Note that "realname" is allocated internally
		 *  and returned.  The calling program must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getRealName(const char *username,
							char **realname);

		/** Sets "homedir" to the full pathname of the home directory
		 *  of the user specified by "username".  Note that "homedir" is
		 *  allocated internally and returned.  The calling program
		 *  must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getHomeDirectory(const char *username,
							char **homedir);

		/** Sets "shell" to the full pathname of the shell program
		 *  of the user specified by "username".  Note that "shell" is
		 *  allocated internally and returned.  The calling program
		 *  must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getShell(const char *username,
							char **shell);

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

		/** Prints out a representation of the user entry. */
		void	print() const;

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
