// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/hostentry.h>
#include <stdio.h>
#include <sys/socket.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

// this function takes addr[]={127,1,1,0} and returns "127.1.1.0"
char *getAddressString(int length, const char *addr) {
	char	*address=new char[(length*4)+1];
	address[0]=(char)NULL;
	for (int byte=0; byte<length; byte++) {
		sprintf(address,"%s%d",address,addr[byte]);
		if (byte<length-1) {
			sprintf(address,"%s.",address);
		}
	}
	return address;
}

int main(int argc, const char **argv) {


	// get the host information for "localhost"
	hostentry	he;
	he.initialize("localhost");

	// print the components individually
	printf("Individually...\n");
	printf("	Name: %s\n",he.getName());
	printf("	Alias list:\n");
	for (int i=0; he.getAliasList()[i]; i++) {
		printf("		%s\n",he.getAliasList()[i]);
	}
	printf("	Address type: %d\n",he.getAddressType());
	printf("	Address length: %d\n",he.getAddressLength());
	printf("	Address list:\n");
	for (int i=0; he.getAddressList()[i]; i++) {
		char	*addr=getAddressString(he.getAddressLength(),
						he.getAddressList()[i]);
		printf("		%s\n",addr);
		delete[] addr;
	}
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	he.print();
	printf("\n");



	// get the host information for "127.0.0.1"
	char	address[]={127,0,0,1};
	he.initialize(address,4,AF_INET);

	// print the components individually
	printf("Individually...\n");
	printf("	Name: %s\n",he.getName());
	printf("	Alias list:\n");
	for (int i=0; he.getAliasList()[i]; i++) {
		printf("		%s\n",he.getAliasList()[i]);
	}
	printf("	Address type: %d\n",he.getAddressType());
	printf("	Address length: %d\n",he.getAddressLength());
	printf("	Address list:\n");
	for (int i=0; he.getAddressList()[i]; i++) {
		char	*addr=getAddressString(he.getAddressLength(),
						he.getAddressList()[i]);
		printf("		%s\n",addr);
		delete[] addr;
	}
	printf("\n");

	// use the built in print method
	printf("Built In...\n");
	he.print();
	printf("\n");
}
