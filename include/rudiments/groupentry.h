// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_GROUPENTRY_H
#define RUDIMENTS_GROUPENTRY_H

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <grp.h>

// The groupentry class provides methods for retrieving entries from /etc/group

class groupentry {
	public:

		// if you need to quickly look up a specific field, use one of
		// these methods
		static int	getPassword(const char *groupname,
							char **password);
		static int	getGroupId(const char *groupname,
							gid_t *groupid);
		static int	getMembers(const char *groupname,
							char ***members);

		static int	getName(gid_t groupid, char **name);
		static int	getPassword(gid_t groupid, char **password);
		static int	getMembers(gid_t groupid, char ***members);


		// if you need to look up a group entry and refer to multiple
		// fields, use these methods
			groupentry();
			~groupentry();

		int	initialize(const char *groupname);
			// looks up a group entry by name
		int	initialize(gid_t groupid);
			// looks up a group entry by group id

		char	*getName() const;
		char	*getPassword() const;
		gid_t	getGroupId() const;
		char	**getMembers() const;


		void	print() const;
			// prints out the group entry

	#include <rudiments/private/groupentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/groupentryinlines.h>
#endif

#endif
