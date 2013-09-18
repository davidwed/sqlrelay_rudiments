// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/directory.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#ifdef RUDIMENTS_HAVE_MKDIR_1
	#include <rudiments/permissions.h>
#endif

// for DIR
#if defined(RUDIMENTS_HAVE_DIRENT_H)
	#include <dirent.h>
#elif defined(RUDIMENTS_HAVE_DIRECT_H)
	#include <direct.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif

#ifdef _WIN32
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
	// windows doesn't define these, but we need them
	// internally to this file
	#ifndef _PC_NAME_MAX
		#define _PC_NAME_MAX	0
	#endif
	#ifndef _PC_NO_TRUNC
		#define _PC_NO_TRUNC	1
	#endif
	#ifndef _PC_PATH_MAX
		#define _PC_PATH_MAX	2
	#endif
#endif

#include <stdio.h>

class directoryprivate {
	friend class directory;
	private:
		DIR		*_dir;
		uint64_t	_currentindex;
};

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_READDIR_R)
static threadmutex	*_rdmutex;
#endif


// lame that this isn't part of the class, but I can't think of another way to
// keep #ifndef RUDIMENTS_HAVE_DIRENT_H out of the header file
#ifdef RUDIMENTS_HAVE_READDIR_R
static int64_t bufferSize(directory *d) {
	int64_t	name_max=d->maxFileNameLength();
	if (name_max==-1) {
		return -1;
	}
	#ifdef RUDIMENTS_HAVE_DIRENT_H
        	//return offsetof(struct dirent, d_name)+name_max+1;
        	return sizeof(struct dirent)+name_max+1;
	#else
        	return sizeof(struct direct)+name_max+1;
	#endif
}
#endif

directory::directory() {
	pvt=new directoryprivate;
	pvt->_dir=NULL;
	pvt->_currentindex=0;
}

directory::~directory() {
	close();
	delete pvt;
}

bool directory::open(const char *path) {
	close();
	do {
		pvt->_dir=opendir(path);
	} while (pvt->_dir==NULL && error::getErrorNumber()==EINTR);
	return (pvt->_dir!=NULL);
}

bool directory::skip() {
	char	*file=read();
	if (file) {
		delete[] file;
		return true;
	}
	return false;
}

char *directory::read() {

	if (!pvt->_dir) {
		return NULL;
	}

	#ifdef RUDIMENTS_HAVE_READDIR_R
		// get the size of the buffer
		int64_t	size=bufferSize(this);
		if (size==-1) {
			return NULL;
		}
		#ifdef RUDIMENTS_HAVE_DIRENT_H
			dirent	*entry=reinterpret_cast<dirent *>(
						new unsigned char[size]);
		#else
			direct	*entry=reinterpret_cast<direct *>(
						new unsigned char[size]);
		#endif
		dirent	*result;
		int32_t	rdresult;
		do {
			rdresult=readdir_r(pvt->_dir,entry,&result);
		} while (rdresult==-1 &&
				error::getErrorNumber()==EINTR);
		if (rdresult || !result) {
			delete[] entry;
			return NULL;
		}
		pvt->_currentindex++;
		char	*retval=charstring::duplicate(result->d_name);
		delete[] entry;
		return retval;
	#else
		#ifdef RUDIMENTS_HAVE_DIRENT_H
			dirent	*entry;
		#else
			direct	*entry;
		#endif
		if (_rdmutex && !_rdmutex->lock()) {
			return NULL;
		}
		do {
			entry=readdir(pvt->_dir);
		} while (!entry && error::getErrorNumber()==EINTR);
		if (!entry) {
			return NULL;
		}
		pvt->_currentindex++;
		char	*retval=charstring::duplicate(entry->d_name);
		if (_rdmutex) {
			_rdmutex->unlock();
		}
		return retval;
	#endif
}

void directory::rewind() {
	if (pvt->_dir) {
		rewinddir(pvt->_dir);
	}
	pvt->_currentindex=0;
}

