// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHAREDMEMORY_H
#define RUDIMENTS_SHAREDMEMORY_H

#include <rudiments/private/sharedmemoryincludes.h>

// Shared memory segments allow multiple processes to access a common
// address space.
//
// Shared memory can be used for interprocess communication and is generally
// faster than using fifo's or unix sockets.
//
// To synchronize access to shared memory, use the semaphoreset class.
//
// A shared memory segment is owned by a user and group and has access 
// permissions just like a file.

class sharedmemory {
	public:
			sharedmemory();
			// Creates a shared memory segment.
		virtual	~sharedmemory();
			// Cleans up and removes the shared memory segment if
			// it was created by the create() or createOrAttach() 
			// method below.  If it was just attached to, it is
			// not removed.

		int	create(key_t key, int size, mode_t permissions);
			// Creates a shared memory segment of "size" bytes, 
			// identified by "key".
			// "key" should be generated using the ftok
			//	function.
			// "permissions" sets the access permissions
			//	for the segment.
		int	attach(key_t key);
			// Attaches to a shared memory identified by "key".
		int	createOrAttach(key_t key, int size, mode_t permissions);
			// Attempts to create the shared memory segment 
			// identified by "key".  If this fails, it attempts
			// to attach to a shared memory segment identified by
			// "key".

		void	dontRemove();
			// Instructs the destructor not to remove the shared
			// memory segment if it was created during a call to
			// create() or createOrAttach() above.  This is useful
			// if an application creates a shared memory segment
			// then forks and wants to delete the shared memory
			// segment in the forked process but does not want
			// the segment removed from the system.
		int	forceRemove();
			// Removes the shared memory segment whether it was
			// created or just attached to.

		int	getId() const;
			// Returns the internal id for the 
			// shared memory segment.

		void	*getPointer() const;
			// Returns a pointer to the shared memory
			// segment.  Data may be read from or written
			// to the segment using this pointer.

		int	setUserName(const char *username);
			// Makes this shared memory segment owned by
			// the user "username".
			//
			// Note that setUserName() uses the passwdentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the passwdentry
			// class a mutex.  See passwdentry.h for more detail.
		int	setGroupName(const char *groupname);
			// Makes this shared memory segment owned by
			// the group "groupname".
			//
			// Note that setGroupName() uses the groupentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the groupentry
			// class a mutex.  See groupentry.h for more detail.
		int	setUserId(uid_t uid);
			// makes this shared memory segment owned by
			// the user identified by uid
		int	setGroupId(gid_t gid);
			// makes this shared memory segment owned by
			// the group identified by gid
		int	setPermissions(mode_t permissions);
			// sets the access permissions for this
			// shared memory segment to "permissions"

		char	*getUserName();
			// returns the name of the user that owns this
			// shared memory segment
			//
			// Note that getUserName() uses the passwdentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the passwdentry
			// class a mutex.  See passwdentry.h for more detail.
		char	*getGroupName();
			// returns the name of the group that owns this
			// shared memory segment
			//
			// Note that getGroupName() uses the groupentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the groupentry
			// class a mutex.  See groupentry.h for more detail.
		uid_t	getUserId();
			// returns the user id of the user that owns this
			// shared memory segment
		gid_t	getGroupId();
			// returns the group id of the group that owns this
			// shared memory segment
		mode_t	getPermissions();
			// returns the access permissions for this
			// shared memory segment

	#include <rudiments/private/sharedmemory.h>

};

#ifdef ENABLE_INLINES
	#include <rudiments/private/sharedmemoryinlines.h>
#endif

#endif
