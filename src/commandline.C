// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/commandline.h>
#include <rudiments/charstring.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

commandline::commandline(int argc, const char **argv) {
	this->argc=argc;
	this->argv=(char **)argv;
}

commandline::~commandline() {}

const char *commandline::value(const char *arg) const {

	if (arg && arg[0]) {
		for (int i=1; i<argc-1; i++) {
			if (!charstring::compare(arg,argv[i])) {
				return argv[i+1];
			}
		}
	}
	return "";
}

bool commandline::found(const char *arg) const {

	if (arg && arg[0]) {
		for (int i=1; i<argc; i++) {
			if (!charstring::compare(arg,argv[i])) {
				return true;
			}
		}
	}
	return false;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
