// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>

#include <rudiments/private/config.h>

#include <stdio.h>

int main(int argv, const char **argc) {

	// create an inet socket client
	inetclientsocket	clnt;

	// connect to a server on localhost, listening on port 8000
	clnt.connectToServer("localhost",8000,1,0);

	// write "hello" to the server
	clnt.write("hello",5);

	// read 10 bytes from the server and display them
	char	buffer[11];
	int	sizeread=clnt.read(buffer,10);
	buffer[sizeread]=(char)NULL;
	printf("%s\n",buffer);

	// close the connection to the server
	clnt.close();
}
