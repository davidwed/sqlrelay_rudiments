// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/passwdentry.h>
#include <rudiments/stdio.h>

void print(passwdentry *pwent) {
	stdoutput.printf("	Name:			%s\n",
						pwent->getName());
	stdoutput.printf("	Password:		%s\n",
						pwent->getPassword());
	stdoutput.printf("	User Id:		%d\n",
						pwent->getUserId());
	stdoutput.printf("	Primary Group Id:	%d\n",
						pwent->getPrimaryGroupId());
	stdoutput.printf("	Real Name:		%s\n",
						pwent->getRealName());
	stdoutput.printf("	Home Directory:		%s\n",
						pwent->getHomeDirectory());
	stdoutput.printf("	Shell:			%s\n",
						pwent->getShell());
	stdoutput.printf("	SID:			%s\n",
						pwent->getSid());
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	// get the password entry for "dmuse"
	passwdentry	pwent;
	pwent.initialize("dmuse");
	stdoutput.printf("username: dmuse...\n");
	print(&pwent);

	// get the password entry for user id of the user we just looked up
	uid_t	id=pwent.getUserId();
	pwent.initialize(id);
	stdoutput.printf("userid: %d...\n",id);
	print(&pwent);

	// invalid user
	pwent.initialize("invalidusername");
	stdoutput.printf("username: invalidusername...\n");
	print(&pwent);

	// invalid user id
	id=pwent.getUserId();
	pwent.initialize(id);
	stdoutput.printf("userid: %d (invalid)...\n",id);
	print(&pwent);
}
