// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

inline commandline::commandline(int argc, const char **argv) {
	this->argc=argc;
	this->argv=(char **)argv;
}

inline commandline::~commandline() {
}
