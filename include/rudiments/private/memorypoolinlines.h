// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

INLINE memorypoolnode::~memorypoolnode() {
	delete[] buffer;
}

INLINE memorypool::~memorypool() {
	free();
	delete nodelist.getNodeByIndex(0)->getData();
}
