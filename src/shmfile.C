// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/shmfile.h>
#include <sys/types.h>
#include <sys/mman.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

shmfile::shmfile() : file() {}

shmfile::~shmfile() {}

int shmfile::openInternal(const char *name, int flags) {
	return shm_open(name,flags,0);
}

int shmfile::openInternal(const char *name, int flags, mode_t perms) {
	return shm_open(name,flags,perms);
}

bool shmfile::remove(const char *filename) {
	return !shm_unlink(filename);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
