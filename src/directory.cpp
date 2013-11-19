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
		#ifndef _WIN32
			DIR		*_dir;
		#else
			char		*_filespec;
			HANDLE		_dir;
			WIN32_FIND_DATA	_findfiledata;
			bool		_onfirst;
		#endif
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
	#ifndef _WIN32
		pvt->_dir=NULL;
	#else
		pvt->_filespec=NULL;
		pvt->_dir=INVALID_HANDLE_VALUE;
		pvt->_onfirst=true;
	#endif
	pvt->_currentindex=0;
}

directory::~directory() {
	close();
	#ifdef _WIN32
		delete[] pvt->_filespec;
	#endif
	delete pvt;
}

bool directory::open(const char *path) {
	close();
	#ifndef _WIN32
		do {
			pvt->_dir=opendir(path);
		} while (pvt->_dir==NULL && error::getErrorNumber()==EINTR);
		return (pvt->_dir!=NULL);
	#else
		// The rewind method just calls close() and
		// open(pvt->_filespec).  Don't rebuild pvt->_filespec if it's
		// what was passed in.
		if (path!=pvt->_filespec) {
			delete[] pvt->_filespec;
			pvt->_filespec=new char[charstring::length(path)+3];
			charstring::copy(pvt->_filespec,path);
			charstring::append(pvt->_filespec,"\\*");
		}
		pvt->_dir=FindFirstFile(pvt->_filespec,&pvt->_findfiledata);
		pvt->_onfirst=true;
		return (pvt->_dir!=INVALID_HANDLE_VALUE);
	#endif
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

	// handle unopened directory
	#ifndef _WIN32
		if (!pvt->_dir) {
			return NULL;
		}
	#else
		if (pvt->_dir==INVALID_HANDLE_VALUE) {
			return NULL;
		}
	#endif

	#if defined(RUDIMENTS_HAVE_READDIR_R)
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
	#elif !defined(_WIN32)
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
	#else
		if (pvt->_onfirst) {
			pvt->_onfirst=false;
		} else if (FindNextFile(pvt->_dir,&pvt->_findfiledata)!=TRUE) {
			return NULL;
		}
		pvt->_currentindex++;
		return charstring::duplicate(pvt->_findfiledata.cFileName);
	#endif
}

void directory::rewind() {
	#ifndef _WIN32
		if (pvt->_dir) {
			rewinddir(pvt->_dir);
		}
		pvt->_currentindex=0;
	#else
		close();
		open(pvt->_filespec);
	#endif
}

bool directory::close() {
	#ifndef _WIN32
		bool	retval=true;
		if (pvt->_dir) {
			do {
				retval=!closedir(pvt->_dir);
			} while (!retval && error::getErrorNumber()==EINTR);
			pvt->_dir=NULL;
			pvt->_currentindex=0;
		}
		return retval;
	#else
		bool	retval=(FindClose(pvt->_dir)==TRUE);
		pvt->_dir=INVALID_HANDLE_VALUE;
		return retval;
	#endif
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
	#ifndef _WIN32
		if (!pvt->_dir) {
			return NULL;
		}
	#else
		if (pvt->_dir==INVALID_HANDLE_VALUE) {
			return NULL;
		}
	#endif

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
	#ifndef _WIN32
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
			return !result &&
				permissions::setFilePermissions(path,perms);
		#else
			#error no mkdir or anything like it
		#endif
	#else
		return (CreateDirectory(path,NULL)==TRUE);

		// init the security attributes
		/*LPSECURITY_ATTRIBUTES	satt;
		satt.nLength=sizeof(LPSECURITY_ATTRIBUTES);
		satt.bInheritHandle=TRUE;

		// contruct the security descriptor
		if (InitializeSecurityDescriptor(
					&satt.lpSecurityDescriptor,
					SECURITY_DESCRIPTOR_REVISION)!=NTRUE) {
			return false;
		}

		// FIXME: set directory permissions...

		// create the directory
		return (CreateDirectory(path,&satt)==TRUE);*/
	#endif
}

bool directory::remove(const char *path) {
	#ifndef _WIN32
		int32_t	result;
		do {
			result=rmdir(path);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		return (RemoveDirectory(path)==TRUE);
	#endif
}

char *directory::getCurrentWorkingDirectory() {
	size_t	size=1024;
	for (;;) {
		char	*buffer=new char[size];
		char	*result=NULL;
		#ifndef _WIN32
			do {
				result=getcwd(buffer,size);
			} while (!result && error::getErrorNumber()==EINTR);
		#else
			DWORD	charswritten=GetCurrentDirectory(size,buffer);
			if (!charswritten) {
				delete[] buffer;
				return NULL;
			}
			if (charswritten<size) {
				result=buffer;
			}
		#endif
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
	#ifndef _WIN32
		int32_t	result;
		do {
			result=chdir(path);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		return (SetCurrentDirectory(path)==TRUE);
	#endif
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
		return false;
	#else
		// other platforms should support this
		#error no chroot or anything like it
	#endif
}

bool directory::needsMutex() {
	#if defined(_WIN32)
		return false;
	#elif !defined(RUDIMENTS_HAVE_READDIR_R)
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
		retval=NAME_MAX;
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
		if (name==_PC_PATH_MAX || name==_PC_NAME_MAX) {
			return MAX_PATH;
		} else if (name==_PC_NO_TRUNC) {
			return 0;
		}
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
		if (name==_PC_PATH_MAX || name==_PC_NAME_MAX) {
			return MAX_PATH;
		} else if (name==_PC_NO_TRUNC) {
			return 0;
		}
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no pathconf or anything like it
	#endif
}
