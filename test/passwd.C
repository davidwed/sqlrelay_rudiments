// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/passwdentry.h>

#include <stdio.h>


int main(int argv, const char **argc) {


	// get the password entry for "root"
	passwdentry	pwent;
	pwent.initialize("root");

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",pwent.getName());
	printf("	Password:	%s\n",pwent.getPassword());
	printf("	User Id:	%d\n",pwent.getUserId());
	printf("	Primary Group:	%d\n",pwent.getPrimaryGroup());
	printf("	Real Name:	%s\n",pwent.getRealName());
	printf("	Home Directory:	%s\n",pwent.getHomeDirectory());
	printf("	Shell:		%s\n",pwent.getShell());
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	pwent.print();
	printf("\n");



	// get the password entry for user id 0
	pwent.initialize((uid_t)0);

	// print the components individually
	printf("Individually...\n");
	printf("	Name:		%s\n",pwent.getName());
	printf("	Password:	%s\n",pwent.getPassword());
	printf("	User Id:	%d\n",pwent.getUserId());
	printf("	Primary Group:	%d\n",pwent.getPrimaryGroup());
	printf("	Real Name:	%s\n",pwent.getRealName());
	printf("	Home Directory:	%s\n",pwent.getHomeDirectory());
	printf("	Shell:		%s\n",pwent.getShell());
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	pwent.print();
	printf("\n");
}
