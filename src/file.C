// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/file.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/fileinlines.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

char *file::getContents() {
	off_t	size;
	if (fileproperties::getSize(fd,&size)==-1) {
		return NULL;
	}
	char	*buffer=new char[size+1];
	buffer[size]=(char)NULL;
	return (read(buffer,size)==size)?buffer:NULL;
}

size_t file::create(const char *name, mode_t permissions,
					void *data, size_t size) {
	create(name,permissions);
	size_t	retval=write(data,size);
	close();
	return retval;
}
