// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

INLINE listener::listener() {
	retryinterruptedwaits=1;
}

INLINE listener::~listener() {
	removeAllFileDescriptors();
}

INLINE void listener::retryInterruptedWaits() {
	retryinterruptedwaits=1;
}

INLINE void listener::dontRetryInterruptedWaits() {
	retryinterruptedwaits=0;
}

INLINE void listener::addFileDescriptor(int fd) {
	filedescriptorlist.append(fd);
}

INLINE void listener::removeFileDescriptor(int fd) {
	filedescriptorlist.removeByData(fd);
}

INLINE void listener::removeAllFileDescriptors() {
	filedescriptorlist.clear();
}

INLINE int listener::waitForNonBlockingRead(long sec, long usec) {
	return safeSelect(sec,usec,1,0);
}

INLINE int listener::waitForNonBlockingWrite(long sec, long usec) {
	return safeSelect(sec,usec,0,1);
}
