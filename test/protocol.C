// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/protocolentry.h>

#include <stdio.h>

int main(int argc, const char **argv) {


	// get the protocol information for "tcp"
	protocolentry	pe;
	pe.initialize("tcp");

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",pe.getName());
	printf("	Alias list:\n");
	for (int i=0; pe.getAliasList()[i]; i++) {
		printf("		%s\n",pe.getAliasList()[i]);
	}
	printf("	Number:		%d\n",pe.getNumber());
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	pe.print();
	printf("\n");



	// get the protocol information for protocol number 6
	pe.initialize(6);

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",pe.getName());
	printf("	Alias list:\n");
	for (int i=0; pe.getAliasList()[i]; i++) {
		printf("		%s\n",pe.getAliasList()[i]);
	}
	printf("	Number:		%d\n",pe.getNumber());
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	pe.print();
	printf("\n");
}
