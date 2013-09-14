// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/error.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	error::setErrorNumber(EINTR);
	stdoutput.printf("error number: %d\n",error::getErrorNumber());
	stdoutput.printf("error string: %s\n",error::getErrorString());
	error::clearError();
	stdoutput.printf("error number (after clear): %d\n",
						error::getErrorNumber());
	stdoutput.printf("error string (after clear) : %s\n",
						error::getErrorString());
}
