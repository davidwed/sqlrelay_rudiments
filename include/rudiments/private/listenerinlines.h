// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline listener::listener() {
	first=NULL;
	last=NULL;
	current=NULL;
}

inline listener::~listener() {
	removeAllFileDescriptors();
}
