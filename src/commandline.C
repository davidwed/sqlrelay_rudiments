// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/commandline.h>
#ifndef ENABLE_INLINES
	#define RUDIMENTS_INLINE
	#include <rudiments/private/commandlineinlines.h>
#endif

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <stdio.h>

char *commandline::value(const char *arg) const {

	if (arg && arg[0]) {
		for (int i=1; i<argc-1; i++) {
			if (!strcmp(arg,argv[i])) {
				return argv[i+1];
			}
		}
	}
	return "";
}

int commandline::found(const char *arg) const {

	if (arg && arg[0]) {
		for (int i=1; i<argc; i++) {
			if (!strcmp(arg,argv[i])) {
				return 1;
			}
		}
	}
	return 0;
}
