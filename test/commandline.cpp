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
			"--five=5",
			"-f","five",
			"--six",
			"-s",
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
	test("found(), -one",cmdline.found("one"));
	test("found(), -two",cmdline.found("two"));
	test("found(), --three",cmdline.found("three"));
	test("found(), --four",cmdline.found("four"));
	test("found(), --five",cmdline.found("five"));
	test("found(), -f",cmdline.found("f"));
	test("found(), --five/-f",cmdline.found("five","f"));
	test("found(), --six",cmdline.found("six"));
	test("found(), -s",cmdline.found("s"));
	test("found(), --six/-s",cmdline.found("six","s"));
	test("found(), --SIX/-s",cmdline.found("SIX","s"));
	test("found(), --six/-S",cmdline.found("six","S"));
	stdoutput.printf("\n");

	// getValue
	stdoutput.printf("getValue...\n");
	test("getValue(), -one",
		!charstring::compare(cmdline.getValue("one"),"1"));
	test("getValue(), -two",
		!charstring::compare(cmdline.getValue("two"),""));
	test("getValue(), --three",
		!charstring::compare(cmdline.getValue("three"),"3"));
	test("getValue(), --four",
		!charstring::compare(cmdline.getValue("four"),""));
	test("getValue(), --five",
		!charstring::compare(cmdline.getValue("five"),"5"));
	test("getValue(), -f",
		!charstring::compare(cmdline.getValue("f"),"five"));
	test("getValue(), --five/-f",
		!charstring::compare(cmdline.getValue("five"),"5"));
	stdoutput.printf("\n");

	process::exit(0);
}
