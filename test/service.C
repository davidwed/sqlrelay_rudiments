// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/serviceentry.h>

#include <stdio.h>

int main(int argv, const char **argc) {


	// get the service information for "smtp","tcp"
	serviceentry	se;
	se.initialize("smtp","tcp");

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",se.getName());
	printf("	Port:		%d\n",se.getPort());
	printf("	Protocol:	%s\n",se.getProtocol());
	printf("	Alias list:\n");
	for (int i=0; se.getAliasList()[i]; i++) {
		printf("		%s\n",se.getAliasList()[i]);
	}
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	se.print();
	printf("\n");



	// get the service information for the service on port 25, "tcp"
	se.initialize(25,"tcp");

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",se.getName());
	printf("	Port:		%d\n",se.getPort());
	printf("	Protocol:	%s\n",se.getProtocol());
	printf("	Alias list:\n");
	for (int i=0; se.getAliasList()[i]; i++) {
		printf("		%s\n",se.getAliasList()[i]);
	}
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	se.print();
	printf("\n");
}
