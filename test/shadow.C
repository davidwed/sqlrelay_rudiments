// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/shadowentry.h>

#include <stdio.h>


int main(int argc, const char **argv) {


	// get the shadow entry for "root"
	shadowentry	spent;
	spent.initialize("root");

	// print the components individually
	printf("Individually...\n");
	printf("	Name: %s\n",spent.getName());
	printf("	Encrypted Password: %s\n",spent.getEncryptedPassword());
	printf("	Last Change: %ld\n",
					spent.getLastChangeDate());
	printf("	Days Before Change Allowed: %d\n",
					spent.getDaysBeforeChangeAllowed());
	printf("	Days Before Change Required: %d\n",
					spent.getDaysBeforeChangeRequired());
	printf("	Days Before Expiration Warning: %d\n",
					spent.getDaysBeforeExpirationWarning());
	printf("	Days Of Inactivity Allowed: %d\n",
					spent.getDaysOfInactivityAllowed());
	printf("	Expiration Date: %d\n",
					spent.getExpirationDate());
	printf("	Flag: %d\n",spent.getFlag());
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	spent.print();
	printf("\n");
}
