// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline listener::listener() {
	first=NULL;
	last=NULL;
	current=NULL;
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
