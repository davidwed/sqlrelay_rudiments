// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

inline int stringlistnode::compare(char *data) const {
	return strcmp(this->data,data);
}

inline void stringlistnode::print() const {
	printf("%s\n",data);
}
