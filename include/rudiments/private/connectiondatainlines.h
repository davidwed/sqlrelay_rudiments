// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline connectiondata::connectiondata() {
	first=NULL;
	last=NULL;
	keycount=0;
}

inline int connectiondata::getKeyCount() const {
	return keycount;
}

inline char *connectiondata::getValue(const char *key) const {
	connectiondatanode	*current=findNode(key);
	return current?current->value:NULL;
}

inline char *connectiondata::getKey(int index) const {
	connectiondatanode	*current=findNode(index);
	return current?current->key:NULL;
}

inline char *connectiondata::getValue(int index) const {
	connectiondatanode	*current=findNode(index);
	return current?current->value:NULL;
}
