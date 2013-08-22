// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/file.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	file	fl;
	fl.open("testfile",O_RDWR);

	stdoutput.printf("locking...\n");
	#if defined(F_RDLCK) && defined(F_WRLCK)
	stdoutput.printf("locked=%d\n",fl.lockFile(F_RDLCK|F_WRLCK));
	#else
	stdoutput.printf("locked=%d\n",fl.lockFile(0));
	#endif

	stdoutput.printf("sleeping\n");
	snooze::macrosnooze(100);
}
