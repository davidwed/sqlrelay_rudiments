// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/protocolentry.h>
#include <rudiments/stdio.h>

void print(protocolentry *pe) {
	stdoutput.printf("	Name:		%s\n",pe->getName());
	stdoutput.printf("	Alias list:\n");
	int	i;
	for (i=0; pe->getAliasList() && pe->getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",pe->getAliasList()[i]);
	}
	stdoutput.printf("	Number:		%d\n",pe->getNumber());
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	// uninitialized
	protocolentry	pe;
	stdoutput.printf("uninitialized:\n");
	print(&pe);

	// get the protocol information for "tcp"
	pe.initialize("tcp");
	stdoutput.printf("tcp:\n");
	print(&pe);

	// get the protocol information for protocol number 6
	pe.initialize(6);
	stdoutput.printf("6:\n");
	print(&pe);

	// null-safety
	pe.initialize((const char *)NULL);
	stdoutput.printf("NULL:\n");
	print(&pe);
}
