// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/file.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>
#include <rudiments/stringbuffer.h>

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
#ifdef RUDIMENTS_HAVE_XATTRS
	#include <sys/xattr.h>
#endif
#ifdef RUDIMENTS_HAVE_UTIME_H
	#include <utime.h>
#endif
#ifdef RUDIMENTS_HAVE_IO_H
	#undef _POSIX_
	#include <io.h>
#endif
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
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
		bool		_getcurrentpropertiesonopen;
};

file::file() : filedescriptor() {
	pvt=new fileprivate;
	rawbuffer::zero(&pvt->_st,sizeof(pvt->_st));
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
	pvt->_getcurrentpropertiesonopen=f.pvt->_getcurrentpropertiesonopen;
}

file::~file() {
	delete pvt;
}

bool file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

bool file::createFile(const char *name, mode_t perms) {
	file	fl;
	return fl.create(name,perms);
}

void file::openInternal(const char *name, int32_t flags) {
	int32_t	result;
	do {
		#if defined(RUDIMENTS_HAVE__OPEN)
			result=_open(name,flags);
		#elif defined(RUDIMENTS_HAVE_OPEN)
			result=::open(name,flags);
		#else
			#error no open or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	fd(result);
}

void file::openInternal(const char *name, int32_t flags, mode_t perms) {

	#ifdef RUDIMENTS_HAVE_CREATEFILE

	// On Windows, when creating a file, in order to set permissions other
	// than just owner read/write, the CreateFile method must be used rather
	// than just plain _open.
	if (flags&O_CREAT) {

		// determine the access and share modes
		DWORD	accessmode=0;
		DWORD	sharemode=0;
		if (flags&O_WRONLY) {
			accessmode=GENERIC_WRITE;
			sharemode=FILE_SHARE_DELETE|FILE_SHARE_WRITE;
		}
		if (flags&O_RDONLY) {
			accessmode=GENERIC_READ;
			sharemode=FILE_SHARE_READ;
		}
		if (flags&O_RDWR) {
			accessmode=GENERIC_READ|GENERIC_WRITE;
			sharemode=FILE_SHARE_DELETE|
					FILE_SHARE_READ|FILE_SHARE_WRITE;
		}

		// determine the security attributes
		SECURITY_ATTRIBUTES	satt;
		satt.nLength=sizeof(LPSECURITY_ATTRIBUTES);
		satt.bInheritHandle=TRUE;

		char	*sddl=permissions::permOctalToSDDL(perms,false);

		if (ConvertStringSecurityDescriptorToSecurityDescriptor(
					sddl,SDDL_REVISION_1,
					&satt.lpSecurityDescriptor,
					NULL)==FALSE) {
			delete[] sddl;
			fd(-1);
			return;
		}

		// create the file
		HANDLE	fh=CreateFile(name,accessmode,sharemode,
					&satt,
					CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,
					NULL);
		if (fh==INVALID_HANDLE_VALUE) {
			delete[] sddl;
			fd(-1);
			return;
		}

		// clean up
		delete[] sddl;

		// get the file descriptor from the handle
		fd(_open_osfhandle((long)fh,flags&~(O_CREAT|O_TRUNC)));

		// truncate if necessary
		if (flags&O_TRUNC) {
			truncate();
		}
		return;
	}
	#endif

	int32_t	result;
	do {
		#if defined(RUDIMENTS_HAVE__OPEN)
			result=_open(name,flags,perms);
		#elif defined(RUDIMENTS_HAVE_OPEN)
			result=::open(name,flags,perms);
		#else
			#error no open or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	fd(result);
}

bool file::open(const char *name, int32_t flags) {
	openInternal(name,flags);
	return (fd()!=-1 &&
		((pvt->_getcurrentpropertiesonopen)?
				getCurrentProperties():true));
}

bool file::open(const char *name, int32_t flags, mode_t perms) {
	openInternal(name,flags,perms);
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
	do {
		result=fstat(fd(),&pvt->_st);
	} while (result==-1 && error::getErrorNumber()==EINTR);

	#if defined(RUDIMENTS_HAVE_GETSECURITYINFO)

		if (result) {
			return false;
		}

		// On Windows, the st_mode isn't set correctly.  Get the DACL
		// of the file and convert it to a mode_t using

		// get the security information
		PACL			dacl=NULL;
		PSECURITY_DESCRIPTOR	ppsd=NULL;
		if (GetSecurityInfo((HANDLE)_get_osfhandle(fd()),
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

		return true;
	#endif
	return !result;
}

bool file::stat(const char *filename, void *st) {
	int32_t	result;
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
	return -1;
#endif
}

blkcnt_t file::getBlockCount() const {
#ifdef RUDIMENTS_HAVE_BLKCNT_T
	return pvt->_st.st_blocks;
#else
	return -1;
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
	#if defined(_S_IFREG)
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
	#if defined(_S_IFCHR)
		return ((pvt->_st.st_mode&_S_IFCHR)==_S_IFCHR);
	#elif defined(S_IFCHR)
		return ((pvt->_st.st_mode&S_IFCHR)==S_IFCHR);
	#else
		return S_ISCHR(pvt->_st.st_mode);
	#endif
}

int32_t file::isFifo() const {
	#if defined(_S_IFIFO)
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

ino_t file::getInode() const {
	return pvt->_st.st_ino;
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
		// how do I specify read vs. write/write?
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
		rawbuffer::zero((void *)&ol,sizeof(ol));
		ol.Offset=lockstart.LowPart;
		ol.OffsetHigh=lockstart.HighPart;
		LARGE_INTEGER	locklength;
		locklength.QuadPart=len;
		return LockFileEx((HANDLE)_get_osfhandle(fd()),
					(method==F_SETLK)?
						LOCKFILE_FAIL_IMMEDIATELY:0,
					0,
					locklength.LowPart,
					locklength.HighPart,
					&ol)!=0;
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
		locklength.QuadPart=len;
		return UnlockFile((HANDLE)_get_osfhandle(fd()),
					lockstart.LowPart,
					lockstart.HighPart,
					locklength.LowPart,
					locklength.HighPart)!=0;
	#else
		#error no fcntl, UnlockFile or anything like it
	#endif
}

bool file::changeOwner(const char *newuser, const char *newgroup) const {
	return changeOwner(passwdentry::getUserId(newuser),
				groupentry::getGroupId(newgroup));
}

bool file::changeOwner(uid_t uid, gid_t gid) const {
	#if defined(RUDIMENTS_HAVE_FCHOWN)
		int32_t	result;
		do {
			result=fchown(fd(),uid,gid);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		error::setErrorNumber(ENOSYS);
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
	do {
		result=::rename(oldpath,newpath);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool file::remove(const char *filename) {
	int32_t	result;
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
		#error no link or anything like it
	#endif
}

bool file::createSymbolicLink(const char *oldpath, const char *newpath) {
	#if defined(RUDIMENTS_HAVE_SYMLINK)
		int32_t	result;
		do {
			result=symlink(oldpath,newpath);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		error::setErrorNumber(ENOSYS);
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
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

bool file::sync() const {
	#if defined(RUDIMENTS_HAVE_FSYNC)
		int32_t	result;
		do {
			result=fsync(fd());
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_COMMIT)
		return _commit(fd())==0;
	#else
		#error no fsync or anything like it
	#endif
}

bool file::dataSync() const {
	#ifdef RUDIMENTS_HAVE_FDATASYNC
		int32_t	result;
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
		#error no utimes or anything like it
	#endif
}

bool file::setLastAccessAndModificationTimes(const char *filename) {
	#if defined(RUDIMENTS_HAVE_UTIMES_CONST_CHAR) || \
			defined(RUDIMENTS_HAVE_UTIMES_CHAR)
		int32_t	result;
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
		#error no mkfifo or anything like it
	#endif
}

bool file::createPipe(filedescriptor *readfd, filedescriptor *writefd) {
	int32_t	result;
	int	fd[2];
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
		int32_t	seed=randomnumber::getSeed();

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

const char * const *file::listAttributes() const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return NULL;
	#else

	// The flistxattr interface is designed such that you have to guess the
	// size of the buffer that it will need, call flistxattr, then see if
	// your guess was correct.  If not, you have to try again.

	// try with a 8 byte buffer to start
	size_t	size=8;
	char	*buffer;

	// try 100 times...
	for (int32_t i=0; i<100; i++) {

		// allocate a buffer
		buffer=new char[size];

		// attempt to read the attribute into the buffer
		ssize_t	newsize;
		do {
			newsize=flistxattr(fd(),buffer,size);
		} while (newsize==-1 && error::getErrorNumber()==EINTR);

		// it's possible that someone changed the attribute between
		// the previous 2 calls to fgetxattr and increased the size
		// of the buffer necessary to retrieve it, if so, try again
		// with a bigger buffer.
		if (newsize==-1) {
			delete[] buffer;
			return NULL;
		} else if (newsize>(ssize_t)size) {
			delete[] buffer;
			size=newsize;
		} else {
			const char * const	*retval=
						attributeArray(buffer,size);
			delete[] buffer;
			return retval;
		}
	}

	// if we couldn't get the attribute after 100 tries, give up
	return NULL;
	#endif
}

bool file::getAttribute(const char *name, uint16_t *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(uint16_t));
}

bool file::getAttribute(const char *name, uint32_t *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(uint32_t));
}

bool file::getAttribute(const char *name, uint64_t *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(uint64_t));
}

bool file::getAttribute(const char *name, int16_t *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(int16_t));
}

bool file::getAttribute(const char *name, int32_t *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(int32_t));
}

bool file::getAttribute(const char *name, int64_t *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(int64_t));
}

bool file::getAttribute(const char *name, float *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(float));
}

bool file::getAttribute(const char *name, double *number) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&number),
			sizeof(double));
}

