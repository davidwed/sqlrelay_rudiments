// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SEMAPHORESET_H
#define RUDIMENTS_SEMAPHORESET_H

#include <rudiments/private/semaphoresetincludes.h>

/** Semaphores allow processes to synchronize their activities.
 *
 *  A semaphore is just a number with two primary operations that can be 
 *  performed on it: signal() and wait()
 *  
 *  The operations are analagous to:
 * 
 *  int32_t	semaphore;
 * 
 *  void	signal() {
 *  	semaphore++;		// increment the semaphore
 *  }
 * 
 *  void	wait() {
 *  	while (!(semaphore>0));	// wait until the semaphore>0
 *  	semaphore--;		// decrement the semaphore
 *  }
 * 
 *  The actual signal() and wait() operations are atomic.  There is no chance 
 *  of another process getting context-switched in and changing the semaphore 
 *  value between the two lines of code in the wait() process.
 * 
 *  Semaphores can be initialized to any number.
 *  
 *  The initial value of the semaphore corresponds to the number of processes
 *  that will pass directly through their wait() calls without being blocked.
 * 
 *  Processes that get blocked calling wait() are placed in a queue.  When 
 *  another process calls signal(), the process at the head of the queue is 
 *  unblocked.
 * 
 *  A semaphoreset is just a collection of related semaphores.
 * 
 *  A semaphoreset is owned by a user and group and has access permissions
 *  just like a file. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class semaphoresetprivate;

class RUDIMENTS_DLLSPEC semaphoreset {
	public:

		/** Creates an instance of the semaphoreset class. */
		semaphoreset();

		/** Deletes this instance of the semaphoreset class.
		 *  Removes the semaphoreset if it was created by this class.
		 *  If it was only attached to, it is not removed. */
		~semaphoreset();

		/** Returns true if the system supports timed
		 *  semaphore operations or false otherwise. */
		bool	supportsTimedSemaphoreOperations();

		/** Creates a semaphore set identified by "key"
		 *       containing "semcount" semaphores.
		 *  "key" should be generated using the ftok 
		 *  	function. 
		 *  "permissions" sets the access permissions
		 * 	for the set.
		 *  "values" should be an array of starting 
		 *  	values for each of the semaphores 
		 *  	in the set. */
		bool	create(key_t key, mode_t permissions, 
				int32_t semcount, const int32_t *values);

		/** Attaches to an already existing semaphore set
		 *  identified by "key", containing "semcount" 
		 *  semaphores. */
		bool	attach(key_t key, int32_t semcount);

		/** Attempts to create the semaphore set 
		 *  identified by "key".  If this fails, it 
		 *  attempts to attach to a semaphore set
		 *  identified by "key". */
		bool	createOrAttach(key_t key, mode_t permissions, 
				int32_t semcount, const int32_t *values);

		/** Instructs the destructor not to remove the semaphore
		 *  set if it was created during a call to create() or 
		 *  createOrAttach().  This is useful if an 
		 *  application creates a semaphore set then forks and 
		 *  wants to delete the semaphore set in the forked 
		 *  process but does not want the semaphore removed from
		 *  the system. */
		void	dontRemove();

		/** Instructs the destructor to remove the semaphore set,
		 *  whether it was created or attached to. */
		bool	forceRemove();

		/** Returns the internal id for the semaphore set. */
		int32_t	getId() const;

		/** Wait on the "index"'th semaphore in the set. */
		bool	wait(int32_t index);

		/** Wait on the "index"'th semaphore in the set until
		 *  "seconds" and "nanoseconds" elapse.  Returns false
		 *  and sets errno to EAGAIN if a timeout occurs.
		 *  Returns false if the system doesn't support timed
		 *  semaphore operations. */
		bool	wait(int32_t index, long seconds, long nanoseconds);

		/** Signal on the "index"'th semaphore in the set. */
		bool	signal(int32_t index);


		/** Wait on the "index"'th semaphore in the set and
		 *  undo the wait when the program exits. */
		bool	waitWithUndo(int32_t index);

		/** Wait on the "index"'th semaphore in the set until
		 *  "seconds" and "nanoseconds" elapse.  Undo the wait
		 *  when the program exits.  Returns false and sets
		 *  errno to EAGAIN if a timeout occurs.
		 *  Returns false if the system doesn't support timed
		 *  semaphore operations. */
		bool	waitWithUndo(int32_t index,
					long seconds, long nanoseconds);

		/** Signal on the "index"'th semaphore in the set and
		 *  undo the signal when the program exits. */
		bool	signalWithUndo(int32_t index);


		/** Set the "index"'th semaphore in the set to "value". */
		bool	setValue(int32_t index, int32_t value);

		/** Return the value of the "index"'th semaphore in the set. */
		int32_t	getValue(int32_t index);


		/** Makes this semaphore set owned by the user "username".
		 * 	
		 *  Note that setUserName() uses the passwdentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the passwdentry
		 *  class a mutex.  See passwdentry.h for more detail. */
		bool	setUserName(const char *username);

		/** Makes this semaphore set owned by 
		 *  the group "groupname".
		 *  	
		 *  Note that setGroupName() uses the groupentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the groupentry
		 *  class a mutex.  See groupentry.h for more detail. */
		bool	setGroupName(const char *groupname);

		/** Makes this semaphore set owned by 
		 *  the user identified by "uid". */
		bool	setUserId(uid_t uid);

		/** Makes this semaphore set owned by 
		 *  the group identified by "gid". */
		bool	setGroupId(gid_t gid);

		/** Sets the access permissions for this 
		 *  semaphore set to "permissions". */
		bool	setPermissions(mode_t permissions);


		/** Returns the name of the user that owns this
		 *  semaphore set.
		 * 
		 *  Note that this method allocates a buffer
		 *  internally and returns it.  The calling program must
		 *  deallocate this buffer.
		 *  	
		 *  Note that getUserName() uses the passwdentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the passwdentry
		 *  class a mutex.  See passwdentry.h for more detail. */
		const char	*getUserName();

		/** Returns the name of the group that owns this
		 *  semaphore set
		 * 
		 *  Note that this method allocates a buffer
		 *  internally and returns it.  The calling program must
		 *  deallocate this buffer.
		 *  	
		 *  Note that getGroupName() uses the groupentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the groupentry
		 *  class a mutex.  See groupentry.h for more detail. */
		const char	*getGroupName();

		/** Returns the user id of the user that owns this
		 *  semaphore set. */
		uid_t	getUserId();

		/** Returns the group id of the group that owns this
		 *  semaphore set. */
		gid_t	getGroupId();

		/** Returns the access permissions for this semaphore set. */
		mode_t	getPermissions();


		/** Returns the number of processes that
		 *  are waiting for the semaphore to become 0. */
		int32_t	getWaitingForZero(int32_t index);

		/** Returns the number of processes that
		 *  are waiting for the semaphore to increment. */
		int32_t	getWaitingForIncrement(int32_t index);

		/** Causes operations to automatically be retired if
		 *  interrupted by a signal.  This is the default
		 *  behiavior. */
		void	retryInterruptedOperations();

		/** Causes operations not to automatically be retired
		 *  if interrupted by a signal.  The default is to retry
		 *  automatically. */
		void	dontRetryInterruptedOperations();

	#include <rudiments/private/semaphoreset.h>

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
