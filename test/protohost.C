// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/hostentry.h>
#include <rudiments/protocolentry.h>

int main(int argv, const char **argc) {

	// get the host information for "localhost" and print it
	hostentry	he;
	he.initialize("localhost");
	he.print();

	// get the host information for "127.0.0.1" and print it
	char	address[]={127,0,0,1};
	he.initialize(address,4,AF_INET);
	he.print();


	// get the protocol information for "tcp" and print it
	protocolentry	pe;
	pe.initialize("tcp");
	pe.print();

	// get the protocol information for protocol number 6 and print it
	pe.initialize(6);
	pe.print();
}
