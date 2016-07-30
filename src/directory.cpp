// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
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
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#include <stdio.h>

#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
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

#ifndef MAX_PATH
	// non-windows doesn't define this ,but we need it
	// internally to this file
	#define MAX_PATH 256
#endif

class directoryprivate {
	friend class directory;
	private:
		#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
			char		*_filespec;
			HANDLE		_dir;
			WIN32_FIND_DATA	_findfiledata;
			bool		_onfirst;
		#else
			DIR		*_dir;
		#endif
		uint64_t	_currentindex;
};


directory::directory() {
	pvt=new directoryprivate;
	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
		pvt->_filespec=NULL;
		pvt->_dir=INVALID_HANDLE_VALUE;
		pvt->_onfirst=true;
	#else
		pvt->_dir=NULL;
	#endif
	pvt->_currentindex=0;
}

directory::~directory() {
	close();
	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
		delete[] pvt->_filespec;
	#endif
	delete pvt;
}

bool directory::open(const char *path) {
	close();
	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
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
	#else
		error::clearError();
		do {
			pvt->_dir=opendir(path);
		} while (pvt->_dir==NULL && error::getErrorNumber()==EINTR);
		return (pvt->_dir!=NULL);
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
	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
		if (pvt->_dir==INVALID_HANDLE_VALUE) {
			return NULL;
		}
	#else
		if (!pvt->_dir) {
			return NULL;
		}
	#endif

	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
		if (pvt->_onfirst) {
			pvt->_onfirst=false;
		} else if (FindNextFile(pvt->_dir,&pvt->_findfiledata)!=TRUE) {
			return NULL;
		}
		pvt->_currentindex++;
		return charstring::duplicate(pvt->_findfiledata.cFileName);
	#else
		#ifdef RUDIMENTS_HAVE_DIRENT_H
			dirent	*entry;
		#else
			direct	*entry;
		#endif
		error::clearError();
		do {
			entry=readdir(pvt->_dir);
		} while (!entry && error::getErrorNumber()==EINTR);
		if (!entry) {
			return NULL;
		}
		pvt->_currentindex++;
		char	*retval=charstring::duplicate(entry->d_name);
		return retval;
	#endif
}

void directory::rewind() {
	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
		close();
		open(pvt->_filespec);
	#else
		if (pvt->_dir) {
			rewinddir(pvt->_dir);
		}
	#endif
	pvt->_currentindex=0;
}

