// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef LISTENER_H
#define LISTENER_H

#include <rudiments/private/config.h>

#include <rudiments/private/filedescriptornode.h>

// The listener class listens on a pool of file descriptors.

class listener {
	public:
			listener();
			~listener();

		virtual	void	addFileDescriptor(int fd);
                        // Adds the specified file descriptor to
			// the pool that the listener is listening on.

		virtual	int	waitForData(long timeoutsec, long timeoutusec);
			// Waits for data to become available on any of the
			// file descriptors in the pool for "timeoutsec"
			// seconds and "timeoutusec" milliseconds before
			// falling through.  
			//
			// Entering -1 for either parameter causes the method
			// to wait indefinitely.  
			//
			// Entering 0 for both parameters causes the method to
			// fall through immediately unless a data is
			// immediately available.
			//
			// Returns the file descriptor that data was available
			// on or -1 on error.

		virtual	void	removeFileDescriptor(int fd);
                        // Removes the specified file descriptor from the pool.
			
		virtual	void	removeAllFileDescriptors();
			// Removes all file descriptors from the pool.

	#include <rudiments/private/listener.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/listenerinlines.h>
#endif

#endif
