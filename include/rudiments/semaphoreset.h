// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SEMAPHORESET_H
#define RUDIMENTS_SEMAPHORESET_H

#include <rudiments/private/semaphoresetincludes.h>

// Semaphores allow processes to synchronize their activities.
//
// A semaphore is just a number with two primary operations that can be 
// performed on it: signal() and wait()
// 
// The operations are analagous to:
//
// int	semaphore;
//
// void	signal() {
// 	semaphore++;		// increment the semaphore
// }
//
// void	wait() {
// 	while (!semaphore>0);	// wait until the semaphore>0
// 	semaphore--;		// decrement the semaphore
// }
//
// The actual signal() and wait() operations are atomic.  There is no chance 
// of another process getting context-switched in and changing the semaphore 
// value between the two lines of code in the wait() process.
//
// Semaphores can be initialized to any number.
// 
// The initial value of the semaphore corresponds to the number of processes
// that will pass directly through their wait() calls without being blocked.
//
// Processes that get blocked calling wait() are placed in a queue.  When 
// another process calls signal(), the process at the head of the queue is 
// unblocked.
//
// A semaphoreset is just a collection of related semaphores.
//
// A semaphoreset is owned by a user and group and has access permissions
// just like a file.

class semaphoreset {
	public:
			semaphoreset();
				// Creates a semaphore set.
		virtual	~semaphoreset();
				// Cleans up and removes the semaphore set
				// if it was created by create() or 
				// createOrAttach() below.  If the semaphore
				// was just attached to, it is not removed.

		bool	create(key_t key, mode_t permissions, 
					int semcount, const int *values);
				// Creates a semaphore set identified by "key"
				//      containing "semcount" semaphores.
				// "key" should be generated using the ftok 
				// 	function. 
				// "permissions" sets the access permissions
				//	for the set.
				// "values" should be an array of starting 
				// 	values for each of the semaphores 
				// 	in the set.
		bool	attach(key_t key, int semcount);
				// Attaches to an already existing semaphore set
				// identified by "key", containing "semcount" 
				// semaphores.
		bool	createOrAttach(key_t key, mode_t permissions, 
					int semcount, const int *values);
				// Attempts to create the semaphore set 
				// identified by "key".  If this fails, it 
				// attempts to attach to a semaphore set
				// identified by "key".

		void	dontRemove();
			// Instructs the destructor not to remove the semaphore
			// set if it was created during a call to create() or 
			// createOrAttach() above.  This is useful if an 
			// application creates a semaphore set then forks and 
			// wants to delete the semaphore set in the forked 
			// process but does not want the semaphore removed from
			// the system.
		bool	forceRemove();
			// Removes the semaphore set, whether it
			// was created or attached to.

		int	getId() const;
			// Returns the internal id for the semaphore set.

		bool	wait(int index);
			// wait on the "index"'th semaphore in the set
		bool	wait(int index, long seconds, long nanoseconds);
			// Wait on the "index"'th semaphore in the set until
			// "seconds" and "nanoseconds" elapse.  Returns false
			// and sets errno to EAGAIN if a timeout occurs.
		bool	signal(int index);
			// signal on the "index"'th semaphore in the set


		bool	waitWithUndo(int index);
			// wait on the "index"'th semaphore in the set and
			// undo the wait when the program exits
		bool	waitWithUndo(int index, long seconds, long nanoseconds);
			// Wait on the "index"'th semaphore in the set until
			// "seconds" and "nanoseconds" elapse.  Undo the wait
			// when the program exits.  Returns false and sets
			// errno to EAGAIN if a timeout occurs.
		bool	signalWithUndo(int index);
			// signal on the "index"'th semaphore in the set and
			// undo the signal when the program exits


		bool	setValue(int index, int value);
			// set the "index"'th semaphore in the set to "value"
		int	getValue(int index);
			// return the value of the "index"'th 
			// semaphore in the set


		bool	setUserName(const char *username);
			// Makes this semaphore set owned by 
			// the user "username".
			// 	
			// Note that setUserName() uses the passwdentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the passwdentry
			// class a mutex.  See passwdentry.h for more detail.
		bool	setGroupName(const char *groupname);
			// Makes this semaphore set owned by 
			// the group "groupname".
			// 	
			// Note that setGroupName() uses the groupentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the groupentry
			// class a mutex.  See groupentry.h for more detail.

		bool	setUserId(uid_t uid);
			// makes this semaphore set owned by 
			// the user identified by "uid"
		bool	setGroupId(gid_t gid);
			// makes this semaphore set owned by 
			// the group identified by "gid"
		bool	setPermissions(mode_t permissions);
			// sets the access permissions for this 
			// semaphore set to "permissions"


		char	*getUserName();
			// returns the name of the user that owns this
			// semaphore set
			//
			// Note that this method allocates a buffer
			// internally and returns it.  The calling program must
			// deallocate this buffer.
			// 	
			// Note that getUserName() uses the passwdentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the passwdentry
			// class a mutex.  See passwdentry.h for more detail.
		char	*getGroupName();
			// returns the name of the group that owns this
			// semaphore set
			//
			// Note that this method allocates a buffer
			// internally and returns it.  The calling program must
			// deallocate this buffer.
			// 	
			// Note that getGroupName() uses the groupentry class.
			// If you are using this method in a multithreaded
			// application, you may need to supply the groupentry
			// class a mutex.  See groupentry.h for more detail.

		uid_t	getUserId();
			// returns the user id of the user that owns this
			// semaphore set
		gid_t	getGroupId();
			// returns the group id of the group that owns this
			// semaphore set
		mode_t	getPermissions();
			// returns the access permissions for this
			// semaphore set


		int	getWaitingForZero(int index);
			// returns the number of processes that
			// are waiting for the semaphore to become 0
		int	getWaitingForIncrement(int index);
			// returns the number of processes that
			// are waiting for the semaphore to increment

	#include <rudiments/private/semaphoreset.h>

};

#endif
