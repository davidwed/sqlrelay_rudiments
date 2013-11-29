// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/passwdentry.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {


	// get the password entry for "dmuse"
	passwdentry	pwent;
	pwent.initialize("dmuse");

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:			%s\n",
						pwent.getName());
	stdoutput.printf("	Password:		%s\n",
						pwent.getPassword());
	stdoutput.printf("	User Id:		%d\n",
						pwent.getUserId());
	stdoutput.printf("	Primary Group Id:	%d\n",
						pwent.getPrimaryGroupId());
	stdoutput.printf("	Real Name:		%s\n",
						pwent.getRealName());
	stdoutput.printf("	Home Directory:		%s\n",
						pwent.getHomeDirectory());
	stdoutput.printf("	Shell:			%s\n",
						pwent.getShell());
	stdoutput.printf("	SID:			%s\n",
						pwent.getSid());
	stdoutput.printf("\n");



	// get the password entry for user id of the user we just looked up
	pwent.initialize(pwent.getUserId());

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name:			%s\n",
						pwent.getName());
	stdoutput.printf("	Password:		%s\n",
						pwent.getPassword());
	stdoutput.printf("	User Id:		%d\n",
						pwent.getUserId());
	stdoutput.printf("	Primary Group Id:	%d\n",
						pwent.getPrimaryGroupId());
	stdoutput.printf("	Real Name:		%s\n",
						pwent.getRealName());
	stdoutput.printf("	Home Directory:		%s\n",
						pwent.getHomeDirectory());
	stdoutput.printf("	Shell:			%s\n",
						pwent.getShell());
	stdoutput.printf("	SID:			%s\n",
						pwent.getSid());
	stdoutput.printf("\n");
}