bool directory::close() {
	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
		bool	retval=(FindClose(pvt->_dir)==TRUE);
		pvt->_dir=INVALID_HANDLE_VALUE;
		pvt->_currentindex=0;
		return retval;
	#else
		bool	retval=true;
		if (pvt->_dir) {
			error::clearError();
			do {
				retval=!closedir(pvt->_dir);
			} while (!retval && error::getErrorNumber()==EINTR);
			pvt->_dir=NULL;
			pvt->_currentindex=0;
		}
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
	#if defined(RUDIMENTS_HAVE_FINDFIRSTFILE)
		if (pvt->_dir==INVALID_HANDLE_VALUE) {
			return NULL;
		}
	#else
		if (!pvt->_dir) {
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
	#if defined(RUDIMENTS_HAVE_CREATEDIRECTORY)

		// create security descriptor
		PSECURITY_DESCRIPTOR	psd=
			(PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
					SECURITY_DESCRIPTOR_MIN_LENGTH);
		if (!InitializeSecurityDescriptor(psd,
					SECURITY_DESCRIPTOR_REVISION)) {
			LocalFree(psd);
			return false;
		}
		void	*dacl=permissions::permOctalToDacl(perms,true);
		if (!SetSecurityDescriptorDacl(psd,TRUE,(PACL)dacl,FALSE)
			#if _WIN32_WINNT>=0x0500
			|| !SetSecurityDescriptorControl(psd,
				SE_DACL_PROTECTED|SE_DACL_AUTO_INHERITED,
				SE_DACL_PROTECTED|SE_DACL_AUTO_INHERITED)
			#endif
			) {
			LocalFree(dacl);
			LocalFree(psd);
			return false;
		}

		// create security attributes
		SECURITY_ATTRIBUTES	satt;
		satt.nLength=sizeof(SECURITY_ATTRIBUTES);
		satt.lpSecurityDescriptor=psd;
		satt.bInheritHandle=TRUE;

		// create directory
		bool	retval=(CreateDirectory(path,&satt)==TRUE);

		// clean up
		LocalFree(dacl);
		LocalFree(psd);

		return retval;
	#else
		int32_t	result;
		error::clearError();
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
	#endif
}

bool directory::remove(const char *path) {
	#if defined(RUDIMENTS_HAVE_REMOVEDIRECTORY)
		return (RemoveDirectory(path)==TRUE);
	#else
		int32_t	result;
		error::clearError();
		do {
			result=rmdir(path);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#endif
}

char *directory::getCurrentWorkingDirectory() {
	size_t	inc=MAX_PATH;
	size_t	max=inc*10;
	for (size_t size=inc; size<max; size=size+inc) {
		char	*buffer=new char[size];
		char	*result=NULL;
		#if defined(RUDIMENTS_HAVE_GETCURRENTDIRECTORY)
			DWORD	charswritten=GetCurrentDirectory(size,buffer);
			if (!charswritten) {
				delete[] buffer;
				return NULL;
			}
			if (charswritten<size) {
				result=buffer;
			}
		#else
			error::clearError();
			do {
				result=getcwd(buffer,size);
			} while (!result && error::getErrorNumber()==EINTR);
		#endif
		if (result) {
			return buffer;
		} else {
			delete[] buffer;
		}
	}
	return NULL;
}

bool directory::changeDirectory(const char *path) {
	#if defined(RUDIMENTS_HAVE_SETCURRENTDIRECTORY)
		return (SetCurrentDirectory(path)==TRUE);
	#else
		int32_t	result;
		error::clearError();
		do {
			result=chdir(path);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#endif
}

bool directory::changeRoot(const char *path) {
	#ifdef RUDIMENTS_HAVE_CHROOT
		int32_t	result;
		error::clearError();
		do {
			result=chroot(path);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
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

bool directory::canExceedMaxFileNameLength(const char *pathname) {
	return !pathConf(pathname,_PC_NO_TRUNC);
}

int64_t directory::pathConf(const char *pathname, int32_t name) {
	#if defined(RUDIMENTS_HAVE_PATHCONF)
		int64_t	result;
		error::clearError();
		do {
			result=pathconf(pathname,name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		if (name==_PC_PATH_MAX || name==_PC_NAME_MAX) {
			return MAX_PATH;
		} else if (name==_PC_NO_TRUNC) {
			return 0;
		}
		RUDIMENTS_SET_ENOSYS
		return -1;
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

bool directory::canExceedMaxFileNameLength() {
	return !fpathConf(_PC_NO_TRUNC);
}

int64_t directory::fpathConf(int32_t name) {
	#if defined(RUDIMENTS_HAVE_FPATHCONF) && \
			(defined(RUDIMENTS_HAVE_DIRFD) || \
			defined(RUDIMENTS_HAVE_DIR_DD_FD) || \
			defined(RUDIMENTS_HAVE_DIR_D_FD))
		int64_t	result;
		error::clearError();
		do {
			result=fpathconf(
					#if defined(RUDIMENTS_HAVE_DIRFD)
						dirfd(pvt->_dir)
					#elif defined(RUDIMENTS_HAVE_DIR_DD_FD)
						pvt->_dir->dd_fd
					#elif defined(RUDIMENTS_HAVE_DIR_D_FD)
						pvt->_dir->d_fd
					#endif
					,name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		if (name==_PC_PATH_MAX || name==_PC_NAME_MAX) {
			return MAX_PATH;
		} else if (name==_PC_NO_TRUNC) {
			return 0;
		}
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}
