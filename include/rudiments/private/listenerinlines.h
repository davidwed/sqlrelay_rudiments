// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline listener::listener() {
	retryinterruptedwaits=1;
}

inline listener::~listener() {
	removeAllFileDescriptors();
}

inline void listener::retryInterruptedWaits() {
	retryinterruptedwaits=1;
}

inline void listener::dontRetryInterruptedWaits() {
	retryinterruptedwaits=0;
}

inline void listener::addFileDescriptor(int fd) {
	filedescriptorlist.append(fd);
}

inline void listener::removeFileDescriptor(int fd) {
	filedescriptorlist.removeByKey(fd);
}

inline void listener::removeAllFileDescriptors() {
	filedescriptorlist.clear();
}

inline int listener::waitForNonBlockingRead(long sec, long usec) {
	return safeSelect(sec,usec,1,0);
}

inline int listener::waitForNonBlockingWrite(long sec, long usec) {
	return safeSelect(sec,usec,0,1);
}
