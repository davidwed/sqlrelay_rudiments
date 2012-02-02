// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/error.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	error::setErrorNumber(EINTR);
	printf("error number: %d\n",error::getErrorNumber());
	printf("error string: %s\n",error::getErrorString());
	error::clearError();
	printf("error number (after clear): %d\n",error::getErrorNumber());
	printf("error string (after clear) : %s\n",error::getErrorString());
}
