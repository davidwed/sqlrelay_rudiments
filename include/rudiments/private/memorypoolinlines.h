// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

inline memorypoolnode::~memorypoolnode() {
	delete[] buffer;
}

inline memorypool::~memorypool() {
	free();
	delete nodelist.getNodeByIndex(0)->getData();
}
