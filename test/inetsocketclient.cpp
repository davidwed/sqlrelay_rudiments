// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/randomnumber.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	commandline	cmdl(argc,argv);
	const char	*host="127.0.0.1";
	if (cmdl.found("host")) {
		host=cmdl.getValue("host");
	}
	uint16_t	port=8000;
	if (cmdl.found("port")) {
		port=charstring::toUnsignedInteger(cmdl.getValue("port"));
	}

	// create an inet socket client
	inetsocketclient	clnt;

	// connect to the server
	if (clnt.connect(host,port,-1,-1,1,1)<0) {
		stdoutput.printf("connect failed: %s\n",
					error::getErrorString());
		process::exit(1);
	}

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
