// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/shmfile.h>
#include <rudiments/error.h>
#include <sys/types.h>
#include <sys/mman.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
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

int32_t shmfile::openInternal(const char *name, int32_t flags) {
	int	result;
	do {
		result=shm_open(name,flags,0);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

int32_t shmfile::openInternal(const char *name, int32_t flags, mode_t perms) {
	int	result;
	do {
		result=shm_open(name,flags,perms);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool shmfile::remove(const char *filename) {
	int	result;
	do {
		result=shm_unlink(filename);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