bool directory::close() {
	bool	retval=true;
	if (pvt->_dir) {
		do {
			retval=!closedir(pvt->_dir);
		} while (!retval && error::getErrorNumber()==EINTR);
		pvt->_dir=NULL;
		pvt->_currentindex=0;
	}
	return retval;
}

uint64_t directory::getChildCount() {
	rewind();
	uint64_t	count=0;
	while (skip()) {
		count++;
	}
	return count;
}

char *directory::getChildName(uint64_t index) {

	// handle unopened directory
	if (!pvt->_dir) {
		return NULL;
	}

	// rewind if necessary
	if (index<pvt->_currentindex) {
		rewind();
	}

	// skip to the index we want to get
	for (uint64_t i=pvt->_currentindex; i<index; i++) {
		skip();
	}

	// return the name at the requested index
	return read();
}

bool directory::create(const char *path, mode_t perms) {
	int32_t	result;
	do {
		#if defined(RUDIMENTS_HAVE_MKDIR_2)
			result=mkdir(path,perms);
		#elif defined(RUDIMENTS_HAVE_MKDIR_1)
			result=mkdir(path);
		#else
			#error no mkdir or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	#if defined(RUDIMENTS_HAVE_MKDIR_2)
		return !result;
	#elif defined(RUDIMENTS_HAVE_MKDIR_1)
		return !result && permissions::setFilePermissions(path,perms);
	#else
		#error no mkdir or anything like it
	#endif
}

bool directory::remove(const char *path) {
	int32_t	result;
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
	int32_t	result;
	do {
		result=chdir(path);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool directory::changeRoot(const char *path) {
	#ifdef RUDIMENTS_HAVE_CHROOT
		int32_t	result;
		do {
			result=chroot(path);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(_WIN32)
		// windows just doesn't support this
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		// other platforms should support this
		#error no chroot or anything like it
	#endif
}

bool directory::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_READDIR_R)
		return true;
	#else
		return false;
	#endif
}

void directory::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_READDIR_R)
		_rdmutex=mtx;
	#endif
}

int64_t directory::maxFileNameLength(const char *pathname) {
	int64_t	retval=pathConf(pathname,_PC_NAME_MAX);
	#if defined(NAME_MAX)
	if (retval==-1) {
		retval = NAME_MAX;
	}
	#endif
	return retval;
}

int64_t directory::maxPathLength(const char *pathname) {
	return pathConf(pathname,_PC_PATH_MAX);
}

bool directory::canAccessLongFileNames(const char *pathname) {
	return !pathConf(pathname,_PC_NO_TRUNC);
}

int64_t directory::pathConf(const char *pathname, int32_t name) {
	#if defined(RUDIMENTS_HAVE_PATHCONF)
		int64_t	result;
		do {
			result=pathconf(pathname,name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#elif defined(_WIN32)
		// no idea how to support this on windows
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no pathconf or anything like it
	#endif
}

int64_t directory::maxFileNameLength() {
	int64_t	retval=fpathConf(_PC_NAME_MAX);
	#if defined(NAME_MAX)
	if (retval==-1) {
		retval = NAME_MAX;
	}
	#endif
	return retval;
}

int64_t directory::maxPathLength() {
	return fpathConf(_PC_PATH_MAX);
}

bool directory::canAccessLongFileNames() {
	return !fpathConf(_PC_NO_TRUNC);
}

int64_t directory::fpathConf(int32_t name) {
	#if defined(RUDIMENTS_HAVE_FPATHCONF)
		int64_t	result;
		do {
			result=fpathconf(
					#if defined(RUDIMENTS_HAVE_DIRFD)
						dirfd(pvt->_dir)
					#elif defined(RUDIMENTS_HAVE_DIR_DD_FD)
						pvt->_dir->dd_fd
					#elif defined(RUDIMENTS_HAVE_DIR_D_FD)
						pvt->_dir->d_fd
					#else
						#error need dirfd replacement
					#endif
					,name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#elif defined(_WIN32)
		// no idea how to support this on windows
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no pathconf or anything like it
	#endif
}
