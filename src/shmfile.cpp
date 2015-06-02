// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/shmfile.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_MMAN_H
	#include <sys/mman.h>
#endif

class shmfileprivate {
	friend class shmfile;
	private:
};

shmfile::shmfile() : file() {
	pvt=new shmfileprivate;
	type("shmfile");
}

shmfile::shmfile(const shmfile &s) : file(s) {
	pvt=new shmfileprivate;
	type("shmfile");
}

shmfile &shmfile::operator=(const shmfile &s) {
	if (this!=&s) {
		file::operator=(s);
	}
	return *this;
}

shmfile::~shmfile() {
	delete pvt;
}

void shmfile::openInternal(const char *name, int32_t flags,
				mode_t perms, bool useperms) {
	#if defined(RUDIMENTS_HAVE_SHM_OPEN)
		int32_t	result;
		error::clearError();
		do {
			result=shm_open(name,flags,perms);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		fd(result);
	#else
		RUDIMENTS_SET_ENOSYS
		fd(-1);
	#endif
}

bool shmfile::remove(const char *filename) {
	#if defined(RUDIMENTS_HAVE_SHM_OPEN)
		int32_t	result;
		error::clearError();
		do {
			result=shm_unlink(filename);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}
