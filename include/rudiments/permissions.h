// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PERMISSIONS_H
#define RUDIMENTS_PERMISSIONS_H

#include <rudiments/private/permissionsincludes.h>

/** Permissions are access priveleges.  Files, directories, semaphores and
 *  shared memory segments all have permissions associated with them.
 * 
 *  There are 3 types of permissions: read, write and execute.
 * 
 *  These permissions can be granted to:
 *  	1. The user that owns the object.
 *  	2. Other users in the group of the user that owns the object.
 *  	3. All other users.
 * 
 *  The execute permission has 2 modifications which can be applied to it:
 *  	1. Set user id - the program will run as the user that owns
 * 			it instead of as the user that ran it
 *  	1. Set group id - the program will run as the group that owns
 * 			it instead of as the group of the user that ran it
 * 
 *  The "sticky bit" may also be set.  When set, the file is saved in the
 *  system's swap space. As a side-effect, it cannot be deleted by a user other
 *  than the one that created it. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class RUDIMENTS_DLLSPEC permissions {
	public:
		/** Set the permissions on "filename" to "perms".
		 * 
		 *  Returns true on success and false on failure. */
		static	bool	setFilePermissions(const char *filename,
							mode_t perms);

		/** Set the permissions on the file associated
		 *  with file descriptor "fd" to "perms".
		 *  
		 *  Returns true on success and false on failure. */
		static	bool	setFilePermissions(int32_t fd, mode_t perms);

		/** Return a permission defined in "permstring".
		 * 
		 *  "permstring" should be the same format as
		 *  displayed by the "ls -l" command.
		 * 
		 *  The first 3 characters set permissions for 
		 *  the owner.
		 *  The next 3 characters set permissions for 
		 *  the group.
		 *  The next 3 characters set permissions for 
		 *  all others.
		 * 
		 *  For example:
		 * 	rwxr-xr-x : read/execute for everyone
		 * 			and write for the owner
		 * 	rw-rw-r-- : read for everyone and
		 * 			write for the owner and
		 * 			owner's group
		 * 	rw-r--r-- : read for everyone and
		 * 			write for the owner
		 * 	rwsr-sr-x : read/execute for everyone,
		 * 			write for the owner and
		 * 			run as the user/group
		 * 			that owns the file
		 * 	rw-rw-rwt : read/write for everyone,
		 * 			and save the file in
		 * 			swap space */
		static	mode_t	evalPermString(const char *permstring);

		/**  FIXME: document this */
		static	char	*evalPermOctal(mode_t mode);
		
		/** Returns rw-rw-rw- (666) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	everyoneReadWrite();

		/** Returns rwxrwxrwx (777) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	everyoneReadWriteExecute();

		/** Returns r-------- (400) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	ownerRead();

		/** Returns -w------- (200) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	ownerWrite();

		/** Returns --x------ (100) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	ownerExecute();

		/** Returns rw------- (600) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	ownerReadWrite();

		/** Returns r-x------ (500) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	ownerReadExecute();

		/** Returns rwx------ (700) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	ownerReadWriteExecute();

		/** Returns ---r----- (040) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	groupRead();

		/** Returns ----w---- (020) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	groupWrite();

		/** Returns -----x--- (010) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	groupExecute();

		/** Returns ---rw---- (060) permissions.
		 *
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	groupReadWrite();

		/** Returns ---r-x--- (050) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	groupReadExecute();

		/** Returns ---rwx--- (070) permissions.
		 *
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	groupReadWriteExecute();

		/** Returns ------r-- (004) permissions.
		 *
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	othersRead();

		/** Returns -------w- (002) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	othersWrite();

		/** Returns --------x (001) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	othersExecute();

		/** Returns ------rw- (006) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	othersReadWrite();

		/** Returns ------r-x (005) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	othersReadExecute();

		/** Returns ------rwx (007) permissions.
		 *
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	othersReadWriteExecute();

		/** Returns --------t permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	saveInSwapSpace();

		/** Returns --s------ (u+s) permissions.
		 * 
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	setUserId();

		/** Returns -----s--- (g+s) permissions.
		 *
		 *  May be or'ed together with the result of another method
		 *  to define a permission. */
		static	mode_t	setGroupId();
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
