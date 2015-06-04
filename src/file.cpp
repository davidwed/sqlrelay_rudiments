// Copyright (c) 2002 David Muse
// See the COPYING file for more information


#include <rudiments/file.h>
#include <rudiments/userentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#ifndef RUDIMENTS_HAVE_BLKSIZE_T
	#include <rudiments/filesystem.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

// for struct stat
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif

#if !defined(RUDIMENTS_HAVE_UTIMES_CONST_CHAR) && \
		!defined(RUDIMENTS_HAVE_UTIMES_CHAR)
	#include <rudiments/datetime.h>
#endif
#ifndef RUDIMENTS_HAVE_MKSTEMP
	#include <rudiments/datetime.h>
	#include <rudiments/randomnumber.h>
	#include <rudiments/permissions.h>
#endif

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef RUDIMENTS_HAVE_UTIME_H
	#include <utime.h>
#endif
#ifdef RUDIMENTS_HAVE_IO_H
	#undef _POSIX_
	#include <io.h>
#endif
#ifdef RUDIMENTS_HAVE_SDDL_H
	#include <sddl.h>
#endif
#ifdef RUDIMENTS_HAVE_ACLAPI_H
	#include <aclapi.h>
#endif
#ifdef RUDIMENTS_HAVE_ACCCTRL_H
	#include <accctrl.h>
#endif

// windows doesn't define these, but we need them
// internally to this file
#ifndef F_GETLK	
	#define F_GETLK		0
#endif
#ifndef F_SETLK	
	#define F_SETLK		1
#endif
#ifndef F_SETLKW	
	#define F_SETLKW	2
#endif
#ifndef _PC_LINK_MAX
	#define _PC_LINK_MAX		0
#endif
#ifndef _PC_CHOWN_RESTRICTED
	#define _PC_CHOWN_RESTRICTED	1
#endif
#ifndef F_OK
	#define F_OK	0
#endif
#ifndef W_OK
	#define W_OK	2
#endif
#ifndef R_OK
	#define R_OK	4
#endif
#ifndef X_OK
	// no such thing on windows, so we'll
	// just set this to be the same as F_OK
	#define X_OK	0
#endif

class fileprivate {
	friend class file;
	private:
		struct	stat	_st;
		#if defined(RUDIMENTS_HAVE_GETFILETYPE)
			DWORD		_filetype;
		#endif
		#if defined(RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE)
			uint64_t	_inode;
		#endif
		#ifndef RUDIMENTS_HAVE_BLKSIZE_T
			char		*_name;
			blksize_t	_blocksize;
		#endif
		bool	_getcurrentpropertiesonopen;
};

file::file() : filedescriptor() {
	pvt=new fileprivate;
	bytestring::zero(&pvt->_st,sizeof(pvt->_st));
	#if defined(RUDIMENTS_HAVE_GETFILETYPE)
		pvt->_filetype=0;
	#endif
	#if defined(RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE)
		pvt->_inode=0;
	#endif
	#ifndef RUDIMENTS_HAVE_BLKSIZE_T
		pvt->_name=NULL;
		pvt->_blocksize=0;
	#endif
	pvt->_getcurrentpropertiesonopen=true;
	type("file");
}

file::file(const file &f) : filedescriptor(f) {
	pvt=new fileprivate;
	fileClone(f);
	type("file");
}

file &file::operator=(const file &f) {
	if (this!=&f) {
		filedescriptor::operator=(f);
		fileClone(f);
	}
	return *this;
}

void file::fileClone(const file &f) {
	pvt->_st=f.pvt->_st;
	#if defined(RUDIMENTS_HAVE_GETFILETYPE)
		pvt->_filetype=f.pvt->_filetype;
	#endif
	#if defined(RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE)
		pvt->_inode=f.pvt->_inode;
	#endif
	#ifndef RUDIMENTS_HAVE_BLKSIZE_T
		pvt->_name=charstring::duplicate(f.pvt->_name);
		pvt->_blocksize=f.pvt->_blocksize;
	#endif
	pvt->_getcurrentpropertiesonopen=f.pvt->_getcurrentpropertiesonopen;
}

file::~file() {
	#ifndef RUDIMENTS_HAVE_BLKSIZE_T
		delete[] pvt->_name;
	#endif
	delete pvt;
}

bool file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

bool file::createFile(const char *name, mode_t perms) {
	file	fl;
	return fl.create(name,perms);
}

