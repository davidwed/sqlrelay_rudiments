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
	char	*contents=new char[st.st_size+1];
	contents[st.st_size]=(char)NULL;
	return (st.st_size==0 || read(contents,st.st_size)==sizeof(st.st_size))?
			contents:NULL;
}

char *file::getContents(const char *name) {
	file	fl;
	fl.open(name,O_RDONLY);
	char	*contents=fl.getContents();
	fl.close();
	return contents;
}

size_t file::create(const char *name, mode_t permissions,
					const void *data, size_t size) {
	create(name,permissions);
	size_t	retval=write(data,size);
	close();
	return retval;
}
