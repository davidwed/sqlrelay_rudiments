// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_GROUPENTRY_H
#define RUDIMENTS_GROUPENTRY_H

#include <rudiments/private/groupentryincludes.h>

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

		static	int	needsMutex();
			// If your system doesn't support getgrnam_r() and
			// getgrgid_r() then this class needs a mutex to assure
			// thread safety.
			//
			// This method returns 1 if this class needs a mutex to
			// operate safely in a threaded environment and 0
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/groupentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/groupentryinlines.h>
#endif

#endif