void file::openInternal(const char *name, int32_t flags,
				mode_t perms, bool useperms) {

	#ifndef RUDIMENTS_HAVE_BLKSIZE_T
		delete[] pvt->_name;
		pvt->_name=charstring::duplicate(name);
	#endif

	#ifdef RUDIMENTS_HAVE_CREATEFILE

		// FIXME: remove this when permissions::permOctalToDacl()
		// works on NT4
		#if _WIN32_WINNT>=0x0500

		// On Windows, when creating a file, in order to set permissions
		// other than just owner read/write, the CreateFile method must
		// be used rather than just plain _open.
		// Also, on Windows, if you want to be able to set file
		// ownership and permissions, you have to use CreateFile just to
		// open the file so that the access mode can include WRITE_DAC
		// and WRITE_OWNER, which are not set when using _open.

		PSECURITY_DESCRIPTOR	psd=NULL;
		void			*dacl=NULL;
		PSECURITY_ATTRIBUTES	psatt=NULL;

		if (flags&O_CREAT) {

			// create security descriptor
			psd=(PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
					SECURITY_DESCRIPTOR_MIN_LENGTH);
			if (!InitializeSecurityDescriptor(psd,
					SECURITY_DESCRIPTOR_REVISION)) {
				fd(-1);
				LocalFree(psd);
				return;
			}
			dacl=permissions::permOctalToDacl(perms,true);
			if (!SetSecurityDescriptorDacl(psd,TRUE,
						(PACL)dacl,FALSE)
				#if _WIN32_WINNT>=0x0500
				|| !SetSecurityDescriptorControl(psd,
						SE_DACL_PROTECTED,
						SE_DACL_PROTECTED)
				#endif
				) {
				fd(-1);
				LocalFree(dacl);
				LocalFree(psd);
				return;
			}

			// create security attributes
			SECURITY_ATTRIBUTES	satt;
			satt.nLength=sizeof(SECURITY_ATTRIBUTES);
			satt.lpSecurityDescriptor=psd;
			satt.bInheritHandle=TRUE;
			psatt=&satt;
		}

		#endif

		// Determine the access and share modes.
		// O_RDONLY, O_WRONLY and O_RDWR are usually 0, 1 and 2.
		// We can use & to test for O_WRONLY and O_RDWR but we can't
		// effectively test for O_RDONLY at all, except by testing for
		// the lack of the other two.  It's easier to default to
		// read-only and just update the access/share modes if we
		// detect O_WRONLY/O_RDWR.
		DWORD	accessmode=GENERIC_READ|READ_CONTROL;
		if (flags&O_WRONLY) {
			accessmode=GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER;
		}
		if (flags&O_RDWR) {
			accessmode=GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|
						GENERIC_READ|READ_CONTROL;
		}

		// determine the creation disposition
		DWORD	cdisp=0;
		if (flags&O_CREAT) {
			if (flags&O_EXCL) {
				cdisp=CREATE_NEW;
			} else {
				// O_CREAT without O_EXCL should create the
				// file if it doesn't exist but otherwise just
				// open it normally.
				// CREATE_ALWAYS creates the file if it doesn't
				// exist, but otherwise "replaces" it - ie. it
				// opens and truncates it.
				// We need to open the file normally if it
				// exists (OPEN_EXISTING) and otherwise create
				// it (CREATE_ALWAYS).
				// Unfortunately we can't do all of that
				// atomically.
				cdisp=(exists(name))?
					OPEN_EXISTING:CREATE_ALWAYS;
			}
		} else {
			cdisp=OPEN_EXISTING;
		}

		// determine the attrs
		// FILE_FLAG_BACKUP_SEMANTICS must be used when opening a
		// directory, for some reason
		DWORD	attrs=FILE_ATTRIBUTE_NORMAL;
		DWORD	fileattr=GetFileAttributes(name);
		if (fileattr&FILE_ATTRIBUTE_DIRECTORY) {
			attrs=FILE_FLAG_BACKUP_SEMANTICS;
		}

		// FIXME: remove this when permissions::permOctalToDacl()
		// works on NT4
		#if _WIN32_WINNT>=0x0500

		// create/open the file
		HANDLE	fh=CreateFile(name,accessmode,
					FILE_SHARE_DELETE|
					FILE_SHARE_READ|
					FILE_SHARE_WRITE,
					psatt,cdisp,attrs,NULL);
		LocalFree(psd);
		LocalFree(dacl);

		#else

		// create/open the file
		HANDLE	fh=CreateFile(name,accessmode,
					FILE_SHARE_DELETE|
					FILE_SHARE_READ|
					FILE_SHARE_WRITE,
					NULL,cdisp,attrs,NULL);
		#endif

		if (fh==INVALID_HANDLE_VALUE) {
			fd(-1);
			return;
		}

		// get the file descriptor from the handle
		fd(_open_osfhandle((long)fh,flags&~(O_CREAT|O_TRUNC)));

		// truncate if necessary
		if (flags&O_TRUNC) {
			truncate();
		}

		// append if specified
		if (flags&O_APPEND) {
			setPositionRelativeToEnd(0);
		}
	#else

		int32_t	result;
		error::clearError();
		do {
			if (useperms) {
				#if defined(RUDIMENTS_HAVE__OPEN)
					result=_open(name,flags,perms);
				#elif defined(RUDIMENTS_HAVE_OPEN)
					result=::open(name,flags,perms);
				#else
					#error no open or anything like it
				#endif
			} else {
				#if defined(RUDIMENTS_HAVE__OPEN)
					result=_open(name,flags);
				#elif defined(RUDIMENTS_HAVE_OPEN)
					result=::open(name,flags);
				#else
					#error no open or anything like it
				#endif
			}
		} while (result==-1 && error::getErrorNumber()==EINTR);
		fd(result);
	#endif
}

bool file::open(const char *name, int32_t flags) {
	openInternal(name,flags,0,false);
	return (fd()!=-1 &&
		((pvt->_getcurrentpropertiesonopen)?
				getCurrentProperties():true));
}

bool file::open(const char *name, int32_t flags, mode_t perms) {
	openInternal(name,flags,perms,true);
	return (fd()!=-1 &&
		((pvt->_getcurrentpropertiesonopen)?
				getCurrentProperties():true));
}

char *file::getContents() {
	off64_t	size=(fd()>-1)?pvt->_st.st_size:0;
	char	*contents=new char[size+1];
	contents[size]='\0';
	return (size==0 || read(contents,size)==size)?contents:NULL;
}

char *file::getContents(const char *name) {
	file	fl;
	fl.open(name,O_RDONLY);
	char	*contents=fl.getContents();
	fl.close();
	return contents;
}

ssize_t file::getContents(unsigned char *buffer, size_t buffersize) {
	return read(buffer,(buffersize<(size_t)getSize())?buffersize:getSize());
}

ssize_t file::getContents(const char *name, unsigned char *buffer,
						size_t buffersize) {
	file	fl;
	fl.open(name,O_RDONLY);
	ssize_t	bytes=fl.getContents(buffer,buffersize);
	fl.close();
	return bytes;
}


bool file::tryLockFile(int16_t type) const {
	return tryLockRegion(type,0,0);
}

bool file::lockFile(int16_t type) const {
	return lockRegion(type,0,0);
}

bool file::checkLockFile(int16_t type,
			int16_t *conftype, int16_t *confwhence,
			off64_t *confstart, off64_t *conflen) const {
	return checkLockRegion(type,0,0,conftype,confwhence,confstart,conflen);
}

bool file::unlockFile() const {
	return unlockRegion(0,0);
}

bool file::tryLockRegion(int16_t type, off64_t start, off64_t len) const {
	return lock(F_SETLK,type,SEEK_SET,start,len);
}

bool file::lockRegion(int16_t type, off64_t start, off64_t len) const {
	return lock(F_SETLKW,type,SEEK_SET,start,len);
}

bool file::checkLockRegion(int16_t type, off64_t start, off64_t len,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLock(type,SEEK_SET,start,len,
				conftype,confwhence,confstart,conflen);
}

bool file::unlockRegion(off64_t start, off64_t len) const {
	return unlock(SEEK_SET,start,len);
}

bool file::tryLockFromCurrent(int16_t type, off64_t len) const {
	return tryLockFromCurrent(type,0,len);
}

bool file::tryLockFromCurrent(int16_t type, off64_t start, off64_t len) const {
	return lock(F_SETLK,type,SEEK_CUR,start,len);
}

bool file::lockFromCurrent(int16_t type, off64_t len) const {
	return lockFromCurrent(type,0,len);
}

bool file::lockFromCurrent(int16_t type, off64_t start, off64_t len) const {
	return lock(F_SETLKW,type,SEEK_CUR,start,len);
}

