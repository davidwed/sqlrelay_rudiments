// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PASSWDENTRY_H
#define RUDIMENTS_PASSWDENTRY_H

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <pwd.h>

class passwdentry {
	public:
			passwdentry();
			~passwdentry();

		int	initialize(const char *username);
		int	initialize(uid_t userid);

		char	*getName() const;
		char	*getPassword() const;
		uid_t	getUserId() const;
		gid_t	getPrimaryGroup() const;
		char	*getRealName() const;
		char	*getHomeDirectory() const;
		char	*getShell() const;

		void	print() const;


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

	#include <rudiments/private/passwdentry.h>
};

#ifdef ENABLE_INLINES
	#define INLINE inline
	#include <rudiments/private/passwdentryinlines.h>
#endif

#endif
