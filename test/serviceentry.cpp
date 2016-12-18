// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/serviceentry.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	// uninitialized
	serviceentry	se;
	stdoutput.printf("uninitialized:\n");
	test("name",!se.getName());
	test("port",se.getPort()==-1);
	test("protcol",!se.getProtocol());
	test("alias list",!se.getAliasList());
	stdoutput.printf("\n");

	// get the service information for "smtp","tcp"
	stdoutput.printf("smtp/tcp:\n");
	se.initialize("smtp","tcp");
	test("name",!charstring::compare(se.getName(),"smtp"));
	test("port",se.getPort()==25);
	test("protcol",!charstring::compare(se.getProtocol(),"tcp"));
	// some platforms alias smtp as mail, but some do not
	test("alias list",!se.getAliasList() ||
			(se.getAliasList() &&
			!charstring::compare(se.getAliasList()[0],"mail")));
	stdoutput.printf("\n");

	// get the service information for the service on port 25, "tcp"
	stdoutput.printf("25/tcp:\n");
	se.initialize(25,"tcp");
	test("name",!charstring::compare(se.getName(),"smtp"));
	test("port",se.getPort()==25);
	test("protcol",!charstring::compare(se.getProtocol(),"tcp"));
	// some platforms alias smtp as mail, but some do not
	test("alias list",!se.getAliasList() ||
			(se.getAliasList() &&
			!charstring::compare(se.getAliasList()[0],"mail")));
	stdoutput.printf("\n");

	// null-safety
	se.initialize((const char *)NULL,NULL);
	stdoutput.printf("NULL/NULL:\n");
	test("name",!se.getName());
	test("port",se.getPort()==-1);
	test("protcol",!se.getProtocol());
	test("alias list",!se.getAliasList());
	stdoutput.printf("\n");
}
