// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	commandline     cmdline(argc,argv);

	// if -help was specified, display a usage message
	if (cmdline.found("-help")) {
		printf("usage:  divide -divisor number -dividend number\n");
		exit(0);
	}

	// If -divisor and -dividend are supplied, use them.  Otherwise
	// display an error message.
	if (cmdline.found("-divisor") && cmdline.found("-dividend")) {
		double	divisor=atof(cmdline.value("-divisor"));
		double	dividend=atof(cmdline.value("-dividend"));
		printf("%0.2f\n",divisor/dividend);
	} else {
		printf("You must supply a divisor and a dividend.\n");
	}
}
