// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE commandline::commandline(int argc, const char **argv) {
	this->argc=argc;
	this->argv=(char **)argv;
}

RUDIMENTS_INLINE commandline::~commandline() {
}
