// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

template <class datatype>
inline int stringdictionarynode<datatype>::compare(char *key) const {
	return strcmp(this->key,key);
}

template <class datatype>
inline void stringdictionarynode<datatype>::print() const {
	printf("key:  %s\n",key);
	printf("data: %x\n",data);
}
