// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_GROUPENTRY_H
#define RUDIMENTS_GROUPENTRY_H

#include <rudiments/private/groupentryincludes.h>

// The groupentry class provides methods for retrieving entries from /etc/group

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class groupentryprivate;

class groupentry {
	public:

		// If you need to quickly look up a specific field, use one of
		// these methods.
		//
		// These methods return true on success and false on failure.
		static bool	getPassword(const char *groupname,
							char **password);
		static bool	getGroupId(const char *groupname,
							gid_t *groupid);
		static bool	getMembers(const char *groupname,
							char ***members);

		static bool	getName(gid_t groupid, char **name);
		static bool	getPassword(gid_t groupid, char **password);
		static bool	getMembers(gid_t groupid, char ***members);


		// If you need to look up a group entry and refer to multiple
		// fields, use these methods.
				groupentry();
				groupentry(const groupentry &g);
		groupentry	&operator=(const groupentry &g);
				~groupentry();

		bool	initialize(const char *groupname);
			// Looks up a group entry by name.
			// Returns true on success and false on failure.
		bool	initialize(gid_t groupid);
			// Looks up a group entry by group id.
			// Returns true on success and false on failure.

		const char		*getName() const;
		const char		*getPassword() const;
		gid_t			getGroupId() const;
		const char * const	*getMembers() const;


		void	print() const;
			// Prints out the group entry.

#ifdef RUDIMENTS_HAS_THREADS
		static	bool	needsMutex();
			// If your system doesn't support getgrnam_r() and
			// getgrgid_r() then this class needs a mutex to assure
			// thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.
#endif

	#include <rudiments/private/groupentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
