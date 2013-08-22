// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/shadowentry.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif


int main(int argc, const char **argv) {


	// get the shadow entry for "root"
	shadowentry	spent;
	spent.initialize("root");

	// print the components individually
	stdoutput.printf("Individually...\n");
	stdoutput.printf("	Name: %s\n",spent.getName());
	stdoutput.printf("	Encrypted Password: %s\n",
					spent.getEncryptedPassword());
	stdoutput.printf("	Last Change: %ld\n",
					spent.getLastChangeDate());
	stdoutput.printf("	Days Before Change Allowed: %d\n",
					spent.getDaysBeforeChangeAllowed());
	stdoutput.printf("	Days Before Change Required: %d\n",
					spent.getDaysBeforeChangeRequired());
	stdoutput.printf("	Days Before Expiration Warning: %d\n",
					spent.getDaysBeforeExpirationWarning());
	stdoutput.printf("	Days Of Inactivity Allowed: %d\n",
					spent.getDaysOfInactivityAllowed());
	stdoutput.printf("	Expiration Date: %d\n",
					spent.getExpirationDate());
	stdoutput.printf("	Flag: %d\n",spent.getFlag());
	stdoutput.printf("\n");

	// use the built in print method
	stdoutput.printf("Built In...\n");
	spent.print();
	stdoutput.printf("\n");
}
