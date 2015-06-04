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

		/** Returns the id of the group.
		 *
		 *  Note: On platforms (like Windows) where the
		 *  platformSupportsFormalSid method returns true, groups don't
		 *  have simple numeric ids.  On those platforms, the value
		 *  returned by this method is simply an index into an internal
		 *  structure that methods of other rudiments classes know how
		 *  to access and ultimately translate to a group.  It should
		 *  not be passed in to functions or methods of other libraries
		 *  that don't ultimately use rudiments methods. */
		gid_t		getGroupId() const;

		/** Returns a string representation of the SID (security id) of
		 *  this group.
		 *
		 *  Note: On platforms (like non-Windows platforms) where the
		 *  platformSupportsFormalSid method returns false, the value
		 *  returned is just a string representation of the number
		 *  returned by getGroupId(). */
		const char	*getSidString() const;
		const void	*getSid() const;
		uint64_t	getSidSize() const;

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

		/** Convenience method.
		 *  Returns a string representation of the SID (security id) of
		 *  the group specified by "groupname".
		 *  See non-static version of this method for more information.
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or if "groupname" is
		 *  invalid.  */
		static char	*getSidString(const char *groupname);

		/** Returns true if the platform supports a formal group SID
		 *  (security id) and false if not.
		 *
		 *  Windows and windows-like platforms do.  Unix and unix-like
		 *  platforms (including Mac OS X) do not. */
		static bool	platformSupportsFormalSid();

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
