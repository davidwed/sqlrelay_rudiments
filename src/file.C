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
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

bool file::open(const char *name, int flags) {
	return ((fd=::open(name,flags))!=-1 &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true));
}

bool file::open(const char *name, int flags, mode_t perms) {
	return ((fd=::open(name,flags,perms))!=-1 &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true));
}

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

ssize_t file::getContents(unsigned char *buffer, size_t buffersize) {
	return read(buffer,(buffersize<(size_t)getSize())?buffersize:getSize());
}

ssize_t file::create(const char *name, mode_t perms, const char *string) {
	return create(name,perms,(void *)string,strlen(string));
}

ssize_t file::create(const char *name, mode_t permissions,
					const void *data, size_t size) {
	size_t	retval;
	if (((fd=::open(name,O_CREAT|O_TRUNC|O_RDWR,permissions))!=-1) &&
		((retval=write(data,size))==size) &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true)) {
		return retval;
	}
	close();
	return -1;
}
