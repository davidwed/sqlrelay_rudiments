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

	#include <rudiments/private/groupentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/groupentryinlines.h>
#endif

#endif
