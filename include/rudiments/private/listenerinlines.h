// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE listener::listener() {
	retryinterruptedwaits=1;
}

RUDIMENTS_INLINE listener::~listener() {
	removeAllFileDescriptors();
}

RUDIMENTS_INLINE void listener::retryInterruptedWaits() {
	retryinterruptedwaits=1;
}

RUDIMENTS_INLINE void listener::dontRetryInterruptedWaits() {
	retryinterruptedwaits=0;
}

RUDIMENTS_INLINE void listener::addFileDescriptor(int fd) {
	filedescriptorlist.append(fd);
}

RUDIMENTS_INLINE void listener::removeFileDescriptor(int fd) {
	filedescriptorlist.removeByData(fd);
}

RUDIMENTS_INLINE void listener::removeAllFileDescriptors() {
	filedescriptorlist.clear();
}

RUDIMENTS_INLINE int listener::waitForNonBlockingRead(long sec, long usec) {
	return safeSelect(sec,usec,1,0);
}

RUDIMENTS_INLINE int listener::waitForNonBlockingWrite(long sec, long usec) {
	return safeSelect(sec,usec,0,1);
}
