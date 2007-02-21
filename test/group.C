// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {


	// get the group entry for "bin"
	groupentry	grent;
	grent.initialize("bin");

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",grent.getName());
	printf("	Password:	%s\n",grent.getPassword());
	printf("	Group Id:	%d\n",grent.getGroupId());
	printf("	Members:\n");
	for (int i=0; grent.getMembers()[i]; i++) {
		printf("		%s\n",grent.getMembers()[i]);
	}
	printf("\n");

	// use the built in print method
	printf("Built in...:\n");
	grent.print();
	printf("\n");



	// get the group entry for group id 1
	grent.initialize((gid_t)1);

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",grent.getName());
	printf("	Password:	%s\n",grent.getPassword());
	printf("	Group Id:	%d\n",grent.getGroupId());
	printf("	Members:\n");
	for (int i=0; grent.getMembers()[i]; i++) {
		printf("		%s\n",grent.getMembers()[i]);
	}
	printf("\n");

	// use the built in print method
	printf("Built in...:\n");
	grent.print();
	printf("\n");
}