bool file::getAttribute(const char *name, unsigned char *character) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&character),
			sizeof(unsigned char));
}

bool file::getAttribute(const char *name, char *character) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&character),
			sizeof(char));
}

bool file::getAttribute(const char *name, bool *value) const {
	return getAttribute(name,
			reinterpret_cast<void **>(&value),
			sizeof(bool));
}

bool file::getAttribute(const char *name, unsigned char **string) const {
	size_t	size;
	return getAttribute(name,reinterpret_cast<void **>(string),&size);
}

bool file::getAttribute(const char *name, char **string) const {
	size_t	size;
	return getAttribute(name,reinterpret_cast<void **>(string),&size);
}

bool file::getAttribute(const char *name,
				unsigned char **string, size_t *size) const {
	return getAttribute(name,reinterpret_cast<void **>(string),size);
}

bool file::getAttribute(const char *name, char **string, size_t *size) const {
	return getAttribute(name,reinterpret_cast<void **>(string),size);
}

bool file::getAttribute(const char *name, void **buffer, size_t *size) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	*buffer=NULL;
	*size=0;
	return false;
	#else

	// The fgetxattr interface is designed such that you have to guess the
	// size of the buffer that it will need, call fgetxattr, then see if
	// your guess was correct.  If not, you have to try again.

	// try with a 8 byte buffer to start
	(*size)=8;

	// try 100 times...
	for (int32_t i=0; i<100; i++) {

		// allocate a buffer
		(*buffer)=static_cast<void *>(new unsigned char[(*size)]);

		// attempt to read the attribute into the buffer
		ssize_t	newsize;
		do {
			newsize=fgetxattr(fd(),name,(*buffer),(*size));
		} while (newsize==-1 && error::getErrorNumber()==EINTR);

		// it's possible that someone changed the attribute between
		// the previous 2 calls to fgetxattr and increased the size
		// of the buffer necessary to retrieve it, if so, try again
		// with a bigger buffer.
		if (newsize==-1) {
			delete[] static_cast<unsigned char *>(*buffer);
			return false;
		} else if (newsize>(ssize_t)(*size)) {
			delete[] static_cast<unsigned char *>(*buffer);
			(*size)=newsize;
		} else {
			return true;
		}
	}

	// if we couldn't get the attribute after 100 tries, give up
	return false;

	#endif
}