bool file::checkLockFromCurrent(int16_t type, off64_t len,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLockFromCurrent(type,0,len,
				conftype,confwhence,confstart,conflen);
}

bool file::checkLockFromCurrent(int16_t type, off64_t start, off64_t len,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLock(type,SEEK_CUR,start,len,
				conftype,confwhence,confstart,conflen);
}

bool file::unlockFromCurrent(off64_t len) const {
	return unlockFromCurrent(0,len);
}

bool file::unlockFromCurrent(off64_t start, off64_t len) const {
	return unlock(SEEK_CUR,start,len);
}

bool file::tryLockFromEnd(int16_t type, off64_t len) const {
	return tryLockFromEnd(type,0,len);
}

bool file::tryLockFromEnd(int16_t type, off64_t start, off64_t len) const {
	return lock(F_SETLK,type,SEEK_END,start,len);
}

bool file::lockFromEnd(int16_t type, off64_t len) const {
	return lockFromEnd(type,0,len);
}

bool file::lockFromEnd(int16_t type, off64_t start, off64_t len) const {
	return lock(F_SETLKW,type,SEEK_END,start,len);
}

bool file::checkLockFromEnd(int16_t type, off64_t len,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLockFromEnd(type,0,len,
				conftype,confwhence,confstart,conflen);
}

bool file::checkLockFromEnd(int16_t type, off64_t start, off64_t len,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLock(type,SEEK_END,start,len,
				conftype,confwhence,confstart,conflen);
}

bool file::unlockFromEnd(off64_t len) const {
	return unlockFromEnd(0,len);
}

bool file::unlockFromEnd(off64_t start, off64_t len) const {
	return unlock(SEEK_END,start,len);
}

bool file::tryLockRemainder(int16_t type, off64_t start) const {
	return lock(F_SETLK,type,SEEK_SET,start,0);
}

bool file::lockRemainder(int16_t type, off64_t start) const {
	return lock(F_SETLKW,type,SEEK_SET,start,0);
}

bool file::checkLockRemainder(int16_t type, off64_t start,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLock(type,SEEK_SET,start,0,
				conftype,confwhence,confstart,conflen);
}

bool file::unlockRemainder(off64_t start) const {
	return unlock(SEEK_SET,start,0);
}

bool file::tryLockRemainderFromCurrent(int16_t type) const {
	return tryLockRemainderFromCurrent(type,0);
}

bool file::tryLockRemainderFromCurrent(int16_t type, off64_t start) const {
	return lock(F_SETLK,type,SEEK_CUR,start,0);
}

bool file::lockRemainderFromCurrent(int16_t type) const {
	return lockRemainderFromCurrent(type,0);
}

bool file::lockRemainderFromCurrent(int16_t type, off64_t start) const {
	return lock(F_SETLKW,type,SEEK_CUR,start,0);
}

bool file::checkLockRemainderFromCurrent(int16_t type,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLockRemainderFromCurrent(type,0,
				conftype,confwhence,confstart,conflen);
}

bool file::checkLockRemainderFromCurrent(int16_t type, off64_t start,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLock(type,SEEK_CUR,start,0,
				conftype,confwhence,confstart,conflen);
}

bool file::unlockRemainderFromCurrent() const {
	return unlockRemainderFromCurrent(0);
}

bool file::unlockRemainderFromCurrent(off64_t start) const {
	return unlock(SEEK_CUR,start,0);
}

bool file::tryLockRemainderFromEnd(int16_t type) const {
	return tryLockRemainderFromEnd(type,0);
}

bool file::tryLockRemainderFromEnd(int16_t type, off64_t start) const {
	return lock(F_SETLK,type,SEEK_END,start,0);
}

bool file::lockRemainderFromEnd(int16_t type) const {
	return lockRemainderFromEnd(type,0);
}

bool file::lockRemainderFromEnd(int16_t type, off64_t start) const {
	return lock(F_SETLKW,type,SEEK_END,start,0);
}

bool file::checkLockRemainderFromEnd(int16_t type,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLockRemainderFromEnd(type,0,
				conftype,confwhence,confstart,conflen);
}

bool file::checkLockRemainderFromEnd(int16_t type, off64_t start,
				int16_t *conftype, int16_t *confwhence,
				off64_t *confstart, off64_t *conflen) const {
	return checkLock(type,SEEK_END,start,0,
				conftype,confwhence,confstart,conflen);
}

bool file::sequentialAccess(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_SEQUENTIAL)
		return posixFadvise(start,len,POSIX_FADV_SEQUENTIAL);
	#else
		return true;
	#endif
}

bool file::randomAccess(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_RANDOM)
		return posixFadvise(start,len,POSIX_FADV_RANDOM);
	#else
		return true;
	#endif
}

bool file::onlyOnce(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_NOREUSE)
		return posixFadvise(start,len,POSIX_FADV_NOREUSE);
	#else
		return true;
	#endif
}

bool file::willNeed(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_WILLNEED)
		return posixFadvise(start,len,POSIX_FADV_WILLNEED);
	#else
		return true;
	#endif
}

bool file::wontNeed(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_DONTNEED)
		return posixFadvise(start,len,POSIX_FADV_DONTNEED);
	#else
		return true;
	#endif
}

bool file::normalAccess(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_NORMAL)
		return posixFadvise(start,len,POSIX_FADV_NORMAL);
	#else
		return true;
	#endif
}

