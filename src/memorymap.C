// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/memorymap.h>

#include <sys/mman.h>

// for getpagesize()...
#include <unistd.h>

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
	return !munmap(data,length);
}

bool memorymap::setProtection(int protection) {
	return setProtection(0,length,protection);
}

bool memorymap::setProtection(off_t offset, size_t len, int protection) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !mprotect((void *)ptr,len,protection);
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
	return !msync((void *)ptr,len,((immediate)?MS_SYNC:MS_ASYNC)|
					((invalidate)?MS_INVALIDATE:0));
}

bool memorymap::sequentialAccess(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((void *)ptr,len,MADV_SEQUENTIAL);
}

bool memorymap::randomAccess(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((void *)ptr,len,MADV_RANDOM);
}

bool memorymap::willNeed(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((void *)ptr,len,MADV_WILLNEED);
}

bool memorymap::wontNeed(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((void *)ptr,len,MADV_DONTNEED);
}

bool memorymap::normalAccess(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !madvise((void *)ptr,len,MADV_NORMAL);
}

bool memorymap::lock() {
	return lock(0,length);
}

bool memorymap::lock(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !mlock((void *)ptr,len);
}

bool memorymap::unlock() {
	return !munlock(0,length);
}

bool memorymap::unlock(off_t offset, size_t len) {
	unsigned char	*ptr=((unsigned char *)data)+offset;
	return !munlock((void *)ptr,len);
}

bool memorymap::resize(size_t newlength, bool maymove) {
	return !mremap(data,length,newlength,(maymove)?MREMAP_MAYMOVE:0);
}

bool memorymap::inMemory() {
	return inMemory(0,length);
}

bool memorymap::inMemory(off_t offset, size_t len) {

	// create an array of char's, 1 for each page
	int		pagesize=getpagesize();
	int		tmplen=(len+pagesize-1)/pagesize;
	unsigned char	tmp[tmplen];

	// call mincore to fill the array
	unsigned char	*ptr=((unsigned char *)data)+offset;
	if (mincore((void *)ptr,len,tmp)) {
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

bool memorymap::unlockAll() {
	return !munlockall();
}