bool file::getAttribute(const char *name, void *buffer, size_t size) const {
	size_t		s;
	unsigned char	*buf;
	bool	retval=getAttribute(name,reinterpret_cast<void **>(&buf),&s);
	if (retval) {
		rawbuffer::copy(buf,buffer,(s<size)?s:size);
		delete[] buf;
	}
	return retval;
}

bool file::createAttribute(const char *name, uint16_t number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, uint32_t number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, uint64_t number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, int16_t number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, int32_t number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, int64_t number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, float number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, double number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, unsigned char character) const {
	return createAttribute(name,&character,sizeof(character));
}

bool file::createAttribute(const char *name, char character) const {
	return createAttribute(name,&character,sizeof(character));
}

bool file::createAttribute(const char *name, bool value) const {
	return createAttribute(name,&value,sizeof(value));
}

bool file::createAttribute(const char *name,
				const unsigned char *string) const {
	return createAttribute(name,string,charstring::length(string));
}

bool file::createAttribute(const char *name, const char *string) const {
	return createAttribute(name,string,charstring::length(string));
}

bool file::createAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return createAttribute(name,static_cast<const void *>(string),size);
}

bool file::createAttribute(const char *name, const char *string,
							size_t size) const {
	return createAttribute(name,static_cast<const void *>(string),size);
}

