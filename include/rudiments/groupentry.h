// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_GROUPENTRY_H
#define RUDIMENTS_GROUPENTRY_H

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

		static int	getPassword(const char *groupname,
							char **password);
		static int	getGroupId(const char *groupname,
							gid_t *groupid);
		static int	getMembers(const char *groupname,
							char ***members);

		static int	getName(gid_t groupid, char **name);
		static int	getPassword(gid_t groupid, char **password);
		static int	getMembers(gid_t groupid, char ***members);

	#include <rudiments/private/groupentry.h>
};

#ifdef ENABLE_INLINES
	#define INLINE inline
	#include <rudiments/private/groupentryinlines.h>
#endif

#endif
