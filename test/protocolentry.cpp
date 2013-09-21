// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/protocolentry.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {


	// get the protocol information for "tcp"
	protocolentry	pe;
	pe.initialize("tcp");

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",pe.getName());
	stdoutput.printf("	Alias list:\n");
	int	i;
	for (i=0; pe.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",pe.getAliasList()[i]);
	}
	stdoutput.printf("	Number:		%d\n",pe.getNumber());
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	pe.print();
	stdoutput.printf("\n");



	// get the protocol information for protocol number 6
	pe.initialize(6);

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",pe.getName());
	stdoutput.printf("	Alias list:\n");
	for (i=0; pe.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",pe.getAliasList()[i]);
	}
	stdoutput.printf("	Number:		%d\n",pe.getNumber());
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	pe.print();
	stdoutput.printf("\n");
}
