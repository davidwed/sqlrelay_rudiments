// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DIRECTORY_H
#define RUDIMENTS_DIRECTORY_H

#include <rudiments/private/directoryincludes.h>

// The directory class provides methods for opening/closing a directory and
// traversing it's files and subdirectories.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class directoryprivate;

class RUDIMENTS_DLLSPEC directory {
	public:
		/** Creates an instance of the directory class. */
		directory();

		/** Deletes this instance of the directory class. */
		~directory();

		/** Opens directory "path". */
		bool	open(const char *path);

		/** Closes the currently open directory. */
		bool	close();

		/** Resets the current file/subdirectory inside the
		 *  open directory to the first one. */
		void	rewind();

		/** Returns the number of files/subdirectories inside
		 *  the open directory. */
		uint64_t	getChildCount();

		/** Returns the name of the file/subdirectory at "index".
		 * 
		 *  This methods allocates a buffer internally and
		 *  returns it.  The calling program must deallocate the
		 *  buffer. */
		char	*getChildName(uint64_t index);

		/** Returns the length of the longest file name
		 *  that can be created in directory "pathname". */
		int64_t	maxFileNameLength();

		/** Returns the length of the longest relative
		 *  path name that can be created under the
		 *  directory "pathname". */
		int64_t	maxPathLength();

		/** Returns true if an attempt to access a
		 *  file in directory "pathname" longer than the
		 *  length returned by maxFileNameLength() will
		 *  succeed or false if it will fail. */
		bool	canAccessLongFileNames();

		/** Creates a new directory "path" with permissions "perms". */
		static bool	create(const char *path, mode_t perms);

		/** Removes the directory "path", if it is empty. */
		static bool	remove(const char *path);

		/** Returns the current working directory.
		 *
		 *  This methods allocates a buffer internally
		 *  and returns it.  The calling program must
		 *  deallocate the buffer. */
		static char	*getCurrentWorkingDirectory();

		/** Sets the current working directory to "path". */
		static bool	changeDirectory(const char *path);

		/** Changes root to "path". */
		static bool	changeRoot(const char *path);

		/** Returns the length of the longest file name
		 *  that can be created in directory "pathname". */
		static int64_t	maxFileNameLength(const char *pathname);

		/** Returns the length of the longest relative
		 *  path name that can be created under the
		 *  directory "pathname". */
		static int64_t	maxPathLength(const char *pathname);

		/** Returns true if an attempt to access a
		 *  file in directory "pathname" longer than the
		 *  length returned by maxFileNameLength() will
		 *  succeed or false if it will fail. */
		static bool	canAccessLongFileNames(const char *pathname);

		/** This method returns true if this class needs a mutex
		 *  to operate safely in a threaded environment and false
		 *  otherwise. */
		static bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  If your application is not multithreaded, then
		 *  there is no need to supply a mutex. */
		static void	setMutex(mutex *mtx);

	#include <rudiments/private/directory.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
