// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PASSWDENTRY_H
#define RUDIMENTS_PASSWDENTRY_H

#include <rudiments/private/passwdentryincludes.h>

// The passwdentry class provides methods for retrieving
// entries from /etc/passwd

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class passwdentryprivate;

class passwdentry {
	public:

		// If you need to quickly look up ap specific field, use one
		// of these methods.
		//
		// These methods return true on success and false on failure.
		static bool	getName(uid_t userid, char **name);
		static bool	getPassword(uid_t userid, char **password);
		static bool	getPrimaryGroupId(uid_t userid, gid_t *groupid);
		static bool	getRealName(uid_t userid, char **realname);
		static bool	getHomeDirectory(uid_t userid, char **homedir);
		static bool	getShell(uid_t userid, char **shell);

		static bool	getPassword(const char *username,
							char **password);
		static bool	getUserId(const char *username,
							uid_t *userid);
		static bool	getPrimaryGroupId(const char *username,
							gid_t *groupid);
		static bool	getRealName(const char *username,
							char **realname);
		static bool	getHomeDirectory(const char *username,
							char **homedir);
		static bool	getShell(const char *username,
							char **shell);

		// If you need to look up a passwd entry and refer to multiple
		// fields, use these methods.
				passwdentry();
				passwdentry(const passwdentry &p);
		passwdentry	&operator=(const passwdentry &p);
				~passwdentry();

		bool	initialize(const char *username);
			// Looks up a passwd entry by name.
			// Returns true on success and false on failure.
		bool	initialize(uid_t userid);
			// Looks up a passwd entry by user id.
			// Returns true on success and false on failure.

		const char	*getName() const;
		const char	*getPassword() const;
		uid_t		getUserId() const;
		gid_t		getPrimaryGroupId() const;
		const char	*getRealName() const;
		const char	*getHomeDirectory() const;
		const char	*getShell() const;

		void	print() const;
			// Prints out the passwd entry.

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

	#include <rudiments/private/passwdentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
