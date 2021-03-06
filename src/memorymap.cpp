// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/memorymap.h>
#include <rudiments/sys.h>
#include <rudiments/error.h>
#if defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
	#include <rudiments/filedescriptor.h>
#endif

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_SYS_MMAN_H
	#include <sys/mman.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif

#ifdef RUDIMENTS_HAVE_MUNMAP_CADDR_T
	#define MUNMAP_ADDRCAST caddr_t
#else
	#define MUNMAP_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MINCORE_CADDR_T
	#define MINCORE_ADDRCAST caddr_t
#else
	#define MINCORE_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MPROTECT_CADDR_T
	#define MPROTECT_ADDRCAST caddr_t
#else
	#define MPROTECT_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MSYNC_CADDR_T
	#define MSYNC_ADDRCAST caddr_t
#else
	#define MSYNC_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MLOCK_CADDR_T
	#define MLOCK_ADDRCAST caddr_t
#else
	#define MLOCK_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MUNLOCK_CADDR_T
	#define MUNLOCK_ADDRCAST caddr_t
#else
	#define MUNLOCK_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MADVISE_CADDR_T
	#define MADVISE_ADDRCAST caddr_t
#else
	#define MADVISE_ADDRCAST void *
#endif

#ifndef MAP_FAILED
	#define MAP_FAILED ((void *)-1)
#endif

class memorymapprivate {
	friend class memorymap;
	private:
		void	*_data;
		size_t	_length;
		#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
			HANDLE	_map;
		#endif
};

memorymap::memorymap() {
	pvt=new memorymapprivate;
	pvt->_data=NULL;
	pvt->_length=0;
	#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
		pvt->_map=NULL;
	#endif
}

memorymap::~memorymap() {
	detach();
	delete pvt;
}

bool memorymap::attach(int32_t fd, off64_t offset, size_t len,
					int32_t protection, int32_t flags) {
	pvt->_length=len;
	#if defined(RUDIMENTS_HAVE_MMAP)
		error::clearError();
		do {
			pvt->_data=mmap(NULL,len,protection,flags,fd,offset);
		} while (pvt->_data==(void *)MAP_FAILED &&
				error::getErrorNumber()==EINTR);
		return (pvt->_data!=(void *)MAP_FAILED);
	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)

		// calculate max mapping size and offset
		DWORD	offsethigh=(((uint64_t)offset)>>32);
		DWORD	offsetlow=(((uint64_t)offset)&0x00000000FFFFFFFF);

		// determine map and view protection
		DWORD	mapprot=0;
		DWORD	viewprot=0;
		DWORD	viewprotwrite=(flags&MAP_PRIVATE)?
					FILE_MAP_COPY:FILE_MAP_WRITE;
		if (protection&PROT_EXEC) {
			if (protection&PROT_READ) {
				if (protection&PROT_WRITE) {
					mapprot=PAGE_EXECUTE_READWRITE;
					viewprot=viewprotwrite|
						#ifdef FILE_MAP_EXECUTE
							FILE_MAP_EXECUTE;
						#else
							SECTION_MAP_EXECUTE;
						#endif
				} else {
					mapprot=PAGE_EXECUTE_READ;
					viewprot=FILE_MAP_READ|
						#ifdef FILE_MAP_EXECUTE
							FILE_MAP_EXECUTE;
						#else
							SECTION_MAP_EXECUTE;
						#endif
				}
			}
		} else if (protection&PROT_READ) {
			if (protection&PROT_WRITE) {
				mapprot=PAGE_READWRITE;
				viewprot=viewprotwrite;
			} else {
				mapprot=PAGE_READONLY;
				viewprot=FILE_MAP_READ;
			}
		}

		// create the file mapping
		pvt->_map=CreateFileMapping((HANDLE)filedescriptor::
						getHandleFromFileDescriptor(fd),
						NULL,mapprot,
						0,0,
						NULL);
		if (!pvt->_map) {
			return false;
		}

		// create a view of the file mapping
		pvt->_data=MapViewOfFile(pvt->_map,
						viewprot,
						offsethigh,offsetlow,
						len);
		if (!pvt->_data) {
			CloseHandle(pvt->_map);
			return false;
		}
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::detach() {
	#if defined(RUDIMENTS_HAVE_MMAP) || \
		defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
		#if defined(RUDIMENTS_HAVE_MMAP)
			int32_t	result;
			error::clearError();
			do {
				result=munmap(
					reinterpret_cast<MUNMAP_ADDRCAST>
					(pvt->_data),pvt->_length);
			} while (result==-1 && error::getErrorNumber()==EINTR);
			bool	retval=!result;
		#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
			bool	retval=(UnmapViewOfFile(pvt->_data)==TRUE &&
						CloseHandle(pvt->_map)==TRUE);
		#endif
		pvt->_data=NULL;
		pvt->_length=0;
		return retval;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::setProtection(int32_t protection) {
	return setProtection(0,pvt->_length,protection);
}

bool memorymap::setProtection(off64_t offset, size_t len, int32_t protection) {
	#ifdef RUDIMENTS_HAVE_MPROTECT
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		int32_t	result;
		error::clearError();
		do {
			result=mprotect(
				reinterpret_cast<MPROTECT_ADDRCAST>(ptr),
				len,protection);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void *memorymap::getData() {
	return pvt->_data;
}

size_t memorymap::getLength() {
	return pvt->_length;
}

bool memorymap::sync(bool immediate, bool invalidate) {
	return sync(0,pvt->_length,immediate,invalidate);
}

bool memorymap::sync(off64_t offset, size_t len,
			bool immediate, bool invalidate) {
	#ifdef RUDIMENTS_HAVE_MSYNC
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		int32_t	result;
		error::clearError();
		do {
			result=msync(reinterpret_cast<MSYNC_ADDRCAST>(ptr),len,
					((immediate)?MS_SYNC:MS_ASYNC)|
						((invalidate)?MS_INVALIDATE:0));
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		return (FlushViewOfFile(
			reinterpret_cast<void *>(ptr),len)==TRUE);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::sequentialAccess(off64_t offset, size_t len) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_SEQUENTIAL)
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		return mAdvise(ptr,len,MADV_SEQUENTIAL);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::randomAccess(off64_t offset, size_t len) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_RANDOM)
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		return mAdvise(ptr,len,MADV_RANDOM);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::willNeed(off64_t offset, size_t len) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_WILLNEED)
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		return mAdvise(ptr,len,MADV_WILLNEED);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::wontNeed(off64_t offset, size_t len) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_DONTNEED)
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		return mAdvise(ptr,len,MADV_DONTNEED);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::normalAccess(off64_t offset, size_t len) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_NORMAL)
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		return mAdvise(ptr,len,MADV_NORMAL);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::lock() {
	return lock(0,pvt->_length);
}

