// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/directory.h>
#include <rudiments/charstring.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && !defined(HAVE_READDIR_R)
pthread_mutex_t	*directory::rdmutex;
#endif

directory::directory() {
	dir=NULL;
	currentindex=0;
}

directory::~directory() {
	close();
}

bool directory::open(const char *path) {
	return ((dir=opendir(path))!=NULL);
}

bool directory::close() {
	bool	retval=true;
	if (dir) {
		retval=!closedir(dir);
		dir=NULL;
		currentindex=0;
	}
	return retval;
}

char *directory::getChildName(unsigned long index) {

	// directory entries are 1-based
	unsigned long	actualindex=index+1;

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
		for (unsigned long i=currentindex; i<actualindex; i++) {
			if (readdir_r(dir,&entry,&result) || !result) {
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
		if (rdmutex && pthread_mutex_lock(rdmutex)) {
			return NULL;
		}
		#endif
		for (unsigned long i=currentindex; i<actualindex; i++) {
			if (!(entry=readdir(dir))) {
				return NULL;
			}
			currentindex++;
		}
		char	*retval=charstring::duplicate(entry->d_name);
		#ifdef RUDIMENTS_HAS_THREADS
		if (rdmutex) {
			pthread_mutex_unlock(rdmutex);
		}
		#endif
		return retval;
	#endif
}

bool directory::create(const char *path, mode_t perms) {
	return !mkdir(path,perms);
}

bool directory::remove(const char *path) {
	return !rmdir(path);
}

char *directory::getCurrentWorkingDirectory() {
	size_t	size=1024;
	for (;;) {
		char	*buffer=new char[size];
		if (getcwd(buffer,size)) {
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
	return !chdir(path);
}

bool directory::changeRoot(const char *path) {
	return !chroot(path);
}

#ifdef RUDIMENTS_HAS_THREADS
bool directory::needsMutex() {
	#if !defined(HAVE_READDIR_R)
		return true;
	#else
		return false;
	#endif
}

void directory::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_READDIR_R)
		rdmutex=mutex;
	#endif
}
#endif

long directory::maxFileNameLength(const char *pathname) {
	return pathconf(pathname,_PC_NAME_MAX);
}

long directory::maxPathLength(const char *pathname) {
	return pathconf(pathname,_PC_PATH_MAX);
}

bool directory::canAccessLongFileNames(const char *pathname) {
	return !pathconf(pathname,_PC_NO_TRUNC);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
