// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline connectiondatanode::connectiondatanode(const char *key,
						const char *value) {
	this->key=(char *)key;
	this->value=(char *)value;
	next=NULL;
}
