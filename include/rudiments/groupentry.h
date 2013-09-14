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

		/** Sets "password" to the password for "groupname".  Note that
		 *  "password" is allocated internally and returned.  The
		 *  calling program must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getPassword(const char *groupname,
							char **password);

		/** Sets "groupid" to the id for "groupname".
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getGroupId(const char *groupname,
							gid_t *groupid);

		/** Sets "members" to a NULL-terminated list of the names of
		 *  the users in "groupname".  Note that "members" is allocated
		 *  internally and returned.  The calling program must
		 *  free each element and the array itself.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getMembers(const char *groupname,
							char ***members);

		/** Sets "name" to the name of "groupid".  Note that "name" is
		 *  allocated internally and returned.  The calling program
		 *  must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getName(gid_t groupid, char **name);

		/** Sets "password" to the password for "groupid".  Note that
		 *  "password" is allocated internally and returned.  The
		 *  calling program must free the buffer.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getPassword(gid_t groupid, char **password);

		/** Sets "members" to a NULL-terminated list of the names of
		 *  the users in "groupid".  Note that "members" is allocated
		 *  internally and returned.  The calling program must
		 *  free each element and the array itself.
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getMembers(gid_t groupid, char ***members);


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


		/**  Prints out a representation of the group entry. */
		void	print() const;

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(mutex *mtx);

	#include <rudiments/private/groupentry.h>
};

#endif
