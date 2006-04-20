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

	// print the environment variable "TEST"
	printf("TEST=%s\n",environment::getValue("TEST"));

	// set the value of "TEST" to "value"
	if (environment::setValue("TEST","value")) {
		printf("TEST=%s\n",environment::getValue("TEST"));
	} else {
		printf("setValue() failed\n");
	}

	// set the value of "TEST" to "newvalue"
	if (environment::setValue("TEST","newvalue")) {
		printf("TEST=%s\n",environment::getValue("TEST"));
	} else {
		printf("setValue() failed\n");
	}

	// remove "TEST" from the environment
	environment::remove("TEST");

	// print the (non-existent) environment variable "TEST"
	printf("TEST=%s\n",environment::getValue("TEST"));

	printf("\n\n");
	environment::print();

	printf("\n\n");
	environment::clear();

	printf("\n\nclear?\n");
	environment::print();
}
