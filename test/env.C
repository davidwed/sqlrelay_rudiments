// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/environment.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argv, const char **argc) {

	environment	env;

	printf("TEST=%s\n",env.getValue("TEST"));
	if (env.setValue("TEST","value")) {
		printf("TEST=%s\n",env.getValue("TEST"));
	} else {
		printf("setValue() failed\n");
	}
	if (env.setValue("TEST","newvalue")) {
		printf("TEST=%s\n",env.getValue("TEST"));
	} else {
		printf("setValue() failed\n");
	}
	env.remove("TEST");
	printf("TEST=%s\n",env.getValue("TEST"));
}
