// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixclientsocket.h>
#include <rudiments/permissions.h>
#include <errno.h>
#include <strings.h>

int main(int argc, const char **argv) {

	unixclientsocket clnt;
	clnt.connect("/tmp/handoff.socket",-1,-1,0,1);

	for (;;) {
		int	descriptor;
		clnt.receiveFileDescriptor(&descriptor);
		filedescriptor	clientsock;
		clientsock.setFileDescriptor(descriptor);
		clientsock.write("hello");
		clientsock.close();
	}
}
