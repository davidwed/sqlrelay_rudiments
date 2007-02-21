// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// create an inet socket client
	inetclientsocket	clnt;

	// connect to a server on localhost, listening on port 8000
	clnt.connect("localhost",8000,-1,-1,1,0);

	// write "hello" to the server
	clnt.write("hello",5);

	// read 5 bytes from the server and display them
	char	buffer[6];
	int	sizeread=clnt.read(buffer,5);
	buffer[sizeread]=(char)NULL;
	printf("%s\n",buffer);

	// since the oneshotsvr will only send 1 message, then wait, we
	// can test read-with-timeout here
	if (clnt.read(buffer,10,1,0)==-1) {
		printf("timeout!!!\n");
	} else {
		printf("timeout failed!!!\n");
	}

	// close the connection to the server
	clnt.close();
}
