// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/userentry.h>
#include <rudiments/stdio.h>

void print(userentry *uent) {
	stdoutput.printf("	Name:			%s\n",
						uent->getName());
	stdoutput.printf("	Password:		%s\n",
						uent->getPassword());
	stdoutput.printf("	User Id:		%d\n",
						uent->getUserId());
	stdoutput.printf("	Primary Group Id:	%d\n",
						uent->getPrimaryGroupId());
	stdoutput.printf("	Real Name:		%s\n",
						uent->getRealName());
	stdoutput.printf("	Home Directory:		%s\n",
						uent->getHomeDirectory());
	stdoutput.printf("	Shell:			%s\n",
						uent->getShell());
	stdoutput.printf("	Name: 			%s\n",
						uent->getName());
	stdoutput.printf("	Encrypted Password:	%s\n",
						uent->getEncryptedPassword());
	stdoutput.printf("	Last Change:		%ld\n",
						uent->getLastChangeDate());
	stdoutput.printf("	Days Before Change Allowed:	%d\n",
					uent->getDaysBeforeChangeAllowed());
	stdoutput.printf("	Days Before Change Required:	%d\n",
					uent->getDaysBeforeChangeRequired());
	stdoutput.printf("	Days Before Expiration Warning:	%d\n",
					uent->getDaysBeforeExpirationWarning());
	stdoutput.printf("	Days Of Inactivity Allowed:	%d\n",
					uent->getDaysOfInactivityAllowed());
	stdoutput.printf("	Expiration Date:	%d\n",
						uent->getExpirationDate());
	stdoutput.printf("	Flag:			%d\n",
						uent->getFlag());
	stdoutput.printf("	SID:			%s\n",
						uent->getSidString());
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	// uninitialized
	userentry	uent;
	stdoutput.printf("uninitialized...\n");
	print(&uent);

	// get the user entry for "dmuse"
	uent.initialize("dmuse");
	stdoutput.printf("username: dmuse...\n");
	print(&uent);

	// get the user entry for user id of the user we just looked up
	uid_t	id=uent.getUserId();
	uent.initialize(id);
	stdoutput.printf("userid: %d...\n",id);
	print(&uent);

	// invalid user
	uent.initialize("invalidusername");
	stdoutput.printf("username: invalidusername...\n");
	print(&uent);

	// invalid user id
	id=uent.getUserId();
	uent.initialize(id);
	stdoutput.printf("userid: %d (invalid)...\n",id);
	print(&uent);

	// null-safety
	stdoutput.printf("NULL...\n");
	uent.initialize((const char *)NULL);
	print(&uent);
}
