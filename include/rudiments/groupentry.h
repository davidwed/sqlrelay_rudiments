// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_GROUPENTRY_H
#define RUDIMENTS_GROUPENTRY_H

#include <rudiments/private/groupentryincludes.h>

class groupentryprivate;

/** The groupentry class provides methods for retrieving information about
 *  user groups. */
class RUDIMENTS_DLLSPEC groupentry {
	public:

		/** Creates an instance of the groupentry class. */
		groupentry();

		/** Creates an instance of the groupentry class
		 *  that is a copy of "g". */
		groupentry(const groupentry &g);

		/** Makes this instance of the groupentry class
		 *  identical to "g". */
		groupentry	&operator=(const groupentry &g);

		/** Deletes this instance of the groupentry class. */
		~groupentry();

		/** Looks up a group entry by name.
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *groupname);

		/** Looks up a group entry by group id.
		 *  Returns true on success and false on failure. */
		bool	initialize(gid_t groupid);

		/** Returns the name of the group. */
		const char		*getName() const;

		/** Returns the password for the group. */
		const char		*getPassword() const;

		/** Returns the id of the group. */
		gid_t			getGroupId() const;

		/** Returns a NULL-terminated list of the
		 *  names of the members of the group. */
		const char * const	*getMembers() const;

		/** Convenience method.
		 *  Returns the id for "groupname".
		 *
		 *  Returns -1 if "groupname" was invalid or if an error
		 *  occurred. */
		static gid_t	getGroupId(const char *groupname);

		/** Convenience method.
		 *  Returns the name of "groupid".  Note that the return value
		 *  is allocated internally and returned.  The calling program
		 *  must free the buffer.
		 *
		 *  Returns NULL if "groupid" was invalid or if an error
		 *  occurred. */
		static char	*getName(gid_t groupid);


		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/groupentry.h>
};

#endif
