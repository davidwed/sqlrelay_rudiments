// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixclientsocket.h>
#include <rudiments/permissions.h>
#include <strings.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	unixclientsocket clnt;
	clnt.connect("/tmp/handoff.socket",-1,-1,0,1);

	for (;;) {
		int	descriptor;
		if (!clnt.receiveFileDescriptor(&descriptor)) {
			printf("receive failed\n");
		}
		filedescriptor	clientsock;
		clientsock.setFileDescriptor(descriptor);
		clientsock.write("hello");
		clientsock.close();
	}
}
