// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/directory.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

// for DIR
#ifdef RUDIMENTS_HAVE_DIRENT_H
	#include <dirent.h>
#else
	#include <direct.h>
#endif

#include <stdlib.h>
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <sys/stat.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class directoryprivate {
	friend class directory;
	private:
		DIR		*_dir;
		uint64_t	_currentindex;
};

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_READDIR_R)
static mutex	*_rdmutex;
#endif


// lame that this isn't part of the class, but I can't think of another way to
// keep #ifndef RUDIMENTS_HAVE_DIRENT_H out of the header file
#ifdef RUDIMENTS_HAVE_READDIR_R
static int64_t bufferSize(directory *d, DIR *dirp) {
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
	do {
		pvt->_dir=opendir(path);
	} while (pvt->_dir==NULL && error::getErrorNumber()==EINTR);
	return (pvt->_dir!=NULL);
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

	// handle unopened directory
	if (!pvt->_dir) {
		return 0;
	}

	// rewind
	rewinddir(pvt->_dir);
	pvt->_currentindex=0;

	#ifdef RUDIMENTS_HAVE_READDIR_R
		// get the size of the buffer
		int64_t	size=bufferSize(this,pvt->_dir);
		if (size==-1) {
			return 0;
		}
		#ifdef RUDIMENTS_HAVE_DIRENT_H
			dirent	*entry=reinterpret_cast<dirent *>(
						new unsigned char[size]);
			dirent	*result;
		#else
			direct	*entry=reinterpret_cast<direct *>(
						new unsigned char[size]);
			dirent	*result;
		#endif
		for (;;) {
			int	rdresult;
			do {
				rdresult=readdir_r(pvt->_dir,entry,&result);
			} while (rdresult==-1 &&
					error::getErrorNumber()==EINTR);
			if (!result) {
				delete[] entry;
				return pvt->_currentindex;
			}
			pvt->_currentindex++;
		}
	#else
		#ifdef RUDIMENTS_HAVE_DIRENT_H
			dirent	*entry;
		#else
			direct	*entry;
		#endif
		if (_rdmutex && !_rdmutex->lock()) {
			return 0;
		}
		for (;;) {
			do {
				entry=readdir(pvt->_dir);
			} while (!entry && error::getErrorNumber()==EINTR);
			if (!entry) {
				if (_rdmutex) {
					_rdmutex->unlock();
				}
				return pvt->_currentindex;
			}
			pvt->_currentindex++;
		}
	#endif
}

char *directory::getChildName(uint64_t index) {

	// directory entries are 1-based
	uint64_t	actualindex=index+1;

	if (actualindex<pvt->_currentindex) {

		// handle unopened directory
		if (!pvt->_dir) {
			return NULL;
		}

		// rewind
		rewinddir(pvt->_dir);
		pvt->_currentindex=0;
	}

	#ifdef RUDIMENTS_HAVE_READDIR_R
		// get the size of the buffer
		int64_t	size=bufferSize(this,pvt->_dir);
		if (size==-1) {
			return NULL;
		}
		#ifdef RUDIMENTS_HAVE_DIRENT_H
			dirent	*entry=reinterpret_cast<dirent *>(
						new unsigned char[size]);
			dirent	*result;
		#else
			direct	*entry=reinterpret_cast<direct *>(
						new unsigned char[size]);
			dirent	*result;
		#endif
		for (uint64_t i=pvt->_currentindex; i<actualindex; i++) {
			int	rdresult;
			do {
				rdresult=readdir_r(pvt->_dir,entry,&result);
			} while (rdresult==-1 &&
					error::getErrorNumber()==EINTR);
			if (rdresult || !result) {
				delete[] entry;
				return NULL;
			}
			pvt->_currentindex++;
		}
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
		for (uint64_t i=pvt->_currentindex; i<actualindex; i++) {
			do {
				entry=readdir(pvt->_dir);
			} while (!entry && error::getErrorNumber()==EINTR);
			if (!entry) {
				return NULL;
			}
			pvt->_currentindex++;
		}
		char	*retval=charstring::duplicate(entry->d_name);
		if (_rdmutex) {
			_rdmutex->unlock();
		}
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

bool directory::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_READDIR_R)
		return true;
	#else
		return false;
	#endif
}

void directory::setMutex(mutex *mtx) {
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

int64_t directory::pathConf(const char *pathname, int name) {
	int64_t	result;
	do {
		result=pathconf(pathname,name);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
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

int64_t directory::fpathConf(int name) {
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
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
