// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixclientsocket.h>
#include <rudiments/permissions.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	unixclientsocket clnt;
	clnt.connect("/tmp/handoff.socket",-1,-1,0,1);

	for (;;) {
		int	descriptor;
		if (!clnt.receiveFileDescriptor(&descriptor)) {
			stdoutput.printf("receive failed\n");
			process::exit(1);
		}
		filedescriptor	clientsock;
		clientsock.setFileDescriptor(descriptor);
		clientsock.write("hello");
		clientsock.close();
	}
}
