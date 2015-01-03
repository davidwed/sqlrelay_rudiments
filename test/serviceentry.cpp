// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/serviceentry.h>
#include <rudiments/stdio.h>

void print(serviceentry *se) {
	stdoutput.printf("	Name:		%s\n",se->getName());
	stdoutput.printf("	Port:		%d\n",se->getPort());
	stdoutput.printf("	Protocol:	%s\n",se->getProtocol());
	stdoutput.printf("	Alias list:\n");
	int	i;
	for (i=0; se->getAliasList() && se->getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",se->getAliasList()[i]);
	}
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	// uninitialized
	serviceentry	se;
	stdoutput.printf("uninitialized:\n");
	print(&se);


	// get the service information for "smtp","tcp"
	se.initialize("smtp","tcp");
	stdoutput.printf("smtp/tcp:\n");
	print(&se);

	// get the service information for the service on port 25, "tcp"
	se.initialize(25,"tcp");
	stdoutput.printf("25/tcp:\n");
	print(&se);

	// null-safety
	se.initialize((const char *)NULL,NULL);
	stdoutput.printf("NULL/NULL:\n");
	print(&se);
}
