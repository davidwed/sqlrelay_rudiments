// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/snooze.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	file	fl;
	fl.create("testfile",permissions::evalPermString("rw-rw----"),"hello");

	printf("locking...\n");
	printf("locked=%d\n",fl.lockFile(F_RDLCK|F_WRLCK));

	printf("sleeping\n");
	snooze::macrosnooze(100);
}
