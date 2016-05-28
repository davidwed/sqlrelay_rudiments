// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>

#include <stdio.h>

stdiofiledescriptor::stdiofiledescriptor(int32_t fd) : filedescriptor(fd) {

	// This is really for stdinput, nobody reads from stdout/err,
	// but it doesn't hurt them.
	allowShortReads();
}

stdiofiledescriptor::~stdiofiledescriptor() {

	// This destructor runs when a program exits, but it can also happen
	// when a library is unloaded.  Eg. an apache module.  In cases like
	// that, we don't want the file descriptors for 0, 1, or 2 to be closed
	// or the server itself won't be able to read/write to them.  Setting
	// the file descriptor to -1 here accomplishes this.
	setFileDescriptor(-1);
}

bool stdiofiledescriptor::flush() {
	FILE	*strm=NULL;
	switch (fd()) {
		case 0:
			strm=stdin;
			break;
		case 1:
			strm=stdout;
			break;
		case 2:
			strm=stderr;
			break;
		default:
			return false;
	}
	return !fflush(strm);
}

RUDIMENTS_DLLSPEC stdiofiledescriptor	stdinput(0);
RUDIMENTS_DLLSPEC stdiofiledescriptor	stdoutput(1);
RUDIMENTS_DLLSPEC stdiofiledescriptor	stderror(2);
