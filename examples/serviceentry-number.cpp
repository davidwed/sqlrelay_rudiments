// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/serviceentry.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// get the service information for the service
	// that would run on port 25, "tcp" protocol
	serviceentry	se;
	se.initialize(25,"tcp");

	// print out details
	stdoutput.printf("	Name:		%s\n",se.getName());
	stdoutput.printf("	Port:		%d\n",se.getPort());
	stdoutput.printf("	Protocol:	%s\n",se.getProtocol());
	stdoutput.printf("	Alias list:\n");
	for (uint32_t i=0; se.getAliasList() && se.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",se.getAliasList()[i]);
	}
}
