// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHMFILE_H
#define RUDIMENTS_SHMFILE_H

#include <rudiments/private/shmfileincludes.h>

// The shmfile class provides methods for interacting with POSIX shared memory
// objects.  POSIX shared memory objects may be accessed like files, but
// reside in shared memory rather than on the filesystem.
//
// For maximum portability, the name of a shmfile should begin with a slash (/)
// and contain no embedded slashes.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class shmfileprivate;

class shmfile : public file {
	public:

			shmfile();
			shmfile(const shmfile &s);
		shmfile	&operator=(const shmfile &s);
		virtual	~shmfile();

		static bool	remove(const char *filename);
				// Removes POXIX shared memory object
				// "filename".  Returns true on success and
				// false on failure.

	#include <rudiments/private/shmfile.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
