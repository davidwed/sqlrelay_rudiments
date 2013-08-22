// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/environment.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// print the environment variable "TEST"
	stdoutput.printf("TEST=%s\n",environment::getValue("TEST"));

	// set the value of "TEST" to "value"
	if (environment::setValue("TEST","value")) {
		stdoutput.printf("TEST=%s\n",environment::getValue("TEST"));
	} else {
		stdoutput.printf("setValue() failed\n");
	}

	// set the value of "TEST" to "newvalue"
	if (environment::setValue("TEST","newvalue")) {
		stdoutput.printf("TEST=%s\n",environment::getValue("TEST"));
	} else {
		stdoutput.printf("setValue() failed\n");
	}

	// remove "TEST" from the environment
	environment::remove("TEST");

	// print the (non-existent) environment variable "TEST"
	stdoutput.printf("TEST=%s\n",environment::getValue("TEST"));

	stdoutput.printf("\n\n");
	environment::print();

	stdoutput.printf("\n\n");
	environment::clear();

	stdoutput.printf("\n\nclear?\n");
	environment::print();
}
