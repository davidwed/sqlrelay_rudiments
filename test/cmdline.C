// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/commandline.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argv, const char **argc) {

	commandline     cmdline(argv,argc);

	if (cmdline.found("-help")) {
		printf("usage:  divide -divisor number -dividend number\n");
		exit(0);
	}

	if (cmdline.found("-divisor") && cmdline.found("-dividend")) {
		double	divisor=atof(cmdline.value("-divisor"));
		double	dividend=atof(cmdline.value("-dividend"));
		printf("%0.2f\n",divisor/dividend);
	} else {
		printf("You must supply a divisor and a dividend.\n");
	}
}
