// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/passwdentry.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif


int main(int argc, const char **argv) {


	// get the password entry for "root"
	passwdentry	pwent;
	pwent.initialize("root");

	// print the components individually
	printf("Individually...\n");
	printf("	Name:			%s\n",
					pwent.getName());
	printf("	Password:		%s\n",
					pwent.getPassword());
	printf("	User Id:		%d\n",
					pwent.getUserId());
	printf("	Primary Group Id:	%d\n",
					pwent.getPrimaryGroupId());
	printf("	Real Name:		%s\n",
					pwent.getRealName());
	printf("	Home Directory:		%s\n",
					pwent.getHomeDirectory());
	printf("	Shell:		%s\n",
					pwent.getShell());
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	pwent.print();
	printf("\n");



	// get the password entry for user id 0
	pwent.initialize((uid_t)0);

	// print the components individually
	printf("Individually...\n");
	printf("	Name:			%s\n",
					pwent.getName());
	printf("	Password:		%s\n",
					pwent.getPassword());
	printf("	User Id:		%d\n",
					pwent.getUserId());
	printf("	Primary Group Id:	%d\n",
					pwent.getPrimaryGroupId());
	printf("	Real Name:		%s\n",
					pwent.getRealName());
	printf("	Home Directory:		%s\n",
					pwent.getHomeDirectory());
	printf("	Shell:			%s\n",
					pwent.getShell());
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	pwent.print();
	printf("\n");
}
