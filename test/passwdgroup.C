// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <stdio.h>


int	main() {

	passwdentry	*pwent=new passwdentry();
	pwent->initialize("root");
	printf("name:      %s\n",pwent->getName());
	printf("password:  %s\n",pwent->getPassword());
	printf("id:        %d\n",pwent->getUserId());
	printf("group:     %d\n",pwent->getPrimaryGroup());
	printf("real name: %s\n",pwent->getRealName());
	printf("home:      %s\n",pwent->getHomeDirectory());
	printf("shell:     %s\n\n",pwent->getShell());

	pwent->initialize((uid_t)0);
	printf("name:      %s\n",pwent->getName());
	printf("password:  %s\n",pwent->getPassword());
	printf("id:        %d\n",pwent->getUserId());
	printf("group:     %d\n",pwent->getPrimaryGroup());
	printf("real name: %s\n",pwent->getRealName());
	printf("home:      %s\n",pwent->getHomeDirectory());
	printf("shell:     %s\n\n\n",pwent->getShell());
	delete pwent;

	groupentry	*grent=new groupentry();
	grent->initialize("root");
	printf("name:      %s\n",grent->getName());
	printf("password:  %s\n",grent->getPassword());
	printf("id:        %d\n",grent->getGroupId());
	for (int index=0; grent->getMembers()[index]; index++) {
		printf("member[%d]: %s\n",index,grent->getMembers()[index]);
	}

	grent->initialize((gid_t)0);
	printf("\nname:      %s\n",grent->getName());
	printf("password:  %s\n",grent->getPassword());
	printf("id:        %d\n",grent->getGroupId());
	for (int index=0; grent->getMembers()[index]; index++) {
		printf("member[%d]: %s\n",index,grent->getMembers()[index]);
	}

	delete grent;
}
