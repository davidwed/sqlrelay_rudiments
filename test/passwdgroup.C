// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <stdio.h>


int main(int argv, const char **argc) {

	// get the password entry for "root" and print it
	passwdentry	pwent;
	pwent.initialize("root");
	pwent.print();
	printf("\n");


	// get the password entry for user id 0 and print it
	pwent.initialize((uid_t)0);
	pwent.print();
	printf("\n");


	// get the group entry for "bin" and print it
	groupentry	grent;
	grent.initialize("bin");
	grent.print();
	printf("\n");


	// get the group entry for group id 1 and print it
	grent.initialize((gid_t)1);
	grent.print();
}
