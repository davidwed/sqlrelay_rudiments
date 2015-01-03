// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/hostentry.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

// for AF_INET
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif
#ifdef RUDIMENTS_HAVE_WINSOCK2_H
	#include <winsock2.h>
#endif

// this function takes addr[]={127,1,1,0} and returns "127.1.1.0"
char *getAddressString(int length, const char *addr) {
	size_t	addrlen=(length*4)+1;
	char	*address=new char[addrlen];
	address[0]=(char)NULL;
	for (int byte=0; byte<length; byte++) {
		charstring::printf(address,addrlen,"%s%d",address,addr[byte]);
		if (byte<length-1) {
			charstring::printf(address,addrlen,"%s.",address);
		}
	}
	return address;
}

void print(hostentry *he) {

	// print the components individually
	stdoutput.printf("	Name: %s\n",he->getName());
	stdoutput.printf("	Alias list:\n");
	int	i;
	for (i=0; he->getAliasList() && he->getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",he->getAliasList()[i]);
	}
	stdoutput.printf("	Address type: %d\n",he->getAddressType());
	stdoutput.printf("	Address length: %d\n",he->getAddressLength());
	stdoutput.printf("	Address list:\n");
	for (i=0; he->getAddressList() && he->getAddressList()[i]; i++) {
		char	*addr=getAddressString(he->getAddressLength(),
						he->getAddressList()[i]);
		stdoutput.printf("		%s\n",addr);
		delete[] addr;
	}
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	// unintialized
	hostentry	he;
	stdoutput.printf("uninitialized:\n");
	print(&he);


	// get the host information for "localhost"
	he.initialize("localhost");
	stdoutput.printf("localhost:\n");
	print(&he);

	// get the host information for "127.0.0.1"
	char	address[]={127,0,0,1};
	he.initialize(address,4,AF_INET);
	stdoutput.printf("127.0.0.1:\n");
	print(&he);

	// null-safety
	he.initialize(NULL);
	stdoutput.printf("NULL:\n");
	print(&he);
}
