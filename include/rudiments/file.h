// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef FILE_H
#define FILE_H

#include <rudiments/private/config.h>

#include <rudiments/filedescriptor.h>

// The file class provides methods for interacting with files.

class file : public filedescriptor {
	public:
			file();
			file(int fd);

		int	open(const char *name, int flags);
			// Opens file "name" using "flags".
			//
			// Returns 1 on success and 0 on failure.

		int	open(const char *name, int flags, mode_t perms);
			// Opens file "name" using "flags".  If the file
			// doesn't already exist and flags contains O_CREAT,
			// the file will be created with permissions "perms".
			//
			// Returns 1 on success and 0 on failure.

		int	create(const char *name, mode_t perms);
			// Creates the file "name" with permissions "perms".
			// If the file already exists, it will be truncated.
			//
			// Returns 1 on success and 0 on failure.

		size_t	create(const char *name, mode_t perms,
						void *data, size_t size);
			// Creates the file "name" with permissions "perms".
			// If the file already exists, it will be truncated.
			// The initial contents of the file are set to "data"
			// of size "size".
			//
			// Returns the number of bytes written to the file or
			// -1 if an error occurs.

		char	*getContents();
			// Allocates a string large enough to accommodate the
			// contents of the currently opened file, reads the
			// contents of the file into the string and returns the
			// string.  The string must be freed by the calling
			// program.
			//
			// If the file contains no data, then a string of length
			// zero is returned.  If an error occurs then a NULL
			// is returned.
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/file.h>
#endif

#endif
