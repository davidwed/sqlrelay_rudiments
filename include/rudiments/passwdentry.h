// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PASSWDENTRY_H
#define RUDIMENTS_PASSWDENTRY_H

#include <rudiments/private/config.h>

#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
	#include <pthread.h>
#endif

#include <sys/types.h>
#include <pwd.h>

// The passwdentry class provides methods for retrieving
// entries from /etc/passwd

class passwdentry {
	public:

		// if you need to quickly look up ap specific field, use one
		// of these mthods
		static int	getName(uid_t userid, char **name);
		static int	getPassword(uid_t userid, char **password);
		static int	getPrimaryGroup(uid_t userid, gid_t *groupid);
		static int	getRealName(uid_t userid, char **realname);
		static int	getHomeDirectory(uid_t userid, char **homedir);
		static int	getShell(uid_t userid, char **shell);

		static int	getPassword(const char *username,
							char **password);
		static int	getUserId(const char *username,
							uid_t *userid);
		static int	getPrimaryGroup(const char *username,
							gid_t *groupid);
		static int	getRealName(const char *username,
							char **realname);
		static int	getHomeDirectory(const char *username,
							char **homedir);
		static int	getShell(const char *username,
							char **shell);

		// if you need to look up a passwd entry and refer to multiple
		// fields, use these methods
			passwdentry();
			~passwdentry();

		int	initialize(const char *username);
			// looks up a passwd entry by name
		int	initialize(uid_t userid);
			// looks up a passwd entry by user id

		char	*getName() const;
		char	*getPassword() const;
		uid_t	getUserId() const;
		gid_t	getPrimaryGroup() const;
		char	*getRealName() const;
		char	*getHomeDirectory() const;
		char	*getShell() const;

		void	print() const;
			// prints out the passwd entry

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

	#include <rudiments/private/passwdentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/passwdentryinlines.h>
#endif

#endif
