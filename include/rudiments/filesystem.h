// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <rudiments/private/config.h>

#include <sys/types.h>

// The filesystem class provides methods for performing commonly needed
// filesystem-related tasks.

class filesystem {
	public:

		static	int	move(char *source, char *dest);
				// Moves file or directory "source" to "dest".

		static	int	copy(char *source, char *dest);
				// Copies file or directory "source" to "dest".

		static	int	remove(char *file);
				// Removes file or directory "file".

		static	int	hardLink(char *source, char *dest);
				// Creates a hard link between "source" and
				// "dest".

		static	int	symbolicLink(char *source, char *dest);
				// Creates a symbolic link between "source" and
				// "dest".

		static	int	makeFifo(char *file, mode_t perms);
				// Creates a fifo named "file" with permissions
				// "perms".

		static	int	makeCharacterDevice(char *file, mode_t perms,
							unsigned short major,
							unsigned short minor);
				// Creates a character device "file"
				// with permissions "perms", major device
				// number "major" and minor device number
				// "minor".

		static	int	makeBlockDevice(char *file, mode_t perms,
							unsigned short major,
							unsigned short minor);
				// Creates a block device "file" with
				// permissions "perms", major device number
				// "major" and minor device number "minor".

		static	int	changeOwner(char *file, char *owner,
								char *group);
				// Changes the owner of file or directory
				// "file" to "owner" and the group to "group".

		static	int	changePermissions(char *file, mode_t perms);
				// Changes the permissions of file or directory
				// "file" to "perms".

		static	char	**list(char *pattern);
				// Returns a NULL terminated list of files
				// matching regular expression "pattern".

	#include <rudiments/private/filesystem.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/filesysteminlines.h>
#endif

#endif
