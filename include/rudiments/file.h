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

		int	open(const char *name, int flags, mode_t perms);
			// Opens file "name" using "flags".  If the file
			// doesn't already exist and flags contains O_CREAT,
			// the file will be created with permissions "perms".
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/file.h>
#endif

#endif
