// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/rpcentry.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {


	// get the rpc information for "portmap"
	rpcentry	re;
	re.initialize("portmap");

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",re.getName());
	stdoutput.printf("	Number:		%d\n",re.getNumber());
	stdoutput.printf("	Alias list:\n");
	int	i;
	for (i=0; re.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",re.getAliasList()[i]);
	}
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	re.print();
	stdoutput.printf("\n");



	// get the protocol information for protocol number 100000
	re.initialize(100000);

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",re.getName());
	stdoutput.printf("	Number:		%d\n",re.getNumber());
	stdoutput.printf("	Alias list:\n");
	for (i=0; re.getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",re.getAliasList()[i]);
	}
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	re.print();
	stdoutput.printf("\n");
}
