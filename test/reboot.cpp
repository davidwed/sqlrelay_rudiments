// Copyright (c) 2012  David Muse
// See the file COPYING for more information

#include <rudiments/system.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {
	if (!system::reboot()) {
		stdoutput.printf("reboot failed: %s\n",error::getErrorString());
	}
}
