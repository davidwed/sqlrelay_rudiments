// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/timezonefile.h>

int main(int argc, char **argv) {

	timezonefile	tz;
	if (tz.parseFile(argv[1])) {
		tz.print();
	}
}
