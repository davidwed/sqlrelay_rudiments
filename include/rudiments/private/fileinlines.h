// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <rudiments/fileproperties.h>

inline file::file() : filedescriptor() {}
inline file::file(int fd) : filedescriptor(fd) {}

inline int file::open(const char *name, int flags) {
	return ((fd=::open(name,flags))!=-1);
}

inline int file::open(const char *name, int flags, mode_t perms) {
	return ((fd=::open(name,flags,perms))!=-1);
}

inline int file::create(const char *name, mode_t perms) {
	return ((fd=::open(name,O_CREAT|O_TRUNC|O_RDWR,perms))!=-1);
}
