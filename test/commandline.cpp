// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	commandline     cmdline(argc,argv);

	// if -help was specified, display a usage message
	if (cmdline.found("-help")) {
		stdoutput.printf(
			"usage:  divide -divisor number -dividend number\n");
		process::exit(0);
	}

	// If -divisor and -dividend are supplied, use them.  Otherwise
	// display an error message.
	if (cmdline.found("-divisor") && cmdline.found("-dividend")) {
		double	divisor=charstring::toFloat(
					cmdline.getValue("-divisor"));
		double	dividend=charstring::toFloat(
					cmdline.getValue("-dividend"));
		stdoutput.printf("%0.2f\n",divisor/dividend);
	} else {
		stdoutput.printf("You must supply a divisor and a dividend.\n");
	}
}
