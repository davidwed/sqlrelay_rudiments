// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DIRECTORY_H
#define RUDIMENTS_DIRECTORY_H

#include <rudiments/private/directoryincludes.h>

// The directory class provides methods for opening/closing a directory and
// traversing it's files and subdirectories.

class directory {
	public:
			directory();
			~directory();

		bool	open(const char *path);
			// opens directory "path"
		bool	close();
			// closes the currently open directory
		char	*getChildName(unsigned long index);
			// Returns the name of the file/subdirectory at "index".
			//
			// This methods allocates a buffer internally and
			// returns it.  The calling program must deallocate the
			// buffer.

		static bool	create(const char *path, mode_t perms);
				// creates a new directory "path" with 
				// permissions "perms"
		static bool	remove(const char *path);
				// removes the directory "path" if it is empty
		static char	*getCurrentWorkingDirectory();
				// Returns the current working directory.
				//
				// This methods allocates a buffer internally
				// and returns it.  The calling program must
				// deallocate the buffer.
		static bool	changeDirectory(const char *path);
				// sets the current working directory to "path"
		static bool	changeRoot(const char *path);
				// changes root to "path"



#ifdef RUDIMENTS_HAS_THREADS
		static bool	needsMutex();
			// If your system doesn't support readdir_r()
			// then this class needs a mutex to assure thread
			// safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.
#endif

	#include <rudiments/private/directory.h>
};

#endif
