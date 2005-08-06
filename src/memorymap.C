// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/memorymap.h>
#include <rudiments/error.h>

// for getpagesize()...
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef HAVE_WINDOWS_H
	#include <windows.h>
#endif

#ifdef HAVE_MMAP_CADDR_T
	#define ADDRCAST caddr_t
#else
	#define ADDRCAST void *
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

memorymap::memorymap() {
	data=NULL;
	length=0;
}

memorymap::~memorymap() {
	detach();
}

bool memorymap::attach(int fd, off64_t offset, size_t len,
					int protection, int flags) {
	length=len;
	#if defined(HAVE_MMAP)
		do {
			data=mmap(NULL,len,protection,flags,fd,offset);
		} while (data==MAP_FAILED && error::getErrorNumber()==EINTR);
		return (data!=MAP_FAILED);
	#elif defined(HAVE_CREATE_FILE_MAPPING)
		DWORD	mapprot=(protection|PROT_WRITE)?
					PAGE_READONLY:PAGE_READWRITE;
		map=CreateFileMapping((HANDLE)_get_osfhandle(fd),
					NULL,mapprot,0,len,NULL);
		if (map) {
			DWORD	viewprot=(protection|PROT_WRITE)?
						FILE_MAP_READ:FILE_MAP_WRITE;
			data=MapViewOfFile(map,viewprot,0,0,len);
			if (!data) {
				CloseHandle(map);
				return false;
			}
			return true;
		}
		return false;
	#endif
}

bool memorymap::detach() {
	#ifdef HAVE_MMAP
		int	result;
		do {
			#ifdef HAVE_MMAP_CADDR_T
			result=munmap((caddr_t)data,length);
			#else
			result=munmap(data,length);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		bool	retval=!result;
	#elif defined(HAVE_CREATE_FILE_MAPPING)
		bool	retval=(UnmapViewOfFile(data) && CloseHandle(map));
	#endif
	data=NULL;
	length=0;
	return retval;
}

bool memorymap::setProtection(int protection) {
	return setProtection(0,length,protection);
}

bool memorymap::setProtection(off64_t offset, size_t len, int protection) {
	#ifdef HAVE_MPROTECT
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	int	result;
	do {
		result=mprotect(reinterpret_cast<ADDRCAST>(ptr),len,protection);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
	#else
	return false;
	#endif
}

void *memorymap::getData() {
	return data;
}

size_t memorymap::getLength() {
	return length;
}

bool memorymap::sync(bool immediate, bool invalidate) {
	return sync(0,length,immediate,invalidate);
}

bool memorymap::sync(off64_t offset, size_t len,
			bool immediate, bool invalidate) {
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	#ifdef HAVE_MSYNC
	int	result;
	do {
		result=msync(reinterpret_cast<ADDRCAST>(ptr),len,
				((immediate)?MS_SYNC:MS_ASYNC)|
					((invalidate)?MS_INVALIDATE:0));
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
	#else
	return FlushViewOfFile(reinterpret_cast<void *>(ptr),len);
	#endif
}

bool memorymap::sequentialAccess(off64_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	return mAdvise(ptr,len,MADV_SEQUENTIAL);
	#else
	return true;
	#endif
}

bool memorymap::randomAccess(off64_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	return mAdvise(ptr,len,MADV_RANDOM);
	#else
	return true;
	#endif
}

bool memorymap::willNeed(off64_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	return mAdvise(ptr,len,MADV_WILLNEED);
	#else
	return true;
	#endif
}

bool memorymap::wontNeed(off64_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	return mAdvise(ptr,len,MADV_DONTNEED);
	#else
	return true;
	#endif
}

bool memorymap::normalAccess(off64_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	return mAdvise(ptr,len,MADV_NORMAL);
	#else
	return true;
	#endif
}

#ifdef HAVE_MLOCK
bool memorymap::lock() {
	return lock(0,length);
}

bool memorymap::lock(off64_t offset, size_t len) {
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	int	result;
	do {
		result=mlock(reinterpret_cast<ADDRCAST>(ptr),len);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}
#endif

#ifdef HAVE_MUNLOCK
bool memorymap::unlock() {
	return unlock(0,length);
}

bool memorymap::unlock(off64_t offset, size_t len) {
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	int	result;
	do {
		result=munlock(reinterpret_cast<ADDRCAST>(ptr),len);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}
#endif

#ifdef HAVE_MINCORE
bool memorymap::inMemory() {
	return inMemory(0,length);
}

bool memorymap::inMemory(off64_t offset, size_t len) {

	// create an array of char's, 1 for each page
	int		pagesize=getpagesize();
	int		tmplen=(len+pagesize-1)/pagesize;
	#ifdef HAVE_MINCORE_CHAR
	char		*tmp=new char[tmplen];
	#else
	unsigned char	*tmp=new unsigned char[tmplen];
	#endif

	// call mincore to fill the array
	unsigned char	*ptr=(static_cast<unsigned char *>(data))+offset;
	int		result;
	do {
		result=mincore(reinterpret_cast<ADDRCAST>(ptr),len,tmp);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result) {
		delete[] tmp;
		return false;
	}

	// look through the array, if any of the
	// pages aren't in memory, return false
	for (int i=0; i<tmplen; i++) {
		if (tmp[i]) {
			delete[] tmp;
			return false;
		}
	}
	delete[] tmp;
	return true;
}
#endif

#ifdef HAVE_MLOCKALL
bool memorymap::lockAll() {
	return mLockAll(MCL_CURRENT|MCL_FUTURE);
}

bool memorymap::lockAllCurrent() {
	return mLockAll(MCL_CURRENT);
}

bool memorymap::lockAllFuture() {
	return mLockAll(MCL_FUTURE);
}
#endif

#ifdef HAVE_MUNLOCKALL
bool memorymap::unlockAll() {
	int	result;
	do {
		result=munlockall();
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}
#endif

bool memorymap::mAdvise(unsigned char *start, size_t length, int advice) {
	int	result;
	do {
		result=madvise(reinterpret_cast<ADDRCAST>(start),length,advice);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool memorymap::mLockAll(int flags) {
	int	result;
	do {
		result=mlockall(flags);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
