// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/filesystem.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/filesysteminlines.h>
#endif

#if defined(HAVE_DIRENT_H)
	#include <dirent.h>
#elif defined(HAVE_DIR_H)
	#include <dir.h>
#elif defined(HAVE_NDIR_H)
	#include <ndir.h>
#elif defined(HAVE_SYS_NDIR_H)
	#include <sys/ndir.h>
#endif

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/file.h>
#include <errno.h>

#ifdef HAVE_STRINGS
	#include <strings.h>
#endif
#include <string.h>

int filesystem::copy(char *source, char *destination) {

	// open the files
	file	src;
	if (!src.open(source,O_RDONLY)) {
		return 0;
	}
	file	dst;
	if (!dst.open(destination,O_WRONLY|O_CREAT|O_TRUNC)) {
		src.close();
		return 0;
	}

	// copy from src to dest in 512 byte chunks
	int	retval=0;
	char	block[512];
	for (;;) {

		// read from src
		ssize_t	sizeread;
		for (;;) {

			sizeread=src.read((void *)block,sizeof(block));

			// retry reads that were interrupted by signals
			if (sizeread==-1 && errno==EINTR) {
				continue;
			}
			break;
		}

		// read error
		if (sizeread==-1) {
			break;
		}

		// EOF
		if (sizeread==0) {
			retval=1;
			break;
		}

		// write to dest
		ssize_t	sizewritten;
		for (;;) {

			sizewritten=dst.write((void *)block,sizeread);

			// retry writes that were interrupted by signals
			if (sizewritten==-1 && errno==EINTR) {
				continue;
			}
			break;
		}

		// write error
		if (sizewritten!=sizeread) {
			break;
		}
	}

	// close files
	src.close();
	dst.close();

	return retval;
}

int filesystem::remove(char *file) {
	char	**files=list(file);
	int	index=0;
	while (files[index]) {
		unlink(file);
		index++;
	}
	delete[] files;
}

int filesystem::changeOwner(char *file, char *username, char *groupname) {

	passwdentry	passwdent;
	groupentry	groupent;
	return ((username)?(!passwdent.initialize(username)):1) &&
			((groupname)?(!groupent.initialize(groupname)):1) &&
			(!chown(file,(username)?passwdent.getUserId():-1,
				(groupname)?groupent.getGroupId():-1));
}

char **filesystem::list(char *pattern) {

	// open the directory
	DIR	*dir=opendir(pattern);

	// count the directory entries
	int	count=0;
	while (readdir(dir)) {
		count++;
	}
	rewinddir(dir);

	// Scan through the directory.  For each file,
	// make a copy of it's name in the dirlist.
	char	**dirlist=new char *[count+1];
	dirlist[count]=NULL;
	for (int i=0; i<count; i++) {
		dirlist[i]=strdup(readdir(dir)->d_name);
	}

	// close the directory
	closedir(dir);

	return dirlist;
}
