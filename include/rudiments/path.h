// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PATH_H
#define RUDIMENTS_PATH_H

#include <rudiments/private/pathincludes.h>

class path {
	public:
		static char	*dirname(const char *pathname);
				// Returns the directory portion of "pathname".
				// This method allocates a buffer internally
				// and returns it.  The calling program must
				// deallocate the buffer.
		static char	*basename(const char *pathname);
				// Returns the non-directory portion of
				// "pathname".
				// This method allocates a buffer internally
				// and returns it.  The calling program must
				// deallocate the buffer.
		static char	*basename(const char *pathname,
						const char *suffix);
				// Returns the non-directory portion of
				// "pathname", truncating "suffix".
				// This method allocates a buffer internally
				// and returns it.  The calling program must
				// deallocate the buffer.

		static key_t	generateKey(const char *pathname, int id);
				// Generates a key based on "pathname" and the
				// least signifigant 8 bits of id (which must
				// be non-zero) suitable for use with
				// semaphores, shared memory segments and
				// message queues.
};

#endif
