// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <stdio.h>


int	main() {

	passwdentry	*pwent=new passwdentry();
	pwent->initialize("root");
	pwent->print();
	printf("\n");

	pwent->initialize((uid_t)0);
	pwent->print();
	printf("\n");
	delete pwent;

	groupentry	*grent=new groupentry();
	grent->initialize("root");
	grent->print();
	printf("\n");

	grent->initialize((gid_t)0);
	grent->print();
	delete grent;
}
