// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/passwdentry.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif


int main(int argc, const char **argv) {


	// get the password entry for "root"
	passwdentry	pwent;
	pwent.initialize("root");

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
	stdoutput.printf("	Shell:		%s\n",
						pwent.getShell());
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	pwent.print();
	stdoutput.printf("\n");



	// get the password entry for user id 0
	pwent.initialize((uid_t)0);

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
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	pwent.print();
	stdoutput.printf("\n");
}
