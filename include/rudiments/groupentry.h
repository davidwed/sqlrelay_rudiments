// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef GROUPENTRY_H
#define GROUPENTRY_H

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <grp.h>

class groupentry {
	public:
			groupentry();
			~groupentry();

		int	initialize(const char *groupname);
		int	initialize(gid_t groupid);

		char	*getName() const;
		char	*getPassword() const;
		gid_t	getGroupId() const;
		char	**getMembers() const;

		void	print() const;

		static char	*getPassword(const char *groupname);
		static gid_t	getGroupId(const char *groupname);
		static char	**getMembers(const char *groupname);

		static char	*getName(gid_t groupid);
		static char	*getPassword(gid_t groupid);
		static char	**getMembers(gid_t groupid);

	#include <rudiments/private/groupentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/groupentryinlines.h>
#endif

#endif
