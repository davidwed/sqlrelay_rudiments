// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixclientsocket.h>

#include <stdio.h>

int main(int argc, const char **argv) {

	// create a unix socket client
	unixclientsocket	clnt;

	// connect to a server listening on /tmp/lsnr.socket
	clnt.connectToServer("/tmp/lsnr.socket",1,0);

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
