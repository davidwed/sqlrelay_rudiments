// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHMFILE_H
#define RUDIMENTS_SHMFILE_H

#include <rudiments/private/shmfileincludes.h>

class shmfileprivate;

/** The shmfile class provides methods for interacting with POSIX shared memory
 *  objects.  POSIX shared memory objects may be accessed like files, but
 *  reside in shared memory rather than on the filesystem.
 * 
 *  For maximum portability, the name of a shmfile should begin with a slash (/)
 *  and contain no embedded slashes. */
class RUDIMENTS_DLLSPEC shmfile : public file {
	public:

		/** Creates an instance of the shmfile class. */
		shmfile();

		/** Creates an instance of the shmfile class
		 *  that is a copy of "s". */
		shmfile(const shmfile &s);

		/** Makes this instance of the shmfile class
		 *  identical to "s". */
		shmfile	&operator=(const shmfile &s);

		/** Deletes this instance of the shmfile class. */
		virtual	~shmfile();

		/** Removes POXIX shared memory object
		 *  "filename".  Returns true on success and
		 *  false on failure. */
		static bool	remove(const char *filename);

	#include <rudiments/private/shmfile.h>
};

#endif
