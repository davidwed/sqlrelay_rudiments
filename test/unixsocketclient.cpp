// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixsocketclient.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>

int main(int argc, const char **argv) {

	// create a unix socket client
	unixsocketclient	clnt;

	// connect to a server listening on lsnr.sck
	if (clnt.connect("lsnr.sck",-1,-1,1,1)<0) {
		stdoutput.printf("connect failed: %s\n",
				error::getErrorString());
		process::exit(0);
	}

	// write "hello" to the server
	clnt.write("hello",5);

	// read 10 bytes from the server and display them
	char	buffer[11];
	int	sizeread=clnt.read(buffer,10);
	buffer[sizeread]='\0';
	stdoutput.printf("%s\n",buffer);

	// close the connection to the server
	clnt.close();
}
