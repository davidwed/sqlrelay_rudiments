// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/serviceentry.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {


	// get the service information for "smtp","tcp"
	serviceentry	se;
	se.initialize("smtp","tcp");

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",se.getName());
	stdoutput.printf("	Port:		%d\n",se.getPort());
	stdoutput.printf("	Protocol:	%s\n",se.getProtocol());
	stdoutput.printf("	Alias list:\n");
	for (int i=0; se.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",se.getAliasList()[i]);
	}
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	se.print();
	stdoutput.printf("\n");



	// get the service information for the service on port 25, "tcp"
	se.initialize(25,"tcp");

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",se.getName());
	stdoutput.printf("	Port:		%d\n",se.getPort());
	stdoutput.printf("	Protocol:	%s\n",se.getProtocol());
	stdoutput.printf("	Alias list:\n");
	for (int i=0; se.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",se.getAliasList()[i]);
	}
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	se.print();
	stdoutput.printf("\n");
}
