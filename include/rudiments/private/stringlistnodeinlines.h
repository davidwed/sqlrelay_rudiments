// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

inline int stringlistnode::compare(char *key) {
	return strcmp(data,key);
}
