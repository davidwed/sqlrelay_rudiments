// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef PASSWDENTRY_H
#define PASSWDENTRY_H

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

	#include <rudiments/private/passwdentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/passwdentryinlines.h>
#endif

#endif
