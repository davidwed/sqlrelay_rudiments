// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

template <class datatype>
inline int stringdictionarynode<datatype>::compare(char *key) {
	return strcmp(this->key,key);
}
