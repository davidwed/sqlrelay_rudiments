// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE memorypoolnode::~memorypoolnode() {
	delete[] buffer;
}

RUDIMENTS_INLINE memorypool::~memorypool() {
	free();
	delete nodelist.getNodeByIndex(0)->getData();
}