#ifndef XATTR_CREATE
#define XATTR_CREATE 0
#endif
bool file::createAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,XATTR_CREATE);
}

bool file::replaceAttribute(const char *name, uint16_t number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, uint32_t number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, uint64_t number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, int16_t number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, int32_t number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, int64_t number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, float number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, double number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, unsigned char character) const {
	return replaceAttribute(name,&character,sizeof(character));
}

bool file::replaceAttribute(const char *name, char character) const {
	return replaceAttribute(name,&character,sizeof(character));
}

bool file::replaceAttribute(const char *name, bool value) const {
	return replaceAttribute(name,&value,sizeof(value));
}

bool file::replaceAttribute(const char *name,
				const unsigned char *string) const {
	return replaceAttribute(name,string,charstring::length(string));
}

bool file::replaceAttribute(const char *name, const char *string) const {
	return replaceAttribute(name,string,charstring::length(string));
}

bool file::replaceAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return replaceAttribute(name,static_cast<const void *>(string),size);
}

bool file::replaceAttribute(const char *name, const char *string,
							size_t size) const {
	return replaceAttribute(name,static_cast<const void *>(string),size);
}

#ifndef XATTR_REPLACE
#define XATTR_REPLACE 0
#endif
bool file::replaceAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,XATTR_REPLACE);
}

bool file::setAttribute(const char *name, uint16_t number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, uint32_t number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, uint64_t number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, int16_t number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, int32_t number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, int64_t number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, float number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, double number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, unsigned char character) const {
	return setAttribute(name,&character,sizeof(character));
}

bool file::setAttribute(const char *name, char character) const {
	return setAttribute(name,&character,sizeof(character));
}

bool file::setAttribute(const char *name, bool value) const {
	return setAttribute(name,&value,sizeof(value));
}

bool file::setAttribute(const char *name, const unsigned char *string) const {
	return setAttribute(name,string,charstring::length(string));
}

bool file::setAttribute(const char *name, const char *string) const {
	return setAttribute(name,string,charstring::length(string));
}

bool file::setAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return setAttribute(name,static_cast<const void *>(string),size);
}

bool file::setAttribute(const char *name, const char *string,
							size_t size) const {
	return setAttribute(name,static_cast<const void *>(string),size);
}

bool file::setAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,0);
}

bool file::setAttribute(const char *name, const void *value,
					size_t size, int32_t flags) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return false;
	#else

	ssize_t	result;
	do {
		result=fsetxattr(fd(),name,
				#ifndef RUDIMENTS_HAVE_XFSETXATTR_CONST
				(void *)
				#endif
				value,size,flags);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;

	#endif
}

bool file::removeAttribute(const char *name) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return false;
	#else

	ssize_t	result;
	do {
		result=fremovexattr(fd(),name);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;

	#endif
}

const char * const *file::attributeArray(const char *buffer,
						size_t size) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return NULL;
	#else


	// count the number of attributes
	int32_t	counter=0;
	for (size_t index=0; index<size; index++) {
		if (!buffer[index]) {
			counter++;
		}
	}

	// create an array to hold them
	char	**attributes=new char *[counter+1];
	attributes[counter]=NULL;

	// copy the attributes into the array
	const char	*ptr=buffer;
	counter=0;
	for (size_t index=0; index<size; index++) {
		if (!buffer[index]) {
			attributes[counter]=charstring::duplicate(ptr);
			if (index<size-1) {
				ptr=&buffer[index+1];
			}
			counter++;
		}
	}

	// return the array
	return attributes;

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

key_t file::generateKey(const char *filename, int32_t id) {
#ifdef RUDIMENTS_HAVE_FTOK
	#ifdef RUDIMENTS_HAVE_CONST_CHAR_FTOK
		return ::ftok(filename,id);
	#else
		return ::ftok(const_cast<char *>(filename),id);
	#endif
#else
	file	f;
	if (!f.open(filename,O_RDONLY)) {
		return -1;
	}
	return (key_t)((f.getInode() & 0xffff) |
			((f.getDevice() & 0xff) << 16) |
			((id & 0xff)  << 24));
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
		do {
			result=pathconf(path,name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

int64_t file::fpathConf(int32_t name) const {
	#if defined(RUDIMENTS_HAVE_FPATHCONF)
		int64_t	result;
		do {
			result=fpathconf(fd(),name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}
