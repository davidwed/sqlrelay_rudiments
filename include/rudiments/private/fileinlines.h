// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

inline file::file() : filedescriptor() {}
inline file::file(int fd) : filedescriptor(fd) {}

inline int file::open(const char *name, int flags, mode_t perms) {
	return ::open(name,flags,perms);
}
