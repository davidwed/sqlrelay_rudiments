// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixsocketclient.h>
#include <rudiments/permissions.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	unixsocketclient clnt;
	clnt.connect("handoff.sck",-1,-1,0,1);

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
