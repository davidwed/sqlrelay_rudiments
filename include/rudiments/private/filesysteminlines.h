// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


inline int	filesystem::move(char *source, char *destination) {
	return (copy(source,destination))?(unlink(source)==0):0;
}

inline int	filesystem::hardLink(char *source, char *destination) {
	return (link(source,destination)==0);
}

inline int	filesystem::symbolicLink(char *source, char *destination) {
	return (symlink(source,destination)==0);
}

inline int	filesystem::makeFifo(char *file, mode_t perms) {
	return mknod(file,perms|S_IFIFO,0);
}

inline int	filesystem::makeCharacterDevice(char *file, mode_t perms,
				unsigned short major, unsigned short minor) {
	return mknod(file,perms|S_IFCHR,packDeviceNumbers(major,minor));
}

inline int	filesystem::makeBlockDevice(char *file, mode_t perms,
				unsigned short major, unsigned short minor) {
	return mknod(file,perms|S_IFBLK,packDeviceNumbers(major,minor));
}

inline dev_t	filesystem::packDeviceNumbers(unsigned short major,
					unsigned short minor) {
	return	(dev_t)(((unsigned long)major)<<16|(unsigned long)minor);
}

inline int	filesystem::changePermissions(char *file, mode_t perms) {
	return (chmod(file,perms)==0);
}
