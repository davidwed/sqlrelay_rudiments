// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/memorymap.h>

#include <sys/mman.h>

// for getpagesize()...
#include <unistd.h>

#ifdef HAVE_MMAP_CADDR_T
	#define ADDRCAST caddr_t
#else
	#define ADDRCAST void *
#endif

memorymap::memorymap() {
	data=NULL;
	length=0;
}

memorymap::~memorymap() {
	detach();
}

bool memorymap::attach(int fd, off_t offset, size_t len,
					int protection, int flags) {
	length=len;
	data=mmap(NULL,len,protection,flags,fd,offset);
	return (data!=MAP_FAILED);
}

bool memorymap::detach() {
	#ifdef HAVE_MMAP_CADDR_T
	return !munmap((caddr_t)data,length);
	#else
	return !munmap(data,length);
	#endif
}

bool memorymap::setProtection(int protection) {
	return setProtection(0,length,protection);
}

bool memorymap::setProtection(off_t offset, size_t len, int protection) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !mprotect((ADDRCAST)ptr,len,protection);
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

bool memorymap::sync(off_t offset, size_t len,
			bool immediate, bool invalidate) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !msync((ADDRCAST)ptr,len,((immediate)?MS_SYNC:MS_ASYNC)|
					((invalidate)?MS_INVALIDATE:0));
}

bool memorymap::sequentialAccess(off_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((ADDRCAST)ptr,len,MADV_SEQUENTIAL);
	#else
	return true;
	#endif
}

bool memorymap::randomAccess(off_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((ADDRCAST)ptr,len,MADV_RANDOM);
	#else
	return true;
	#endif
}

bool memorymap::willNeed(off_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((ADDRCAST)ptr,len,MADV_WILLNEED);
	#else
	return true;
	#endif
}

bool memorymap::wontNeed(off_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((ADDRCAST)ptr,len,MADV_DONTNEED);
	#else
	return true;
	#endif
}

bool memorymap::normalAccess(off_t offset, size_t len) {
	#ifdef HAVE_MADVISE
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((ADDRCAST)ptr,len,MADV_NORMAL);
	#else
	return true;
	#endif
}

#ifdef HAVE_MLOCK
bool memorymap::lock() {
	return lock(0,length);
}

bool memorymap::lock(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !mlock((ADDRCAST)ptr,len);
}
#endif

#ifdef HAVE_MUNLOCK
bool memorymap::unlock() {
	return !munlock(0,length);
}

bool memorymap::unlock(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !munlock((ADDRCAST)ptr,len);
}
#endif

#ifdef HAVE_MINCORE
bool memorymap::inMemory() {
	return inMemory(0,length);
}

bool memorymap::inMemory(off_t offset, size_t len) {

	// create an array of char's, 1 for each page
	int		pagesize=getpagesize();
	int		tmplen=(len+pagesize-1)/pagesize;
	#ifdef HAVE_MINCORE_CHAR
	char		tmp[tmplen];
	#else
	unsigned char	tmp[tmplen];
	#endif

	// call mincore to fill the array
	unsigned char	*ptr=((unsigned char *)data)+offset;
	if (mincore((ADDRCAST)ptr,len,tmp)) {
		return false;
	}

	// look through the array, if any of the
	// pages aren't in memory, return false
	for (int i=0; i<tmplen; i++) {
		if (tmp[i]) {
			return false;
		}
	}
	return true;
}
#endif

#ifdef HAVE_MLOCKALL
bool memorymap::lockAll() {
	return !mlockall(MCL_CURRENT|MCL_FUTURE);
}

bool memorymap::lockAllCurrent() {
	return !mlockall(MCL_CURRENT);
}

bool memorymap::lockAllFuture() {
	return !mlockall(MCL_FUTURE);
}
#endif

#ifdef HAVE_MUNLOCKALL
bool memorymap::unlockAll() {
	return !munlockall();
}
#endif
