// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PERMISSIONS_H
#define RUDIMENTS_PERMISSIONS_H

#include <rudiments/private/permissionsincludes.h>

// Permissions are access priveleges.  Files, directories, semaphores and
// shared memory segments all have permissions associated with them.
//
// There are 3 types of permissions: read, write and execute.
//
// These permissions can be granted to:
// 	1. The user that owns the object.
// 	2. Other users in the group of the user that owns the object.
// 	3. All other users.
//
// The execute permission has 2 modifications which can be applied to it:
// 	1. Set user id - the program will run as the user that owns
//			it instead of as the user that ran it
// 	1. Set group id - the program will run as the group that owns
//			it instead of as the group of the user that ran it
//
// The "sticky bit" may also be set.  When set, the file is saved in the
// system's swap space. As a side-effect, it cannot be deleted by a user other
// than the one that created it.

class permissions {
	public:
		static	bool	setFilePermissions(const char *filename,
							mode_t perms);
				// Set the permissions on "filename" to "perms".
				//
				// Returns true on success and false on failure.
		static	bool	setFilePermissions(int fd, mode_t perms);
				// Set the permissions on the file associated
				// with file descriptor "fd" to "perms".
				// 
				// Returns true on success and false on failure.

		static	mode_t	evalPermString(const char *permstring);
				// Return a permission defined in "permstring".
				//
				// "permstring" should be the same format as
				// displayed by the "ls -l" command.
				//
				// The first 3 characters set permissions for 
				// the owner.
				// The next 3 characters set permissions for 
				// the group.
				// The next 3 characters set permissions for 
				// all others.
				//
				// For example:
				//	rwxr-xr-x : read/execute for everyone
				//			and write for the owner
				//	rw-rw-r-- : read for everyone and
				//			write for the owner and
				//			owner's group
				//	rw-r--r-- : read for everyone and
				//			write for the owner
				//	rwsr-sr-x : read/execute for everyone,
				//			write for the owner and
				//			run as the user/group
				//			that owns the file
				//	rw-rw-rwt : read/write for everyone,
				//			and save the file in
				//			swap space
		static	char	*evalPermOctal(mode_t mode);
		


		// The following methods can be or'ed together to define
		// a permission.  For example:
		//
		// mode_t perm=(ownerReadWriteExecute() | 
		//	 		groupReadExecute() | 
		// 			othersReadExecute())
		// 
		// sets perm equal to rwxr-xr-x permissions 

		static	mode_t	everyoneReadWrite();
				// returns rw-rw-rw- (666) permissions
		static	mode_t	everyoneReadWriteExecute();
				// returns rwxrwxrwx (777) permissions
		static	mode_t	ownerRead();
				// returns r-------- (400) permissions
		static	mode_t	ownerWrite();
				// returns -w------- (200) permissions
		static	mode_t	ownerExecute();
				// returns --x------ (100) permissions
		static	mode_t	ownerReadWrite();
				// returns rw------- (600) permissions
		static	mode_t	ownerReadExecute();
				// returns r-x------ (500) permissions
		static	mode_t	ownerReadWriteExecute();
				// returns rwx------ (700) permissions
		static	mode_t	groupRead();
				// returns ---r----- (040) permissions
		static	mode_t	groupWrite();
				// returns ----w---- (020) permissions
		static	mode_t	groupExecute();
				// returns -----x--- (010) permissions
		static	mode_t	groupReadWrite();
				// returns ---rw---- (060) permissions
		static	mode_t	groupReadExecute();
				// returns ---r-x--- (050) permissions
		static	mode_t	groupReadWriteExecute();
				// returns ---rwx--- (070) permissions
		static	mode_t	othersRead();
				// returns ------r-- (004) permissions
		static	mode_t	othersWrite();
				// returns -------w- (002) permissions
		static	mode_t	othersExecute();
				// returns --------x (001) permissions
		static	mode_t	othersReadWrite();
				// returns ------rw- (006) permissions
		static	mode_t	othersReadExecute();
				// returns ------r-x (005) permissions
		static	mode_t	othersReadWriteExecute();
				// returns ------rwx (007) permissions
		static	mode_t	saveInSwapSpace();
				// returns --------t permissions
		static	mode_t	setUserId();
				// returns --s------ (u+s) permissions
		static	mode_t	setGroupId();
				// returns -----s--- (g+s) permissions
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/permissionsinlines.h>
#endif

#endif
