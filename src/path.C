// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/path.h>
#include <rudiments/charstring.h>

#include <stdlib.h>
#include <sys/ipc.h>

char *path::dirname(const char *pathname) {

	if (!pathname) {
		return NULL;
	} else if (!charstring::contains(pathname,'/') ||
			!charstring::compare(pathname,".")) {
		return charstring::duplicate(".");
	} else if (!charstring::compare(pathname,"..")) {
		return charstring::duplicate("..");
	} else if (!charstring::compare(pathname,"/")) {
		return charstring::duplicate("/");
	}

	char	*retval=charstring::duplicate(pathname);
	charstring::rightTrim(retval,'/');
	char	*lastslash=charstring::lastOccurrance(retval,'/');
	if (lastslash==retval) {
		(*(lastslash+1))=(char)NULL;
	} else {
		(*lastslash)=(char)NULL;
	}
	return retval;
}

char *path::basename(const char *pathname) {

	if (!pathname) {
		return NULL;
	}

	char	*copy=charstring::duplicate(pathname);
	charstring::rightTrim(copy,'/');

	char	*retval;
	char	*ptr=charstring::lastOccurrance(copy,'/');
	if (!ptr) {
		retval=charstring::duplicate(copy);
	} else {
		retval=charstring::duplicate(ptr+1);
	}
	delete[] copy;
	return retval;
}

char *path::basename(const char *pathname, const char *suffix) {
	char	*retval=basename(pathname);
	char	*ptr=charstring::lastOccurrance(retval,suffix);
	if (!(*(ptr+charstring::length(suffix)))) {
		(*ptr)=(char)NULL;
	}
	return retval;
}

key_t path::generateKey(const char *pathname, int id) {
	return ::ftok(pathname,id);
}
