// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

inline stringbuffer::stringbuffer() {
	buffer=NULL;
}

inline stringbuffer::~stringbuffer() {
	delete[] buffer;
}

inline char	*stringbuffer::getString() {
	return (buffer)?buffer:(char *)"";
}

inline void	stringbuffer::clear() {
	delete[] buffer;
	buffer=NULL;
}
