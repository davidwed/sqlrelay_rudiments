// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/file.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/fileinlines.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

char *file::getContents() {
	off_t	size=(fd>-1)?st.st_size:0;
	char	*contents=new char[size+1];
	contents[size]=(char)NULL;
	return (size==0 || read(contents,size)==size)?contents:NULL;
}

char *file::getContents(const char *name) {
	file	fl;
	fl.open(name,O_RDONLY);
	char	*contents=fl.getContents();
	fl.close();
	return contents;
}

ssize_t file::getContents(const char *name, unsigned char *buffer,
						size_t buffersize) {
	file	fl;
	fl.open(name,O_RDONLY);
	ssize_t	bytes=fl.getContents(buffer,buffersize);
	fl.close();
	return bytes;
}

size_t file::create(const char *name, mode_t permissions,
					const void *data, size_t size) {
	create(name,permissions);
	size_t	retval=write(data,size);
	close();
	return retval;
}
