// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {


	// get the group entry for "bin"
	groupentry	grent;
	grent.initialize("bin");

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",grent.getName());
	stdoutput.printf("	Password:	%s\n",grent.getPassword());
	stdoutput.printf("	Group Id:	%d\n",grent.getGroupId());
	stdoutput.printf("	Members:\n");
	int i;
	for (i=0; grent.getMembers()[i]; i++) {
		stdoutput.printf("		%s\n",grent.getMembers()[i]);
	}
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built in...:\n");
	grent.print();
	stdoutput.printf("\n");



	// get the group entry for group id 1
	grent.initialize((gid_t)1);

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:		%s\n",grent.getName());
	stdoutput.printf("	Password:	%s\n",grent.getPassword());
	stdoutput.printf("	Group Id:	%d\n",grent.getGroupId());
	stdoutput.printf("	Members:\n");
	for (i=0; grent.getMembers()[i]; i++) {
		stdoutput.printf("		%s\n",grent.getMembers()[i]);
	}
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built in...:\n");
	grent.print();
	stdoutput.printf("\n");
}
