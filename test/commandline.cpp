// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	// if we're called with no arguments then
	// re-call ourself with some arguments
	if (argc==1) {

		process::waitForChildren();

		const char	*args[]={
			"commandline",
			"-one","1",
			"-two",
			"--three=3",
			"--four",
			NULL
		};

		process::spawn(
			#ifdef _WIN32
			"commandline.exe",
			#else
			"./commandline",
			#endif
			args,false);
	
		process::exit(0);
	}

	header("commandline");

	commandline     cmdline(argc,argv);

	// found
	stdoutput.printf("found...\n");
	test("found(), -(value)",cmdline.found("one"));
	test("found(), -(no value)",cmdline.found("two"));
	test("found(), --(value)",cmdline.found("three"));
	test("found(), --(no value)",cmdline.found("four"));
	stdoutput.printf("\n");

	// getValue
	stdoutput.printf("getValue...\n");
	test("getValue(), -(value)",
		!charstring::compare(cmdline.getValue("one"),"1"));
	test("getValue(), -(no value)",
		!charstring::compare(cmdline.getValue("two"),""));
	test("getValue(), --(value)",
		!charstring::compare(cmdline.getValue("three"),"3"));
	test("getValue(), --(no value)",
		!charstring::compare(cmdline.getValue("four"),""));
	stdoutput.printf("\n");

	process::exit(0);
}
