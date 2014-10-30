// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetsocketclient.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create an inet socket client
	inetsocketclient	clnt;

	// connect to a server on localhost, listening on port 8000
	if (clnt.connect("rrtest",8000,-1,-1,1,1)<0) {
	//if (clnt.connect("127.0.0.1",8000,-1,-1,1,1)<0) {
	//if (clnt.connect("192.168.123.2",8000,-1,-1,1,1)<0) {
		stdoutput.printf("connect failed: %s\n",
					error::getErrorString());
		process::exit(1);
	}

	stdoutput.printf("%s\n",clnt.getPeerAddress());

	// write "hello" to the server
	clnt.write("hello",5);

	// read 10 bytes from the server and display them
	char	buffer[11];
	int	sizeread=clnt.read(buffer,10);
	buffer[sizeread]=(char)NULL;
	stdoutput.printf("%s\n",buffer);

	// close the connection to the server
	clnt.close();
}
