// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>

#include <stdio.h>

stdiofiledescriptor::stdiofiledescriptor(int32_t fd) : filedescriptor(fd) {
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
