// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/environment.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	environment	env;

	// print the environment variable "TEST"
	printf("TEST=%s\n",env.getValue("TEST"));

	// set the value of "TEST" to "value"
	if (env.setValue("TEST","value")) {
		printf("TEST=%s\n",env.getValue("TEST"));
	} else {
		printf("setValue() failed\n");
	}

	// set the value of "TEST" to "newvalue"
	if (env.setValue("TEST","newvalue")) {
		printf("TEST=%s\n",env.getValue("TEST"));
	} else {
		printf("setValue() failed\n");
	}

	// remove "TEST" from the environment
	env.remove("TEST");

	// print the (non-existent) environment variable "TEST"
	printf("TEST=%s\n",env.getValue("TEST"));

	printf("\n\n");
	environment::print();
}