bool memorymap::lock(off64_t offset, size_t len) {
	#ifdef RUDIMENTS_HAVE_MLOCK
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		int32_t	result;
		error::clearError();
		do {
			result=mlock(reinterpret_cast<MLOCK_ADDRCAST>(ptr),len);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::unlock() {
	return unlock(0,pvt->_length);
}

bool memorymap::unlock(off64_t offset, size_t len) {
	#ifdef RUDIMENTS_HAVE_MUNLOCK
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		int32_t	result;
		error::clearError();
		do {
			result=munlock(
				reinterpret_cast<MUNLOCK_ADDRCAST>(ptr),
				len);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::inMemory() {
	return inMemory(0,pvt->_length);
}

bool memorymap::inMemory(off64_t offset, size_t len) {

	#ifdef RUDIMENTS_HAVE_MINCORE
		// create an array of char's, 1 for each page
		int32_t		pagesize=sys::getAllocationGranularity();
		int32_t		tmplen=(len+pagesize-1)/pagesize;
		#ifdef RUDIMENTS_HAVE_MINCORE_CHAR
			char		*tmp=new char[tmplen];
		#else
			unsigned char	*tmp=new unsigned char[tmplen];
		#endif

		// call mincore to fill the array
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		int32_t		result;
		error::clearError();
		do {
			result=mincore(
				reinterpret_cast<MINCORE_ADDRCAST>(ptr),
				len,tmp);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (result) {
			delete[] tmp;
			return false;
		}

		// look through the array, if any of the
		// pages aren't in memory, return false
		for (int32_t i=0; i<tmplen; i++) {
			if (tmp[i]) {
				delete[] tmp;
				return false;
			}
		}
		delete[] tmp;
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::lockAll() {
	#if defined(MCL_CURRENT) && defined(MCL_FUTURE)
		return mLockAll(MCL_CURRENT|MCL_FUTURE);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::lockAllCurrent() {
	#if defined(MCL_CURRENT)
		return mLockAll(MCL_CURRENT);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::lockAllFuture() {
	#if defined(MCL_FUTURE)
		return mLockAll(MCL_FUTURE);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::unlockAll() {
	#ifdef RUDIMENTS_HAVE_MUNLOCKALL
		int32_t	result;
		error::clearError();
		do {
			result=munlockall();
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::mAdvise(unsigned char *start, size_t length, int32_t advice) {
	#if defined(RUDIMENTS_HAVE_MADVISE)
		int32_t	result;
		error::clearError();
		do {
			result=madvise(
				reinterpret_cast<MADVISE_ADDRCAST>(start),
				length,advice);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::mLockAll(int32_t flags) {
	#ifdef RUDIMENTS_HAVE_MLOCKALL
		int32_t	result;
		error::clearError();
		do {
			result=mlockall(flags);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::supported() {
	#if defined(_SYLLABLE)
		// Syllable has mmap, but it appears that it can only map
		// from byte-0 of the file.  Attempts to map further into
		// the file just result in re-mapping at byte-0.
		return false;
	#elif defined(RUDIMENTS_HAVE_MMAP) || \
		defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
		return true;
	#else
		return false;
	#endif
}
