// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/rpcentry.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {


	// get the rpc information for "portmap"
	rpcentry	re;
	re.initialize("portmap");

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",re.getName());
	printf("	Number:		%d\n",re.getNumber());
	printf("	Alias list:\n");
	for (int i=0; re.getAliasList()[i]; i++) {
		printf("		%s\n",re.getAliasList()[i]);
	}
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	re.print();
	printf("\n");



	// get the protocol information for protocol number 100000
	re.initialize(100000);

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",re.getName());
	printf("	Number:		%d\n",re.getNumber());
	printf("	Alias list:\n");
	for (int i=0; re.getAliasList()[i]; i++) {
		printf("		%s\n",re.getAliasList()[i]);
	}
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	re.print();
	printf("\n");
}