bool file::reserve(off64_t start, size_t len) const {
	#ifdef RUDIMENTS_HAVE_POSIX_FALLOCATE
		int32_t	result;
		error::clearError();
		do {
			result=posix_fallocate(fd(),start,len);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		return false;
	#endif
}

bool file::truncate(const char *filename) {
	return truncate(filename,0);
}

bool file::truncate(const char *filename, off64_t length) {
	#ifdef RUDIMENTS_HAVE_TRUNCATE
		int32_t	result;
		do {
			result=::truncate(filename,length);
		} while (result==-1 && error::getErrorNumber()==-1);
		return !result;
	#else
		file	f;
		return f.open(filename,O_WRONLY) && f.truncate(length);
	#endif
}

bool file::truncate() const {
	return truncate((off64_t)0);
}

bool file::truncate(off64_t length) const {
	int32_t	result;
	do {
		#if defined(RUDIMENTS_HAVE_FTRUNCATE)
			result=::ftruncate(fd(),length);
		#elif defined(RUDIMENTS_HAVE__CHSIZE_S)
			result=_chsize_s(fd(),length);
		#elif defined(RUDIMENTS_HAVE_SETENDOFFILE)
			return (setPositionRelativeToBeginning(0)!=-1 &&
				SetEndOfFile((HANDLE)
					getHandleFromFileDescriptor(fd()))==
					TRUE);
		#else
			#error no ftruncate or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==-1);
	return !result;
}

bool file::unlockRemainderFromEnd() const {
	return unlockRemainderFromEnd(0);
}

bool file::unlockRemainderFromEnd(off64_t start) const {
	return unlock(SEEK_END,start,0);
}

off64_t file::setPositionRelativeToBeginning(off64_t offset) const {
	return lseek(offset,SEEK_SET);
}

off64_t file::setPositionRelativeToCurrent(off64_t offset) const {
	return lseek(offset,SEEK_CUR);
}

off64_t file::setPositionRelativeToEnd(off64_t offset) const {
	return lseek(offset,SEEK_END);
}

off64_t file::getCurrentPosition() const {
	return lseek(0,SEEK_CUR);
}

off64_t file::lseek(off64_t offset, int32_t whence) const {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__LSEEK)
			result=_lseek(fd(),offset,whence);
		#elif defined(RUDIMENTS_HAVE_LSEEK)
			result=::lseek(fd(),offset,whence);
		#else
			#error no lseek or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool file::exists(const char *filename) {
	return accessible(filename,F_OK);
}

bool file::readable(const char *filename) {
	return accessible(filename,R_OK);
}

bool file::writeable(const char *filename) {
	return accessible(filename,W_OK);
}

bool file::executable(const char *filename) {
	return accessible(filename,X_OK);
}

bool file::accessible(const char *filename, int32_t mode) {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__ACCESS_S)
			result=_access_s(filename,mode);
		#elif defined(RUDIMENTS_HAVE_ACCESS)
			result=access(filename,mode);
		#else
			#error no access or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool file::getCurrentProperties() {
	int32_t	result;
	error::clearError();
	do {
		result=fstat(fd(),&pvt->_st);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result) {
		return false;
	}
	#if defined(RUDIMENTS_HAVE_GETFILETYPE)
		pvt->_filetype=GetFileType(
				(HANDLE)getHandleFromFileDescriptor(fd()));
	#endif
	#ifndef RUDIMENTS_HAVE_BLKSIZE_T
		filesystem	fs;
		pvt->_blocksize=0;
		if (fs.initialize(pvt->_name)) {
			pvt->_blocksize=fs.getBlockSize();
		}
	#endif
	#if defined(RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE)
		// On Windows, the st_ino and st_dev members aren't set.
		// Get the file index and volume serial numbers for these.

		BY_HANDLE_FILE_INFORMATION	bhfi;
		if (GetFileInformationByHandle(
				(HANDLE)getHandleFromFileDescriptor(fd()),
				&bhfi)==TRUE) {

			pvt->_inode=(((uint64_t)bhfi.nFileIndexHigh)<<32)|
							bhfi.nFileIndexLow;
			pvt->_st.st_dev=bhfi.dwVolumeSerialNumber;
		}
	#endif
	#if defined(RUDIMENTS_HAVE_GETSECURITYINFO)

		// FIXME: remove this when permissions::daclToPermOctal()
		// works on NT4
		#if _WIN32_WINNT>=0x0500

		// On Windows, the st_mode isn't set correctly.  Get the DACL
		// of the file and convert it to a mode_t...

		// get the security information
		PACL			dacl=NULL;
		PSECURITY_DESCRIPTOR	ppsd=NULL;
		if (GetSecurityInfo((HANDLE)getHandleFromFileDescriptor(fd()),
					SE_FILE_OBJECT,
					DACL_SECURITY_INFORMATION,
					NULL,NULL,&dacl,NULL,&ppsd)!=
							ERROR_SUCCESS) {
			return false;
		}

		// convert the dacl to perms
		pvt->_st.st_mode=permissions::daclToPermOctal((void *)dacl);

		// clean up
		LocalFree(ppsd);

		#else

		// FIXME: implement for WinNT

		#endif

	#endif
	return true;
}

