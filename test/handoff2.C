// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixclientsocket.h>
#include <rudiments/permissions.h>
#include <errno.h>
#include <strings.h>

int main(int argc, const char **argv) {

	unixclientsocket clnt;
	clnt.connectToServer("/tmp/handoff.socket",0,1);

	for (;;) {
		int	descriptor;
		clnt.receiveFileDescriptor(&descriptor);
		datatransport	clientsock(descriptor);
		clientsock.write("hello");
		clientsock.close();
	}
}
