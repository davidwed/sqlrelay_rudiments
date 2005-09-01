// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/directory.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#include <stdlib.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <sys/stat.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && !defined(HAVE_READDIR_R)
mutex	*directory::rdmutex;
#endif

directory::directory() {
	dir=NULL;
	currentindex=0;
}

directory::~directory() {
	close();
}

bool directory::open(const char *path) {
	do {
		dir=opendir(path);
	} while (dir==NULL && error::getErrorNumber()==EINTR);
	return (dir!=NULL);
}

bool directory::close() {
	bool	retval=true;
	if (dir) {
		do {
			retval=!closedir(dir);
		} while (!retval && error::getErrorNumber()==EINTR);
		dir=NULL;
		currentindex=0;
	}
	return retval;
}

char *directory::getChildName(uint64_t index) {

	// directory entries are 1-based
	uint64_t	actualindex=index+1;

	if (actualindex<currentindex) {
		rewinddir(dir);
		currentindex=0;
	}

	#ifdef HAVE_READDIR_R
		#ifdef HAVE_DIRENT_H
			dirent	entry;
			dirent	*result;
		#else
			direct	entry;
			dirent	*result;
		#endif
		for (uint64_t i=currentindex; i<actualindex; i++) {
			int	rdresult;
			do {
				rdresult=readdir_r(dir,&entry,&result);
			} while (rdresult==-1 &&
					error::getErrorNumber()==EINTR);
			if (rdresult || !result) {
				return NULL;
			}
			currentindex++;
		}
		return charstring::duplicate(entry.d_name);
	#else
		#ifdef HAVE_DIRENT_H
			dirent	*entry;
		#else
			direct	*entry;
		#endif
		#ifdef RUDIMENTS_HAS_THREADS
		if (rdmutex && !rdmutex->lock()) {
			return NULL;
		}
		#endif
		for (uint64_t i=currentindex; i<actualindex; i++) {
			do {
				entry=readdir(dir);
			} while (!entry && error::getErrorNumber()==EINTR);
			if (!entry) {
				return NULL;
			}
			currentindex++;
		}
		char	*retval=charstring::duplicate(entry->d_name);
		#ifdef RUDIMENTS_HAS_THREADS
		if (rdmutex) {
			rdmutex->unlock();
		}
		#endif
		return retval;
	#endif
}

bool directory::create(const char *path, mode_t perms) {
	int	result;
	do {
		result=mkdir(path,perms);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool directory::remove(const char *path) {
	int	result;
	do {
		result=rmdir(path);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

char *directory::getCurrentWorkingDirectory() {
	size_t	size=1024;
	for (;;) {
		char	*buffer=new char[size];
		char	*result;
		do {
			result=getcwd(buffer,size);
		} while (!result && error::getErrorNumber()==EINTR);
		if (result) {
			return buffer;
		} else {
			delete[] buffer;
			size=size+1024;
			if (size>10240) {
				return NULL;
			}
		}
	}
}

bool directory::changeDirectory(const char *path) {
	int	result;
	do {
		result=chdir(path);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool directory::changeRoot(const char *path) {
	int	result;
	do {
		result=chroot(path);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

#ifdef RUDIMENTS_HAS_THREADS
bool directory::needsMutex() {
	#if !defined(HAVE_READDIR_R)
		return true;
	#else
		return false;
	#endif
}

void directory::setMutex(mutex *mtx) {
	#if !defined(HAVE_READDIR_R)
		rdmutex=mtx;
	#endif
}
#endif

int64_t directory::maxFileNameLength(const char *pathname) {
	return pathConf(pathname,_PC_NAME_MAX);
}

int64_t directory::maxPathLength(const char *pathname) {
	return pathConf(pathname,_PC_PATH_MAX);
}

bool directory::canAccessLongFileNames(const char *pathname) {
	return !pathConf(pathname,_PC_NO_TRUNC);
}

int64_t directory::pathConf(const char *pathname, int name) {
	int64_t	result;
	do {
		result=pathconf(pathname,name);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