bool file::stat(const char *filename, void *st) {
	int32_t	result;
	error::clearError();
	do {
		result=::stat(filename,(struct stat *)st);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return (result!=-1);
}

mode_t file::getPermissions() const {
	return pvt->_st.st_mode;
}

uid_t file::getOwnerUserId() const {
	return pvt->_st.st_uid;
}

gid_t file::getOwnerGroupId() const {
	return pvt->_st.st_gid;
}

off64_t file::getSize() const {
	return pvt->_st.st_size;
}

blksize_t file::getBlockSize() const {
	#ifdef RUDIMENTS_HAVE_BLKSIZE_T
		return pvt->_st.st_blksize;
	#else
		return pvt->_blocksize;
	#endif
}

blkcnt_t file::getBlockCount() const {
	#ifdef RUDIMENTS_HAVE_BLKCNT_T
		return pvt->_st.st_blocks;
	#else
		off64_t		size=getSize();
		blksize_t	blksize=getBlockSize();
		return (size && blksize)?(size/blksize+1):0;
	#endif
}

#ifndef RUDIMENTS_HAVE_S_ISSOCK
	#define S_ISSOCK(m) (((m&0140000)==0140000)?1:0)
#endif

int32_t file::isSocket() const {
	#if defined(_S_IFSOCK)
		return ((pvt->_st.st_mode&_S_IFSOCK)==_S_IFSOCK);
	#elif defined(S_IFSOCK)
		return ((pvt->_st.st_mode&S_IFSOCK)==S_IFSOCK);
	#else
		return S_ISSOCK(pvt->_st.st_mode);
	#endif
}

#ifndef RUDIMENTS_HAVE_S_ISLNK
	#define S_ISLNK(m) (((m&0120000)==0120000)?1:0)
#endif

int32_t file::isSymbolicLink() const {
	#if defined(_S_IFLNK)
		return ((pvt->_st.st_mode&_S_IFLNK)==_S_IFLNK);
	#elif defined(S_IFLNK)
		return ((pvt->_st.st_mode&S_IFLNK)==S_IFLNK);
	#else
		return S_ISLNK(pvt->_st.st_mode);
	#endif
}

int32_t file::isRegularFile() const {
	#if defined(RUDIMENTS_HAVE_GETFILETYPE)
		return (pvt->_filetype==FILE_TYPE_DISK);
	#elif defined(_S_IFREG)
		return ((pvt->_st.st_mode&_S_IFREG)==_S_IFREG);
	#elif defined(S_IFREG)
		return ((pvt->_st.st_mode&S_IFREG)==S_IFREG);
	#else
		return S_ISREG(pvt->_st.st_mode);
	#endif
}

#ifndef RUDIMENTS_HAVE_S_ISBLK
	#define S_ISBLK(m) (((m&0060000)==0060000)?1:0)
#endif

int32_t file::isBlockDevice() const {
	#if defined(_S_IFBLK)
		return ((pvt->_st.st_mode&_S_IFBLK)==_S_IFBLK);
	#elif defined(S_IFBLK)
		return ((pvt->_st.st_mode&S_IFBLK)==S_IFBLK);
	#else
		return S_ISBLK(pvt->_st.st_mode);
	#endif
}

int32_t file::isDirectory() const {
	#if defined(_S_IFDIR)
		return ((pvt->_st.st_mode&_S_IFDIR)==_S_IFDIR);
	#elif defined(S_IFDIR)
		return ((pvt->_st.st_mode&S_IFDIR)==S_IFDIR);
	#else
		return S_ISDIR(pvt->_st.st_mode);
	#endif
}

int32_t file::isCharacterDevice() const {
	#if defined(RUDIMENTS_HAVE_GETFILETYPE)
		return (pvt->_filetype==FILE_TYPE_CHAR);
	#elif defined(_S_IFCHR)
		return ((pvt->_st.st_mode&_S_IFCHR)==_S_IFCHR);
	#elif defined(S_IFCHR)
		return ((pvt->_st.st_mode&S_IFCHR)==S_IFCHR);
	#else
		return S_ISCHR(pvt->_st.st_mode);
	#endif
}

int32_t file::isFifo() const {
	#if defined(RUDIMENTS_HAVE_GETFILETYPE)
		return (pvt->_filetype==FILE_TYPE_PIPE);
	#elif defined(_S_IFIFO)
		return ((pvt->_st.st_mode&_S_IFIFO)==_S_IFIFO);
	#elif defined(S_IFIFO)
		return ((pvt->_st.st_mode&S_IFIFO)==S_IFIFO);
	#else
		return S_ISFIFO(pvt->_st.st_mode);
	#endif
}

time_t file::getLastAccessTime() const {
	return pvt->_st.st_atime;
}

time_t file::getLastModificationTime() const {
	return pvt->_st.st_mtime;
}

time_t file::getLastChangeTime() const {
	return pvt->_st.st_ctime;
}


dev_t file::getDevice() const {
	return pvt->_st.st_dev;
}

dev_t file::getDeviceType() const {
	return pvt->_st.st_rdev;
}

uint64_t file::getInode() const {
	#if defined(RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE)
		return pvt->_inode;
	#else
		// some platforms (OpenVMS) require this cast
		return (uint64_t)pvt->_st.st_ino;
	#endif
}

nlink_t file::getNumberOfHardLinks() const {
	return pvt->_st.st_nlink;
}

void file::getCurrentPropertiesOnOpen() {
	pvt->_getcurrentpropertiesonopen=true;
}

void file::dontGetCurrentPropertiesOnOpen() {
	pvt->_getcurrentpropertiesonopen=false;
}

bool file::lock(int32_t method, int16_t type,
			int16_t whence, off64_t start, off64_t len) const {
	#if defined(RUDIMENTS_HAVE_FCNTL)
		struct flock	lck;
		lck.l_type=type;
		lck.l_whence=whence;
		lck.l_start=start;
		lck.l_len=len;
		return !fCntl(method,reinterpret_cast<long>(&lck));
	#elif defined(RUDIMENTS_HAVE_LOCKFILEEX)
		off64_t	cur=getCurrentPosition();
		if (cur==-1) {
			return false;
		}
		LARGE_INTEGER	lockstart;
		lockstart.QuadPart=lseek(start,whence);
		if (lockstart.QuadPart==-1) {
			return false;
		}
		if (setPositionRelativeToBeginning(cur)==-1) {
			return false;
		}
		OVERLAPPED	ol;
		bytestring::zero((void *)&ol,sizeof(ol));
		ol.Offset=lockstart.LowPart;
		ol.OffsetHigh=lockstart.HighPart;
		LARGE_INTEGER	locklength;
		locklength.QuadPart=(len)?len:MAXDWORD;
		DWORD	flags=
			((type&F_WRLCK)?LOCKFILE_EXCLUSIVE_LOCK:0)|
			((method==F_SETLK)?LOCKFILE_FAIL_IMMEDIATELY:0);
		return LockFileEx((HANDLE)getHandleFromFileDescriptor(fd()),
					flags,
					0,
					locklength.LowPart,
					locklength.HighPart,
					&ol)!=FALSE;
	#else
		#error no fcntl, LockFile or anything like it
	#endif
}

bool file::checkLock(int16_t type, int16_t whence,
			off64_t start, off64_t len,
			int16_t *conftype, int16_t *confwhence,
			off64_t *confstart, off64_t *conflen) const {
	#if defined(RUDIMENTS_HAVE_FCNTL)
		struct flock	lck;
		lck.l_type=type;
		lck.l_whence=whence;
		lck.l_start=start;
		lck.l_len=len;
		int32_t	result=fCntl(F_GETLK,reinterpret_cast<long>(&lck));
		*conftype=lck.l_type;
		*confwhence=lck.l_whence;
		*confstart=lck.l_start;
		*conflen=lck.l_len;
		return !result;
	#elif defined(RUDIMENTS_HAVE_LOCKFILEEX)
		// Windows doesn't appear to support this at all.
		// I guess we'll return false, meaning not locked.
		return false;
	#else
		#error no fcntl(F_GETLK), LockFile or anything like it
	#endif
}

bool file::unlock(int16_t whence, off64_t start, off64_t len) const {
	#if defined(RUDIMENTS_HAVE_FCNTL)
		struct flock	lck;
		lck.l_type=F_UNLCK;
		lck.l_whence=whence;
		lck.l_start=start;
		lck.l_len=len;
		return !fCntl(F_SETLK,reinterpret_cast<long>(&lck));
	#elif defined(RUDIMENTS_HAVE_LOCKFILEEX)
		off64_t	cur=getCurrentPosition();
		if (cur==-1) {
			return false;
		}
		LARGE_INTEGER	lockstart;
		lockstart.QuadPart=lseek(start,whence);
		if (lockstart.QuadPart==-1) {
			return false;
		}
		if (setPositionRelativeToBeginning(cur)==-1) {
			return false;
		}
		LARGE_INTEGER	locklength;
		locklength.QuadPart=(len)?len:MAXDWORD;
		return UnlockFile((HANDLE)getHandleFromFileDescriptor(fd()),
					lockstart.LowPart,
					lockstart.HighPart,
					locklength.LowPart,
					locklength.HighPart)!=FALSE;
	#else
		#error no fcntl, UnlockFile or anything like it
	#endif
}

bool file::changeOwner(const char *newuser, const char *newgroup) const {
	return changeOwner(userentry::getUserId(newuser),
				groupentry::getGroupId(newgroup));
}

bool file::changeOwner(uid_t uid, gid_t gid) const {
	#if defined(RUDIMENTS_HAVE_FCHOWN)

		int32_t	result;
		error::clearError();
		do {
			result=fchown(fd(),uid,gid);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;

	#elif defined(RUDIMENTS_HAVE_SETSECURITYINFO)

		#if _WIN32_WINNT>=0x0500

		// get the file handle
		HANDLE	fh=(HANDLE)getHandleFromFileDescriptor(fd());
		if (fh==INVALID_HANDLE_VALUE) {
			return false;
		}

		// get the user and group sid's
		userentry	ue;
		groupentry	ge;
		if (!ue.initialize(uid) || !ge.initialize(gid)) {
			return false;
		}
		// FIXME: do these need to be freed?
		PSID	usid=(PSID)bytestring::duplicate(ue.getSid(),
							ue.getSidSize());
		PSID	gsid=(PSID)bytestring::duplicate(ge.getSid(),
							ge.getSidSize());

		// build trustees
		TRUSTEE ut;
		bytestring::zero(&ut,sizeof(ut));
		BuildTrusteeWithSid(&ut,usid);
		TRUSTEE gt;
		bytestring::zero(&gt,sizeof(gt));
		BuildTrusteeWithSid(&gt,gsid);

		// adjust my privileges so I can set the owner
		HANDLE	th=NULL;
		if (!OpenProcessToken(GetCurrentProcess(),
					TOKEN_ADJUST_PRIVILEGES,&th)) {
			return false;
		}
		TOKEN_PRIVILEGES	priv;
		priv.PrivilegeCount=1;
		priv.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		const char *privs[5]={
			SE_BACKUP_NAME,
			SE_RESTORE_NAME,
			SE_SECURITY_NAME,
			SE_TAKE_OWNERSHIP_NAME,
			NULL
		};
		for (const char * const *p=privs; *p; p++) {
			LUID	luid;
			if (!LookupPrivilegeValue(NULL,*p,&luid)) {
				CloseHandle(th);
				return false;
			}
			priv.Privileges[0].Luid=luid;
			if (!AdjustTokenPrivileges(th,FALSE,&priv,
					sizeof(TOKEN_PRIVILEGES),NULL,NULL)) {
				CloseHandle(th);
				return false;
			}
		}

		// build security descriptor
		DWORD	sdsize=0;
		SECURITY_INFORMATION	sinfo=
			OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION;
		GetUserObjectSecurity(fh,&sinfo,NULL,0,&sdsize);
		PSECURITY_DESCRIPTOR	sdesc=
			(PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,sdsize);
		if (!sdesc) {
			CloseHandle(th);
			return false;
		}
		bytestring::zero(sdesc,sdsize);
		if (!GetUserObjectSecurity(fh,&sinfo,sdesc,sdsize,&sdsize)) {
			LocalFree(sdesc);
			CloseHandle(th);
			return false;
		}
		PSECURITY_DESCRIPTOR	newsdesc=NULL;
		DWORD	result=BuildSecurityDescriptor(
						&ut,&gt,0,NULL,0,NULL,
						sdesc,&sdsize,&newsdesc);
		if (result!=ERROR_SUCCESS) {
			LocalFree(sdesc);
			LocalFree(newsdesc);
			CloseHandle(th);
			return false;
		}

		// set permissions
		if (!SetUserObjectSecurity(fh,&sinfo,newsdesc)) {
			LocalFree(sdesc);
			LocalFree(newsdesc);
			CloseHandle(th);
			return false;
		}
		
		LocalFree(sdesc);
		LocalFree(newsdesc);
		CloseHandle(th);
		return true;

		#else

		// FIXME: implement for WinNT
		return false;

		#endif
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool file::changeOwner(const char *filename, const char *newuser,
						const char *newgroup) {
	file	fl;
	return (fl.open(filename,O_RDWR) && fl.changeOwner(newuser,newgroup));
}

bool file::changeOwner(const char *filename, uid_t uid, gid_t gid) {
	file	fl;
	return (fl.open(filename,O_RDWR) && fl.changeOwner(uid,gid));
}

bool file::canChangeOwner(const char *filename) {
	return !pathConf(filename,_PC_CHOWN_RESTRICTED);
}

bool file::canChangeOwner() const {
	return !fpathConf(_PC_CHOWN_RESTRICTED);
}

bool file::rename(const char *oldpath, const char *newpath) {
	int32_t	result;
	error::clearError();
	do {
		result=::rename(oldpath,newpath);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool file::remove(const char *filename) {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__UNLINK)
			result=_unlink(filename);
		#elif defined(RUDIMENTS_HAVE_UNLINK)
			result=unlink(filename);
		#else
			#error no unlink or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool file::createHardLink(const char *oldpath, const char *newpath) {
	#if defined(RUDIMENTS_HAVE_LINK)
		int32_t	result;
		error::clearError();
		do {
			result=link(oldpath,newpath);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_LOADLIBRARY)
		bool	retval=false;
		HMODULE	lib=LoadLibrary("KERNEL32");
		if (lib) {
			bool	(*proc)(LPCSTR,LPCSTR,
					LPSECURITY_ATTRIBUTES)=
				(bool (*)(LPCSTR,LPCSTR,
					LPSECURITY_ATTRIBUTES))
					GetProcAddress(lib,"CreateHardLinkA");
			if (proc) {
				retval=proc(newpath,oldpath,NULL);
			}
			FreeLibrary(lib);
		}
		return retval;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool file::createSymbolicLink(const char *oldpath, const char *newpath) {
	#if defined(RUDIMENTS_HAVE_SYMLINK)
		int32_t	result;
		error::clearError();
		do {
			result=symlink(oldpath,newpath);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

char *file::resolveSymbolicLink(const char *filename) {

	#if defined(RUDIMENTS_HAVE_READLINK)
		size_t	buffersize=1024;
		for (;;) {

			// create a buffer to store the path
			char	*buffer=new char[buffersize];

			// read the path into the buffer
			int32_t	len;
			error::clearError();
			do {
				len=::readlink(filename,buffer,buffersize);
			} while (len==-1 && error::getErrorNumber()==EINTR);

			if (len==-1) {

				// if the call to readlink failed, delete the
				// buffer and return NULL
				delete[] buffer;
				return NULL;

			} else if ((size_t)len==buffersize) {

				// if the length of the path was the same as
				// the buffer size the we didn't get the entire
				// path, increase the size of the buffer and
				// try again
				delete[] buffer;
				buffersize=buffersize+1024;

				// if the buffer size exceeds 10k
				// then return failure
				if (buffersize>10240) {
					return NULL;
				}

			} else {
				// NULL-terminate the buffer, readlink()
				// doesn't do this for us
				buffer[len]='\0';
				return buffer;
			}
		}
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

bool file::sync() const {
	#if defined(RUDIMENTS_HAVE_FSYNC)
		int32_t	result;
		error::clearError();
		do {
			result=fsync(fd());
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_FLUSHFILEBUFFERS)
		return (FlushFileBuffers(
			(HANDLE)getHandleFromFileDescriptor(fd()))!=0);
	#elif defined(RUDIMENTS_HAVE_COMMIT)
		return _commit(fd())==0;
	#else
		#error no fsync or anything like it
	#endif
}

bool file::dataSync() const {
	#ifdef RUDIMENTS_HAVE_FDATASYNC
		int32_t	result;
		error::clearError();
		do {
			result=fdatasync(fd());
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		return sync();
	#endif
}

bool file::setLastAccessTime(const char *filename,
					time_t lastaccesstime) {
	struct stat	st;
	if (!stat(filename,&st)) {
		return false;
	}
	return setLastAccessAndModificationTimes(filename,
						lastaccesstime,
						st.st_mtime);
}

bool file::setLastModificationTime(const char *filename,
					time_t lastmodtime) {
	struct stat	st;
	if (!stat(filename,&st)) {
		return false;
	}
	return setLastAccessAndModificationTimes(filename,
						st.st_atime,
						lastmodtime);
}

bool file::setLastAccessAndModificationTimes(const char *filename,
						time_t lastaccesstime,
						time_t lastmodtime) {
	#if defined(RUDIMENTS_HAVE_UTIMES_CONST_CHAR) || \
			defined(RUDIMENTS_HAVE_UTIMES_CHAR)
		timeval	tv[2];
		tv[0].tv_sec=static_cast<long>(lastaccesstime);
		tv[0].tv_usec=0;
		tv[1].tv_sec=static_cast<long>(lastmodtime);
		tv[1].tv_usec=0;
		int32_t	result;
		error::clearError();
		do {
			#if defined(RUDIMENTS_HAVE_UTIMES_CONST_CHAR)
				result=utimes(filename,tv);
			#elif defined(RUDIMENTS_HAVE_UTIMES_CHAR)
				result=utimes((char *)filename,tv);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_UTIME)
		utimbuf	tb;
		tb.actime=static_cast<time_t>(lastaccesstime);
		tb.modtime=static_cast<time_t>(lastmodtime);
		int32_t	result;
		error::clearError();
		do {
			result=utime(filename,&tb);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_SETFILETIME)

		// open the file
		HANDLE	handle=CreateFile(filename,
						FILE_WRITE_ATTRIBUTES,
						FILE_SHARE_DELETE|
						FILE_SHARE_READ|
						FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL|
						FILE_FLAG_BACKUP_SEMANTICS,
						NULL);
		if (handle==INVALID_HANDLE_VALUE) {
			return false;
		}

		// windows time starts at 1/1/1601
		// unix time starts at 1/1/1970
		// this is the number of seconds between those dates
		#define	timediff	11644473600

		// convert the times from unix to windows format
		ULARGE_INTEGER	lastaccessfiletime;
		lastaccessfiletime.QuadPart=(lastaccesstime+timediff)*10000000;
		ULARGE_INTEGER	lastmodfiletime;
		lastmodfiletime.QuadPart=(lastmodtime+timediff)*10000000;

		// set the file times
		bool	retval=SetFileTime(handle,NULL,
						(FILETIME *)&lastaccesstime,
						(FILETIME *)&lastmodtime)!=0;

		// close the file
		CloseHandle(handle);

		// return the result
		return retval;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool file::setLastAccessAndModificationTimes(const char *filename) {
	#if defined(RUDIMENTS_HAVE_UTIMES_CONST_CHAR) || \
			defined(RUDIMENTS_HAVE_UTIMES_CHAR)
		int32_t	result;
		error::clearError();
		do {
			#if defined(RUDIMENTS_HAVE_UTIMES_CONST_CHAR)
				result=utimes(filename,NULL);
			#elif defined(RUDIMENTS_HAVE_UTIMES_CHAR)
				result=utimes((char *)filename,NULL);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		datetime	dt;
		dt.getSystemDateAndTime();
		return setLastAccessAndModificationTimes(filename,
							dt.getEpoch(),
							dt.getEpoch());
	#endif
}

bool file::createFifo(const char *filename, mode_t perms) {
	#if defined(RUDIMENTS_HAVE_MKFIFO)
		int32_t	result;
		error::clearError();
		do {
			result=mkfifo(filename,perms);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATENAMEDPIPE)
		HANDLE	handle=CreateNamedPipe(filename,0,0,1,
						1024,1024,5000,NULL);
		if (handle==INVALID_HANDLE_VALUE) {
			return false;
		}
		CloseHandle(handle);
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool file::createPipe(filedescriptor *readfd, filedescriptor *writefd) {
	int32_t	result;
	int	fd[2];
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_PIPE)
			result=pipe(fd);
		#elif defined(RUDIMENTS_HAVE__PIPE)
			result=_pipe(fd,1024,0);
		#else
			#error no pipe or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (!result) {
		if (readfd) {
			readfd->setFileDescriptor(fd[0]);
		}
		if (writefd) {
			writefd->setFileDescriptor(fd[1]);
		}
	}
	return !result;
}

int32_t file::createTemporaryFile(char *templatefilename) {
	#if defined(RUDIMENTS_HAVE_MKSTEMP)
		int32_t	result;
		error::clearError();
		do {
			result=mkstemp(templatefilename);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		// sanity check on templatefilename
		char	*lastsix=templatefilename+
				charstring::length(templatefilename)-6;
		if (charstring::compare(lastsix,"XXXXXX")) {
			error::setErrorNumber(EINVAL);
			return -1;
		}

		// replace X's with random characters...

		// seed the random number
		uint32_t	seed=randomnumber::getSeed();

		// for each of the 6 characters...
		for (uint8_t i=0; i<6; i++) {

			// get a random number, scale it to 0-60
			seed=randomnumber::generateNumber(seed);
			char	ch=(char)randomnumber::scaleNumber(seed,0,59);

			// translate...
			//  0-9  -> '0' - '9'
			// 10-34 -> 'A' - 'Z'
			// 35-59 -> 'a' - 'z'
			if (ch<10) {
				ch=ch+'0';
			} else if (ch<35) {
				ch=ch-10+'A';
			} else {
				ch=ch-35+'a';
			}

			// set character
			lastsix[i]=ch;
		}

		// create the file
		file	f;
		if (!f.create(templatefilename,
				permissions::evalPermString("rw-r--r--"))) {
			return -1;
		}

		// fake it out so the file won't get
		// closed when f is deallocated
		int32_t	fdesc=f.getFileDescriptor();
		f.setFileDescriptor(-1);

		// return the file descriptor
		return fdesc;
	#endif
}

void *file::getInternalFileStatisticsStructure() {
	return (void *)&(pvt->_st);
}

char *file::dirname(const char *filename) {

	if (!filename) {
		return NULL;
	} else if (!charstring::contains(filename,'/') ||
			!charstring::compare(filename,".")) {
		return charstring::duplicate(".");
	} else if (!charstring::compare(filename,"..")) {
		return charstring::duplicate("..");
	} else if (!charstring::compare(filename,"/")) {
		return charstring::duplicate("/");
	}

	char	*retval=charstring::duplicate(filename);
	charstring::rightTrim(retval,'/');
	char	*lastslash=charstring::findLast(retval,'/');
	if (lastslash==retval) {
		(*(lastslash+1))='\0';
	} else {
		(*lastslash)='\0';
	}
	return retval;
}

char *file::basename(const char *filename) {

	if (!filename) {
		return NULL;
	}

	char	*copy=charstring::duplicate(filename);
	charstring::rightTrim(copy,'/');

	char	*retval;
	char	*ptr=charstring::findLast(copy,'/');
	if (!ptr) {
		retval=charstring::duplicate(copy);
	} else {
		retval=charstring::duplicate(ptr+1);
	}
	delete[] copy;
	return retval;
}

char *file::basename(const char *filename, const char *suffix) {
	char	*retval=basename(filename);
	char	*ptr=charstring::findLast(retval,suffix);
	if (!(*(ptr+charstring::length(suffix)))) {
		(*ptr)='\0';
	}
	return retval;
}

char *file::eightDotThree(const char *filename) {

	// FIXME: I'm sure there's an infinitely more efficient way of
	// doing this using a static buffer...

	// get the base name
	char	*base=basename(filename);

	// get the directory name unless no directory was specified
	char	*dir=NULL;
	if (charstring::compare(base,filename)) {
		dir=dirname(filename);
	}

	// get the suffix and terminate the base at the final dot
	char	*suffix=NULL;
	char	*finaldot=charstring::findLast(base,'.');
	if (finaldot) {
		suffix=charstring::duplicate(finaldot+1);
		*finaldot='\0';
	}

	// Strip illegal characters from the base.  If we had to strip any
	// characters from the base then we need to force the loop below to
	// put a ~# at the end.
	bool	tryunique=charstring::stripSet(base," .\"/\\[]:;=,");

	// strip illegal characters from the suffix
	charstring::stripSet(suffix," .\"/\\[]:;=,");

	// truncate the base at 8 characters if necessary
	if (charstring::length(base)>8) {
		*(base+8)='\0';
	}

	// truncate the suffix at 3 characters if necessary
	if (charstring::length(suffix)>3) {
		*(suffix+3)='\0';
	}

	// convert the base and suffix to upper case
	charstring::upper(base);
	charstring::upper(suffix);

	// verify that the filename is unique
	uint8_t		counter=1;
	stringbuffer	fullname;
	char		newbase[9];
	charstring::copy(newbase,base);
	for (;;) {

		// try to come up with a unique name
		if (tryunique) {
			size_t	baselen=charstring::length(base);
			if (baselen>6) {
				baselen=6;
			}
			charstring::copy(newbase,base,baselen);
			newbase[baselen]='~';
			newbase[baselen+1]='0'+counter;
			fullname.clear();
			counter++;
		}

		// build the test file name
		if (dir) {
			fullname.append(dir)->append('/');
		}
		fullname.append(newbase);
		if (suffix) {
			fullname.append('.')->append(suffix);
		}

		// if the name we've come up with is the same as the original
		// filename or if a file with the name we've come up with
		// doesn't exist, then go ahead and use the name
		if (!charstring::compare(fullname.getString(),filename) ||
						!exists(fullname.getString())) {
			break;
		}

		// if the counter hits 10 digits then it must not be possible
		// to uniquely name this file
		if (counter==10) {
			delete[] base;
			delete[] dir;
			delete[] suffix;
			return NULL;
		}

		// try for a unique name the next round
		tryunique=true;
	}

	// get the final name
	char	*retval=fullname.detachString();

	// clean up
	delete[] dir;
	delete[] base;
	delete[] suffix;

	// return the name
	return retval;
}

key_t file::generateKey(const char *filename, int32_t id) {
#if defined(RUDIMENTS_HAVE_FTOK)
	#ifdef RUDIMENTS_HAVE_CONST_CHAR_FTOK
		return ::ftok(filename,id);
	#else
		return ::ftok(const_cast<char *>(filename),id);
	#endif
#elif !defined( __VMS)
	file	f;
	if (!f.open(filename,O_RDONLY)) {
		return -1;
	}
	return (key_t)((f.getInode() & 0xffff) |
			((f.getDevice() & 0xff) << 16) |
			((id & 0xff)  << 24));
#else
	RUDIMENTS_SET_ENOSYS
	return -1;
#endif
}

int64_t file::maxLinks(const char *filename) {
	return pathConf(filename,_PC_LINK_MAX);
}

int64_t file::maxLinks() const {
	return fpathConf(_PC_LINK_MAX);
}

bool file::posixFadvise(off64_t offset, off64_t len, int32_t advice) const {
	#ifdef RUDIMENTS_HAVE_POSIX_FADVISE
		int32_t	result;
		error::clearError();
		do {
			result=posix_fadvise(fd(),offset,len,advice);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		// on platforms that don't support this, it's ok to just return
		// success since really it only gives the filesystem hints
		return true;
	#endif
}

int64_t file::pathConf(const char *path, int32_t name) {
	#if defined(RUDIMENTS_HAVE_PATHCONF)
		int64_t	result;
		error::clearError();
		do {
			result=pathconf(path,name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t file::fpathConf(int32_t name) const {
	#if defined(RUDIMENTS_HAVE_FPATHCONF)
		int64_t	result;
		error::clearError();
		do {
			result=fpathconf(fd(),name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}
