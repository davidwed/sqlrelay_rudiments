// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

int main(int argc, const char **argv) {

	// create an inet socket client
	inetclientsocket	clnt;

	// connect to a server on localhost, listening on port 8000
	//if (clnt.connectToServer("localhost",8000,5,0,1,1)<0) {
	if (clnt.connectToServer("localhost",8000,-1,-1,1,1)<0) {
		printf("connect failed: %s\n",strerror(errno));
		exit(1);
	}

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
